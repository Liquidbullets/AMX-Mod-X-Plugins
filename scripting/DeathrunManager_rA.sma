#include <amxmodx>
#include <hamsandwich>
#include <engine>
#include <nvault>
#include <fun>
#include <dhudmessage>
#include <cstrike>
#include <fakemeta>

#define VERSION "0.0.1"

#define SECONDS 30

new g_szThinkerClassname[]="afk_thinker"

#define OFFSET_LAST_MOVEMENT 124

new const szVault[]="TQuits"

new g_szAuthID[33][33]
new g_iBot[33]
new g_iFrags[33]

new g_pRewardPrice
new g_iVault
new g_iMsgScoreInfo
new const g_szVaultFile[] = "frag_save"

new bool:plrSolid[33] 
new bool:plrRestore[33] 
new plrTeam[33]

new maxplayers 

#define DISPLAY_MSG

new const itemcvar[5][] = {
	"amx_frag_gravitycost",
	"amx_frag_speedcost",
	"amx_frag_stealthcost",
	"amx_frag_lasercost",
	"amx_frag_healthcost"
}

new const itemname[5][] = {
	"low gravity",
	"speed",
	"stealth",
	"laser",
	"health"
}

new powerpurchase[33]

new bool:g_bMapEnd,
	bool:g_bFirstRound,
	g_iFirstTimer,
	g_iLastTerr,
	g_pBanTime,
	g_pAutoBalance,
	g_pLimitTeams,
	bool:g_bGameActive

new g_Lifes[33]

new bool:g_bEnabled
new g_pNoFallDmg
new g_pBlockKill

new g_afkTime

new g_iTBOT
new g_iCTBOT

new szCTBotName[32]="____www.ragaming.org_____"
new szTBotName[32]="____www.ragaming.org____"

public plugin_init() {
	register_plugin("DeathRun",VERSION,"Firippu")

	g_pBanTime=register_cvar("amx_bantime","20")

	g_pAutoBalance=get_cvar_pointer("mp_autoteambalance")
	g_pLimitTeams=get_cvar_pointer("mp_limitteams")

	g_pNoFallDmg=register_cvar("deathrun_terrnfd","1")
	g_pBlockKill=register_cvar("deathrun_blockkill","1")

	register_forward(FM_ClientKill,"FwdClientKill")
	RegisterHam(Ham_TakeDamage,"player","FwdHamPlayerDamage")

	g_bEnabled=true

	g_bMapEnd=false
	g_bFirstRound=true

	register_event("30","MapEnd","a")

	RegisterHam(Ham_Killed,"player","fwdPlayerKilled",1)
	RegisterHam(Ham_Spawn,"player","fwdPlayerSpawn",1)

	register_logevent("eNewRound",2,"1=Round_Start")

	set_cvar_num("mp_freezetime",0)

	g_afkTime = register_cvar("afk_time", "60.0")

	new iEntity
	if((iEntity = create_entity("info_target"))) {
		entity_set_string(iEntity,EV_SZ_classname,g_szThinkerClassname)
		entity_set_float(iEntity,EV_FL_nextthink,get_gametime() + get_pcvar_float(g_afkTime))
		register_think(g_szThinkerClassname,"checkAFK")
	}

	register_forward(FM_PlayerPreThink,"preThink")
	register_forward(FM_PlayerPostThink,"postThink")

	register_forward(FM_AddToFullPack,"AddToFullPack",1)

	maxplayers = get_maxplayers()

	register_event("ScoreInfo","fwEvScoreInfo","a")

	g_pRewardPrice = register_cvar("fs_rewardprice","2")
	g_iMsgScoreInfo = get_user_msgid("ScoreInfo")

	register_dictionary("Frag_Save.txt")
	
	register_cvar("amx_frag","1")
	register_cvar(itemcvar[0],"3")
	register_cvar(itemcvar[1],"6")
	register_cvar(itemcvar[2],"5")
	register_cvar(itemcvar[3],"2")
	register_cvar(itemcvar[4],"5")

	register_menucmd(register_menuid("Awards_System"),1023,"pickoption")

	register_clcmd("say /awards","showMENU")
	register_clcmd("say_team awards","showMENU")
	register_concmd("awards","showMENU")

	register_event("ResetHUD","eRespawn","b")
	register_event("CurWeapon","speedb","be","1=1")

	set_task(5.0,"CreateBot",1)
	set_task(6.0,"CreateBot",2)
}

public checkAFK(iEntity) {
	new Float:afk_time = get_pcvar_float(g_afkTime)

	new i,Float:cur_time,Float:lastActivity 

	cur_time = get_gametime()

	afk_time = cur_time - afk_time 
	for(i=1; i<=maxplayers; i++) {
		if(g_iTBOT!=i && g_iCTBOT!=i) {
			if(is_user_alive(i)) {
				lastActivity = get_pdata_float(i, OFFSET_LAST_MOVEMENT)
				if (lastActivity < afk_time) {
					server_cmd("kick #%d AFK",get_user_userid(i))
				}
			}
		}
	}

	entity_set_float(iEntity,EV_FL_nextthink,get_gametime() + get_pcvar_float(g_afkTime))
}

public LifeMenu(id) {
	new menu = menu_create("\rLife Menu:","life_menu_handler")
	menu_additem(menu,"\wUse Extra Life","1",0)
	menu_additem(menu,"\wNo Not Use","2",0)
	menu_setprop(menu,MPROP_EXIT,MEXIT_ALL)
	menu_display(id,menu,0)
}

public life_menu_handler(id,menu,item) {
	if(item == MENU_EXIT) {
		menu_destroy(menu)
		return PLUGIN_HANDLED
	}

	new data[6],szName[64]
	new access,callback

	menu_item_getinfo(menu,item,access,data,charsmax(data),szName,charsmax(szName),callback)

	new key = str_to_num(data)

	switch(key) {
		case 1: {
			if(!is_user_alive(id) && g_bGameActive) {
				ExecuteHam(Ham_CS_RoundRespawn,id)
				g_Lifes[id]--
				client_print(id,print_chat,"You used a life, %d left.",g_Lifes[id])
				menu_destroy(menu)
				return PLUGIN_HANDLED
			}
		} case 2: {
			
		}
	}

	menu_destroy(menu)
	return PLUGIN_HANDLED
}

public CreateBot(iTeam) {
	new id
	if(iTeam==1) {
		id=engfunc(EngFunc_CreateFakeClient,szTBotName)
	} else if(iTeam==2) {
		id=engfunc(EngFunc_CreateFakeClient,szCTBotName)
	}

	if(pev_valid(id)) {
		engfunc(EngFunc_FreeEntPrivateData,id)
		dllfunc(MetaFunc_CallGameEntity,"player",id)

		set_user_info(id,"rate","3500")
		set_user_info(id,"cl_updaterate","25")
		set_user_info(id,"cl_lw","1")
		set_user_info(id,"cl_lc","1")
		set_user_info(id,"cl_dlmax","128")
		set_user_info(id,"cl_righthand","1")
		set_user_info(id,"_vgui_menus","0")
		set_user_info(id,"_ah","0")
		set_user_info(id,"dm","0")
		set_user_info(id,"tracker","0")
		set_user_info(id,"friends","0")
		set_user_info(id,"*bot","1")

		set_pev(id,pev_flags,pev(id,pev_flags)| FL_FAKECLIENT)
		set_pev(id,pev_colormap,id)

		new szMsg[128]

		if(iTeam==1) {
			dllfunc(DLLFunc_ClientConnect,id,szTBotName,"127.0.0.1",szMsg)
		} else if(iTeam==2) {
			dllfunc(DLLFunc_ClientConnect,id,szCTBotName,"127.0.0.1",szMsg)
		}

		dllfunc(DLLFunc_ClientPutInServer,id)

		cs_set_user_team(id,iTeam)
		dllfunc(DLLFunc_Spawn,id)

		if(iTeam==1) {
			g_iTBOT=id
		} else if(iTeam==2) {
			g_iCTBOT=id
		}

		remove_user_flags(id,ADMIN_USER)
		new flags = read_flags("a")
		set_user_flags(id,flags)
	}
}

public client_command(client) {
	static const szJoinCommand[]="jointeam"

	static szCommand[10]
	read_argv(0,szCommand,9)

	if(equal(szCommand,szJoinCommand)&& CS_TEAM_T <=cs_get_user_team(client)<=CS_TEAM_CT) {
		return PLUGIN_HANDLED
	}

	return PLUGIN_CONTINUE
}

public FwdClientKill(const id) {
	if(!g_bEnabled || !is_user_alive(id))
		return FMRES_IGNORED
	
	if(get_pcvar_num(g_pBlockKill)|| cs_get_user_team(id)==CS_TEAM_T) {
		return FMRES_SUPERCEDE
	}
	
	return FMRES_IGNORED
}

public FwdHamPlayerDamage(id,idInflictor,idAttacker,Float:flDamage,iDamageBits) {
	if(get_pcvar_num(g_pNoFallDmg))
		if(iDamageBits & DMG_FALL)
			if(get_user_team(id)==1)
				return HAM_SUPERCEDE
	
	return HAM_IGNORED
}

public fnTimer() {
	if(g_bFirstRound) {
		if(g_iFirstTimer>=SECONDS) {
			g_bFirstRound=false
			KillEverybody()
		} else {
			PrintHudMessage(1.0,"WARM UP ROUND")
			g_iFirstTimer++
			set_task(1.0,"fnTimer")
		}
	}
}

public fwdPlayerSpawn(iPlayer) {
	if(!is_user_bot(iPlayer)) {
		if(is_user_alive(iPlayer)) {
			strip_user_weapons(iPlayer)
			give_item(iPlayer,"weapon_knife")

			if(!g_bFirstRound) {
				if(cs_get_user_team(iPlayer)==CS_TEAM_T && g_bGameActive && g_iLastTerr!=iPlayer) {
					cs_set_user_team(iPlayer,CS_TEAM_CT)
					ExecuteHam(Ham_CS_RoundRespawn,iPlayer)
				}
			}
		}
	} else {
		set_pev(iPlayer,pev_effects,pev(iPlayer,pev_effects)| EF_NODRAW)
		set_pev(iPlayer,pev_solid,SOLID_NOT)
		set_pev(iPlayer,pev_renderamt,0.0)
		entity_set_origin(iPlayer,Float:{9999.0,9999.0,9999.0})
	}
}

public Randomize() {
	if(!g_bGameActive) {
		new aPlayers[32],iPlayers,ArrayPosition
		for(new i=1; i<maxplayers; i++) {
			if(g_iTBOT!=i && g_iCTBOT!=i) {
				if(is_user_alive(i)) {
					iPlayers++
					aPlayers[ArrayPosition]=i
					ArrayPosition++
					//client_print(0,print_chat,"Current ID: %d",i)
					//client_print(0,print_chat,"Stored ID: %d",aPlayers[i])
				}
			}
		}

		//client_print(0,print_chat,"Players: %d",iPlayers)
		if(iPlayers>1) {
			for(new i=1; i<maxplayers; i++) {
				if(g_iTBOT!=i && g_iCTBOT!=i) {
					if(is_user_alive(i)) {
						cs_set_user_team(i,CS_TEAM_CT)
						ExecuteHam(Ham_CS_RoundRespawn,i)
					}
				}
			}

			new iNewT,bool:bChoosen
			while(!bChoosen) {
				iNewT=aPlayers[random(ArrayPosition)]
				if(iNewT!=g_iLastTerr) {
					bChoosen=true
					//client_print(0,print_chat,"Player Chosen: %d",iNewT)
					cs_set_user_team(iNewT,CS_TEAM_T)
					g_iLastTerr=iNewT
					ExecuteHam(Ham_CS_RoundRespawn,iNewT)

					g_bGameActive=true
					PrintHudMessage(1.0,"Game Active")
				}
			}
		} else {
			// not enough players
		}
	}
}

public PrintHudMessage(Float:iSeconds,szMessage[]) {
	set_dhudmessage(0,160,0,-0.95,-0.25,2,6.0,iSeconds,0.1,1.5)
	show_dhudmessage(0,szMessage)
}

public eNewRound() {
	set_pcvar_num(g_pAutoBalance,0)
	set_pcvar_num(g_pLimitTeams,0)

	for(new iPlayer=1; iPlayer<maxplayers; iPlayer++) {
		if(is_user_alive(iPlayer)) {
			if(g_Lifes[iPlayer]) {
				client_print(iPlayer,print_chat,"You have %d extra lives.",g_Lifes[iPlayer])
			}
		}
	}

	if(!g_bFirstRound) {
		Randomize()
	}

	if(!g_bGameActive) {
		g_iFirstTimer=0
		g_bFirstRound=true
		fnTimer()
	}
}

public client_connect(id) {
	powerpurchase[id] = 0
}

public client_disconnect(iPlayer) {
	if(!g_iBot[iPlayer] && g_iFrags[iPlayer]) {
		new szFrags[6]
		num_to_str(g_iFrags[iPlayer],szFrags,5)

		nvault_set(g_iVault,g_szAuthID[iPlayer],szFrags)
	}

	g_iFrags[iPlayer] = 0
	g_iBot[iPlayer] = 0

	g_Lifes[iPlayer]=0

	if(g_bGameActive && !g_bFirstRound) {
		new ct_alive_players[32],ct_alive_players_num
		get_players(ct_alive_players,ct_alive_players_num,"ace","CT")

		if(g_iLastTerr==iPlayer && !g_bMapEnd && ct_alive_players_num>0) {
			new szName[32]
			get_user_name(iPlayer,szName,31)

			new szIdentifier[32]
			GetIdentifier(iPlayer,szIdentifier)

			new iVault=nvault_open(szVault)

			nvault_set(iVault,szIdentifier,szName)

			nvault_close(iVault)

			//////////

			static Float:vOrigin[3],Float:vVelocity[3]

			entity_get_vector(iPlayer,EV_VEC_origin,vOrigin)
			entity_get_vector(iPlayer,EV_VEC_velocity,vVelocity)

			new bFoundNewT
			while(!bFoundNewT) {
				new iNewT=ct_alive_players[random(ct_alive_players_num)]

				if(is_user_alive(iNewT)) {
					bFoundNewT=true
					cs_set_user_team(iNewT,CS_TEAM_T)

					if((ct_alive_players_num-1)==0) {
						g_bGameActive=false
						KillPlayer(g_iCTBOT)
						KillPlayer(g_iTBOT)
					} else {
						entity_set_origin(iNewT,vOrigin)
						set_user_velocity(iNewT,vVelocity)

						g_iLastTerr=iNewT

						PrintHudMessage(3.0,"Terrorist left; appointed new one.")
					}
				}
			}
		}

		if(cs_get_user_team(iPlayer)==CS_TEAM_CT) {
			if((ct_alive_players_num-1)==0) {
				g_bGameActive=false
				KillPlayer(g_iCTBOT)
			}
		}
	}
}

public client_putinserver(iPlayer) {
	g_iBot[iPlayer] = is_user_bot(iPlayer)

	if(!g_iBot[iPlayer]) {
		get_user_authid(iPlayer,g_szAuthID[iPlayer],32)

		new iFrags = nvault_get(g_iVault,g_szAuthID[iPlayer])

		if(iFrags) {
			g_iFrags[iPlayer] = iFrags
			set_user_frags(iPlayer,iFrags)
		}
	}

	if(!g_iBot[iPlayer]) {
		cs_set_user_team(iPlayer,CS_TEAM_UNASSIGNED)
		#if defined DISPLAY_MSG
		set_task(25.0,"inform",iPlayer)
		#endif
	}
}

public client_authorized(iPlayer) {
	new iTimestamp
	new iVault=nvault_open(szVault)

	new iCurrentTime=get_systime()

	new szIdentifier[32]
	GetIdentifier(iPlayer,szIdentifier)

	new szValue[32]
	new iResultCode=nvault_lookup(iVault,szIdentifier,szValue,charsmax(szValue),iTimestamp)

	if(iResultCode) {
		if((iTimestamp+(get_pcvar_num(g_pBanTime)*60))<iCurrentTime) {
			nvault_remove(iVault,szIdentifier)
		} else {
			server_cmd("kick #%d Temporary banned for quiting as Terrorist.",get_user_userid(iPlayer))
		}
	}

	nvault_close(iVault)
}

public fwdPlayerKilled(iPlayer,iKiller) {
	set_pev(iPlayer,pev_groupinfo,0)
	if(g_bGameActive) {
		if(g_iLastTerr==iPlayer) {
			PrintHudMessage(3.0,"Counter Terrorist Wins")
			KillPlayer(g_iTBOT)
			if(is_user_alive(iKiller)) {
				g_Lifes[iKiller]++
				client_print(iKiller,print_chat,"You gained an extra life!")
			}

			g_bGameActive=false
		} else {
			new ctplayers[32],ctplayers_num
			get_players(ctplayers,ctplayers_num,"ace","CT")

			if(ctplayers_num==0) {
				PrintHudMessage(3.0,"Terrorist Wins")
				KillPlayer(g_iCTBOT)

				//g_Lifes[g_iLastTerr]++
				//client_print(g_iLastTerr,print_chat,"You gained an extra life!")

				g_iFrags[g_iLastTerr]+=3

				g_bGameActive=false
			} else {
				if(g_Lifes[iPlayer]) {
					LifeMenu(iPlayer)
				}
			}
		}
	}
}

public GetIdentifier(iPlayer,szIdentifier[]) {
	get_user_authid(iPlayer,szIdentifier,31)

	if(!IsValidSteamID(szIdentifier)) {
		get_user_ip(iPlayer,szIdentifier,31,1)
	}
}

bool:IsValidSteamID(const szSteamID[]) {
	return(('0'<=szSteamID[8]<='1')&& szSteamID[9]==':' && equal(szSteamID,"STEAM_0:",8)&& is_str_num(szSteamID[10])&& strlen(szSteamID)<20)
}

public MapEnd() {
	g_bMapEnd=true
	g_bGameActive=false
}

public bool:bPlayerInTeam(iPlayer) {
	switch(cs_get_user_team(iPlayer)) {
		case 1..2: {
			return true
		}
	}

	return false
}

public KillPlayer(iPlayer) {
	if(is_user_alive(iPlayer)) {
		cs_set_user_deaths(iPlayer,(cs_get_user_deaths(iPlayer)-1))
		fakedamage(iPlayer,"player",1000.0,0)
	}
}

public KillEverybody() {
	for(new iPlayer=1; iPlayer<maxplayers; iPlayer++) {
		KillPlayer(iPlayer)
	}
}

public AddToFullPack(es, e, ent, host, hostflags, player, pSet) {
	if(player) {
		if(plrSolid[host] && plrSolid[ent] && plrTeam[host] == plrTeam[ent]) {
			set_es(es, ES_Solid, SOLID_NOT)
			set_es(es, ES_RenderMode, kRenderTransAlpha)
			set_es(es, ES_RenderAmt, 130)
		}
	}
}

FirstThink() {
	for(new i = 1; i <= maxplayers; i++) {
		if(!is_user_alive(i)) {
			plrSolid[i] = false
			continue
		}

		plrTeam[i] = get_user_team(i)
		plrSolid[i] = pev(i, pev_solid) == SOLID_SLIDEBOX ? true : false
	}
}

public preThink(id) {
	static i, LastThink

	if(LastThink > id) {
		FirstThink()
	}

	LastThink = id

	if(!plrSolid[id])
		return

	for(i = 1; i <= maxplayers; i++) {
		if(!plrSolid[i] || id == i)
			continue

		if(plrTeam[i] == plrTeam[id]) {
			set_pev(i, pev_solid, SOLID_NOT)
			plrRestore[i] = true
		}
	}
}

public postThink(id) {
	static i
	static Float:gravity

	for(i = 1; i <= maxplayers; i++) {
		if(plrRestore[i]) {
			pev(i, pev_gravity, gravity )
			set_pev(i, pev_solid, SOLID_SLIDEBOX)
			set_pev(i, pev_gravity, gravity )
			plrRestore[i] = false
		}
	}
}

public plugin_precache() {
	new Entity = create_entity("info_map_parameters")

	DispatchKeyValue(Entity,"buying","3")
	DispatchSpawn(Entity)
}

public pfn_keyvalue(Entity) {
	new ClassName[20],Dummy[2]
	copy_keyvalue(ClassName,charsmax(ClassName),Dummy,charsmax(Dummy),Dummy,charsmax(Dummy))

	if(equal(ClassName,"info_map_parameters")) {
		remove_entity(Entity)
		return PLUGIN_HANDLED
	}

	return PLUGIN_CONTINUE
}

public plugin_cfg() {
	g_iVault = nvault_open(g_szVaultFile)

	if(g_iVault == INVALID_HANDLE)
		set_fail_state("Error opening nVault")
}

public plugin_end() {
	nvault_close(g_iVault)
}

public fwEvScoreInfo() {
	new id = read_data(1)
	new iFrags = read_data(2)

	//The server resets scores at mapchange so this will set the players score [if g_iFrags[] > 0].
	if(!iFrags && g_iFrags[id]) {
		set_user_frags(id,g_iFrags[id])
		return PLUGIN_CONTINUE
	}

	g_iFrags[id] = iFrags

	return PLUGIN_CONTINUE
}

public cmdBuyReward(id) {
	new iRewardPrice = get_pcvar_num(g_pRewardPrice)

	if(g_iFrags[id] >= iRewardPrice) {
		g_iFrags[id] -= iRewardPrice
		set_user_frags(id,g_iFrags[id])
		cmdUpdateScoreBoard(id)
	}
}

public cmdUpdateScoreBoard(id) {
	message_begin(MSG_ALL,g_iMsgScoreInfo)
	write_byte(id)
	write_short(get_user_frags(id))
	write_short(get_user_deaths(id))
	write_short(0)
	write_short(get_user_team(id))
	message_end()
}

#if defined DISPLAY_MSG
public inform(id) {
	client_print(id,print_chat,"[A_S]: This server is running Awards_System By -Acid-")
	client_print(id,print_chat,"[A_S]: Type /awards to start buying!")
	return PLUGIN_HANDLED
}
#endif

public showMENU(id) {
	new menu[192]
	new keys = MENU_KEY_0|MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_9

	format(menu,191,"Awards_System^n^n1. Gravity^n2. Speed^n3. Stealth^n4. Laser^n5. Health^n^n0. Exit")
	show_menu(id,keys,menu,_,"Awards_System")
	return PLUGIN_HANDLED
}

public pickoption(id,key) {
	if(!get_cvar_num("amx_frag")) {
		client_print(id,print_chat,"[A_S]: Sorry,the plugin is off!")

		return PLUGIN_HANDLED
	} else if(!is_user_alive(id)) {
		client_print(id,print_chat,"[A_S]: You must be alive to purchase this item!")

		return PLUGIN_HANDLED
	} else if(powerpurchase[id]) {
		client_print(id,print_chat,"[A_S]: You already purchased a different skill!")

		return PLUGIN_HANDLED
	}

	new frags = get_user_frags(id)
	new cost = get_cvar_num(itemcvar[key])

	if(cost >= frags) {
		client_print(id,print_chat,"[A_S]: You don't have enough frags!")

		return PLUGIN_HANDLED
	}

	set_user_frags(id,frags - cost)

	client_print(id,print_chat,"[A_S]: You purchased %s for %i frags!",itemname[key],cost)

	switch(key + 1) {
		case 1: {
			set_user_gravity(id,0.4)
		} case 2: {
			new Float:speed = get_user_maxspeed(id) + 680.0
			set_user_maxspeed(id,speed)
		} case 3: {
			set_user_rendering(id,kRenderFxNone,50,50,50,kRenderTransAdd,50)
			set_user_footsteps(id,1)
		} case 4: {
			set_hudmessage(255,0,0,-1.0,-1.0,0,6.0,30.0,0.1,0.1,10)
			show_hudmessage(id,"*")
		} case 5: {
			set_user_health(id,get_user_health(id) + 50)
		} default: {
			return PLUGIN_HANDLED
		}
	}

	powerpurchase[id] = key + 1

	return PLUGIN_HANDLED
}

public speedb(id) {
	if(powerpurchase[id] == 2) {
		new Float:speed = get_user_maxspeed(id) + 680.0
		set_user_maxspeed(id,speed)
	}
}

public eRespawn(id) { // Reset Purchase
	switch(powerpurchase[id]) {
		case 1:{
			client_print(id,print_chat,"[A_S]: Gravity set back to normal!")
		} case 2: {
			client_print(id,print_chat,"[A_S]: Speed set back to normal!")
		} case 3: {
			set_user_rendering(id,kRenderFxNone,255,255,255,kRenderNormal,16)
			set_user_footsteps(id,0)

			client_print(id,print_chat,"[A_S]: Stealth level set back to normal!")
		} case 4: {
			client_print(id,print_chat,"[A_S]: Laser bulb burnt out!")
		}
	}

	powerpurchase[id] = 0

	return PLUGIN_HANDLED
}

