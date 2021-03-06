#include <amxmodx>
#include <hamsandwich>
#include <engine>
#include <nvault>
#include <fun>
#include <dhudmessage>
#include <cstrike>
#include <fakemeta>
#include <xs>

#define VERSION "0.0.1"

#define WARMUP_TIME 30

#define OFFSET_LAST_MOVEMENT 124
#define OFFSET_PRIMARYWEAPON 116

#pragma reqlib "GHW_NextMap"
native iGHW_MapChanging()
#pragma reqlib "AMX_NextMap"
native iAMX_MapChanging()
#pragma reqlib "AMXMenu_NextMap"
native iAMXMenu_MapChanging()


new g_szThinkerClassname[]="afk_thinker"

new const g_szQuit_Vault[]="TQuits"

new g_szIdentifier[33][35]
new g_iFrags[33]

new g_iVault
new g_iMsgScoreInfo
new const g_szFrag_Vault[] = "frag_save"

new g_iCurrentAward[33]

new g_iMaxPlayers

new Float:playerDelay[33]

new iAlive[33]

new bool:g_bWarmUpRound,
	g_iWarmUpTimer,
	g_iLastTerr,
	g_pQuitBanTime,
	g_pAutoBalance,
	g_pLimitTeams,
	bool:g_bGameActive

new g_ExtraLifes[33]

new g_afkTime

new g_iTBOT
new g_iCTBOT

public plugin_init() {
	register_plugin("DeathRun",VERSION,"Firippu")

	g_pQuitBanTime=register_cvar("amx_bantime","20")

	g_pAutoBalance=get_cvar_pointer("mp_autoteambalance")
	g_pLimitTeams=get_cvar_pointer("mp_limitteams")

	register_forward(FM_ClientKill,"FwdClientKill")
	RegisterHam(Ham_TakeDamage,"player","FwdHamPlayerDamage")

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

	g_iMaxPlayers = get_maxplayers()

	register_event("ScoreInfo","fwEvScoreInfo","a")

	g_iMsgScoreInfo = get_user_msgid("ScoreInfo")

	register_clcmd("say /awards","fnAward_System_Menu")
	register_clcmd("say awards","fnAward_System_Menu")
	register_clcmd("awards","fnAward_System_Menu")
	register_event("CurWeapon","speedb","be","1=1")

	set_task(5.0,"CreateBot",1)
	set_task(6.0,"CreateBot",2)

	
	
	
	// Register: Forward
	register_forward(FM_PlayerPreThink, "preThink")
	register_forward(FM_PlayerPostThink, "postThink")
	
	// Register: Ham
	RegisterHam(Ham_Touch, "func_door", "Ham_Semiclip_Touched", 1)
	RegisterHam(Ham_Touch, "func_door_rotating", "Ham_Semiclip_Touched", 1)
	RegisterHam(Ham_Touch, "func_train", "Ham_Semiclip_Touched", 1)
	RegisterHam(Ham_Touch, "func_rotating", "Ham_Semiclip_Touched", 1)
	RegisterHam(Ham_Touch, "func_tank", "Ham_Semiclip_Touched", 1)
	
	
	
	StartWarmUp()
}

public checkAFK(iEntity) {
	new Float:afk_time = get_pcvar_float(g_afkTime)

	new i,Float:cur_time,Float:lastActivity 

	cur_time = get_gametime()

	afk_time = cur_time - afk_time 
	for(i=1; i<=g_iMaxPlayers; i++) {
		if(g_iTBOT!=i && g_iCTBOT!=i) {
			if(is_user_alive(i)) {
				lastActivity = get_pdata_float(i, OFFSET_LAST_MOVEMENT)
				if(lastActivity < afk_time) {
					server_cmd("kick #%d  AFK",get_user_userid(i))
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
				g_ExtraLifes[id]--
				static szPlayerName[32]
				get_user_name(id,szPlayerName,31)
				client_print(0,print_chat,"%s used an extra life.",szPlayerName)
				client_print(id,print_chat,"You have %d lives left.",g_ExtraLifes[id])
				menu_destroy(menu)
				return PLUGIN_HANDLED
			}
		} case 2: {
			
		} default: {
			return PLUGIN_HANDLED
		}
	}

	menu_destroy(menu)
	return PLUGIN_HANDLED
}

public CreateBot(iTeam) {
	new szBotName[32]="www.ragaming.org"
	new id=engfunc(EngFunc_CreateFakeClient,szBotName)

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

	dllfunc(DLLFunc_ClientConnect,id,szBotName,"127.0.0.1",szMsg)

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

public client_command(client) {
	static const szJoinCommand[]="jointeam"

	static szCommand[10]
	read_argv(0,szCommand,9)

	if(equal(szCommand,szJoinCommand) && g_bGameActive && cs_get_user_team(client)==CS_TEAM_T) {
		return PLUGIN_HANDLED
	}

	return PLUGIN_CONTINUE
}

public FwdClientKill(const id) {
	if(!is_user_alive(id))
		return FMRES_IGNORED

	if(cs_get_user_team(id)==CS_TEAM_T) {
		return FMRES_SUPERCEDE
	}

	return FMRES_IGNORED
}

public FwdHamPlayerDamage(id,idInflictor,idAttacker,Float:flDamage,iDamageBits) {
	static szPlayerName[32]
	get_user_name(id,szPlayerName,31)

	if(iDamageBits & DMG_FALL) {
		//client_print(0,print_chat,"DEBUG: %s hurt by DMG_FALL",szPlayerName)
		if(get_user_team(id)==1) {
			return HAM_SUPERCEDE
		}
	}
/*
	if(iDamageBits & DMG_GENERIC) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_GENERIC",szPlayerName)
	} else if(iDamageBits & DMG_CRUSH) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_CRUSH",szPlayerName)
	} else if(iDamageBits & DMG_BULLET) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_BULLET",szPlayerName)
	} else if(iDamageBits & DMG_SLASH) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_SLASH",szPlayerName)
	} else if(iDamageBits & DMG_BURN) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_BURN",szPlayerName)
	} else if(iDamageBits & DMG_FREEZE) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_FREEZE",szPlayerName)
	} else if(iDamageBits & DMG_BLAST) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_BLAST",szPlayerName)
	} else if(iDamageBits & DMG_CLUB) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_CLUB",szPlayerName)
	} else if(iDamageBits & DMG_SHOCK) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_SHOCK",szPlayerName)
	} else if(iDamageBits & DMG_SONIC) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_SONIC",szPlayerName)
	} else if(iDamageBits & DMG_ENERGYBEAM) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_ENERGYBEAM",szPlayerName)
	} else if(iDamageBits & DMG_NEVERGIB) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_NEVERGIB",szPlayerName)
	} else if(iDamageBits & DMG_ALWAYSGIB) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_ALWAYSGIB",szPlayerName)
	} else if(iDamageBits & DMG_DROWN) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_DROWN",szPlayerName)
	} else if(iDamageBits & DMG_TIMEBASED) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_TIMEBASED",szPlayerName)
	} else if(iDamageBits & DMG_PARALYZE) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_PARALYZE",szPlayerName)
	} else if(iDamageBits & DMG_NERVEGAS) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_NERVEGAS",szPlayerName)
	} else if(iDamageBits & DMG_POISON) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_POISON",szPlayerName)
	} else if(iDamageBits & DMG_RADIATION) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_RADIATION",szPlayerName)
	} else if(iDamageBits & DMG_DROWNRECOVER) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_DROWNRECOVER",szPlayerName)
	} else if(iDamageBits & DMG_ACID) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_ACID",szPlayerName)
	} else if(iDamageBits & DMG_SLOWBURN) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_SLOWBURN",szPlayerName)
	} else if(iDamageBits & DMG_SLOWFREEZE) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_SLOWFREEZE",szPlayerName)
	} else if(iDamageBits & DMG_MORTAR) {
		client_print(0,print_chat,"DEBUG: %s hurt by DMG_MORTAR",szPlayerName)
	}
*/
	return HAM_IGNORED
}

public fnWarmUpTimer() {
	if(g_bWarmUpRound) {
		if(g_iWarmUpTimer>=WARMUP_TIME) {
			g_bWarmUpRound=false

			new id,ivalid,bool:bEnough
			for(id=1; id<g_iMaxPlayers; id++) {
				if(is_user_connected(id)) {
					if(bPlayerInTeam(id) && !is_user_bot(id)) {
						ivalid++
						if(ivalid>1) {
							bEnough=true
							continue
						}
					}
				}
			}

			if(bEnough) {
				KillAll()
			} else {
				set_task(3.0,"StartWarmUp")
			}
		} else {
			PrintHudMessage(1.0,"WARM UP ROUND")
			g_iWarmUpTimer++
			set_task(1.0,"fnWarmUpTimer")
		}
	}
}

public fwdPlayerSpawn(iPlayer) {
	if(!is_user_bot(iPlayer)) {
		if(is_user_alive(iPlayer)) {
			switch(g_iCurrentAward[iPlayer]) {
				case 1: {
					client_print(iPlayer,print_chat,"[A_S]: Gravity set back to normal!")
				} case 2: {
					client_print(iPlayer,print_chat,"[A_S]: Speed set back to normal!")
				} case 3: {
					set_user_rendering(iPlayer,kRenderFxNone,255,255,255,kRenderNormal,16)
					set_user_footsteps(iPlayer,0)

					client_print(iPlayer,print_chat,"[A_S]: Stealth level set back to normal!")
				}
			}

			g_iCurrentAward[iPlayer]=0

			
			
			
			
			iAlive[iPlayer]=true
			set_pev(iPlayer, pev_solid, SOLID_SLIDEBOX)

			
			
			strip_user_weapons(iPlayer)
			set_pdata_int(iPlayer,OFFSET_PRIMARYWEAPON,0)
			give_item(iPlayer,"weapon_knife")

			if(!g_bWarmUpRound) {
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
		new id,iAlivePlayer_Num,iAlivePlayer_ID[33]
		for(id=1; id<g_iMaxPlayers; id++) {
			if(is_user_connected(id)) {
				if(bPlayerInTeam(id) && is_user_alive(id) && !is_user_bot(id) && id!=g_iLastTerr) {
					iAlivePlayer_ID[iAlivePlayer_Num]=id
					iAlivePlayer_Num++
				}
			}
		}

		if(iAlivePlayer_Num>0) {
			for(new i=1; i<g_iMaxPlayers; i++) {
				if(g_iTBOT!=i && g_iCTBOT!=i) {
					if(is_user_alive(i)) {
						cs_set_user_team(i,CS_TEAM_CT)
						ExecuteHam(Ham_CS_RoundRespawn,i)
					}
				}
			}

			new iNewT=iAlivePlayer_ID[random(iAlivePlayer_Num)]
			cs_set_user_team(iNewT,CS_TEAM_T)
			g_iLastTerr=iNewT
			ExecuteHam(Ham_CS_RoundRespawn,iNewT)

			g_bGameActive=true
			PrintHudMessage(1.0,"Game Active")
		} else {
			StartWarmUp()
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

	for(new iPlayer=1; iPlayer<g_iMaxPlayers; iPlayer++) {
		if(is_user_alive(iPlayer)) {
			if(g_ExtraLifes[iPlayer]) {
				client_print(iPlayer,print_chat,"You have %d extra lives.",g_ExtraLifes[iPlayer])
			}
		}
	}

	if(!g_bWarmUpRound) {
		Randomize()
	}
}

public StartWarmUp() {
	if(!g_bWarmUpRound) {
		g_iLastTerr=0
		g_iWarmUpTimer=0
		g_bWarmUpRound=true
		g_bGameActive=false
		fnWarmUpTimer()
	}
}

public client_disconnect(iPlayer) {
	if(!is_user_bot(iPlayer) && g_iFrags[iPlayer]) {
		new szFrags[6]
		num_to_str(g_iFrags[iPlayer],szFrags,5)

		nvault_set(g_iVault,g_szIdentifier[iPlayer],szFrags)
	}

	g_iFrags[iPlayer] = 0

	g_ExtraLifes[iPlayer]=0

	g_iCurrentAward[iPlayer]=0

	g_szIdentifier[iPlayer][0]='^0'
	
	iAlive[iPlayer]=false

	if(g_bGameActive && !g_bWarmUpRound) {
		new id,iAliveCT_Num,iAliveCT_ID[33],iDeadCT_Num
		for(id=1; id<g_iMaxPlayers; id++) {
			if(is_user_connected(id)) {
				if((cs_get_user_team(id)==CS_TEAM_CT) && !is_user_bot(id) && iPlayer!=id) {
					if(is_user_alive(id)) {
						iAliveCT_ID[iAliveCT_Num]=id
						iAliveCT_Num++
					} else {
						iDeadCT_Num++
					}
				}
			}
		}

		if(g_iLastTerr==iPlayer && !iAMX_MapChanging() && !iGHW_MapChanging() && !iAMXMenu_MapChanging() && get_timeleft()>10) {
			new szName[32]
			get_user_name(iPlayer,szName,31)

			new szIdentifier[32]
			GetIdentifier(iPlayer,szIdentifier)

			new iVault=nvault_open(g_szQuit_Vault)

			nvault_set(iVault,szIdentifier,szName)

			nvault_close(iVault)

			//////////

			if(iAliveCT_Num>1) {
				static Float:vOrigin[3],Float:vVelocity[3]

				entity_get_vector(iPlayer,EV_VEC_origin,vOrigin)
				entity_get_vector(iPlayer,EV_VEC_velocity,vVelocity)


				new iNewT=iAliveCT_ID[random(iAliveCT_Num)]

				cs_set_user_team(iNewT,CS_TEAM_T)

				entity_set_origin(iNewT,vOrigin)
				set_user_velocity(iNewT,vVelocity)

				
				

				set_pev(iNewT, pev_solid, SOLID_SLIDEBOX)
				
				
				g_iLastTerr=iNewT

				PrintHudMessage(3.0,"Terrorist left; appointed new one.")
			} else {
				StartWarmUp()
			}
		}

		if(iAliveCT_Num<=0) {
			if(iDeadCT_Num<=0) {
				StartWarmUp()
			} else {
				KillPlayer(g_iCTBOT)
			}
		}
	}
}

public client_connect(id) {
	g_iFrags[id]=0
	g_szIdentifier[id][0]='^0'
}

public client_putinserver(iPlayer) {
	if(!is_user_bot(iPlayer)) {
		new iFrags = nvault_get(g_iVault,g_szIdentifier[iPlayer])

		if(iFrags) {
			g_iFrags[iPlayer] = iFrags
			set_user_frags(iPlayer,iFrags)
		}

		cs_set_user_team(iPlayer,CS_TEAM_UNASSIGNED)
	}
}

public client_authorized(iPlayer) {
	new iTimestamp
	new iVault=nvault_open(g_szQuit_Vault)

	new iCurrentTime=get_systime()

	GetIdentifier(iPlayer,g_szIdentifier[iPlayer])

	new szValue[32]
	new iResultCode=nvault_lookup(iVault,g_szIdentifier[iPlayer],szValue,charsmax(szValue),iTimestamp)

	if(iResultCode) {
		if((iTimestamp+(get_pcvar_num(g_pQuitBanTime)*60))<iCurrentTime) {
			nvault_remove(iVault,g_szIdentifier[iPlayer])
		} else {
			server_cmd("kick #%d Temporary banned for quiting as Terrorist.",get_user_userid(iPlayer))
		}
	}

	nvault_close(iVault)
}

public fwdPlayerKilled(iVictim,iKiller) {
	iAlive[iVictim]=false
	if(g_bGameActive && iVictim!=g_iCTBOT && iVictim!=g_iTBOT) {
		if(g_iLastTerr==iVictim) {
			g_bGameActive=false
			PrintHudMessage(3.0,"Counter Terrorist Wins")
			KillPlayer(g_iTBOT)
			if(iAlive[iKiller]) {
				g_ExtraLifes[iKiller]++
				client_print(iKiller,print_chat,"You gained an extra life!")
			}
		} else {
			new ctplayers[32],ctplayers_num
			get_players(ctplayers,ctplayers_num,"ace","CT")

			if(ctplayers_num==0) {
				g_bGameActive=false
				PrintHudMessage(3.0,"Terrorist Wins")
				KillPlayer(g_iCTBOT)

				//g_ExtraLifes[g_iLastTerr]++
				//client_print(g_iLastTerr,print_chat,"You gained an extra life!")

				g_iFrags[g_iLastTerr]+=3
				set_user_frags(g_iLastTerr,g_iFrags[g_iLastTerr])
				cmdUpdateScoreBoard(g_iLastTerr)
			} else {
				if(g_ExtraLifes[iVictim]) {
					LifeMenu(iVictim)
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

public KillAll() {
	for(new iPlayer=1; iPlayer<g_iMaxPlayers; iPlayer++) {
		KillPlayer(iPlayer)
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
	g_iVault = nvault_open(g_szFrag_Vault)

	if(g_iVault == INVALID_HANDLE)
		set_fail_state("Error opening nVault")
}

public plugin_end() {
	nvault_close(g_iVault)
	g_bGameActive=false
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

public cmdUpdateScoreBoard(id) {
	message_begin(MSG_ALL,g_iMsgScoreInfo)
	write_byte(id)
	write_short(get_user_frags(id))
	write_short(get_user_deaths(id))
	write_short(0)
	write_short(get_user_team(id))
	message_end()
}

public fnAward_System_Menu(id) {
	new menu = menu_create("\rAward System Menu:","award_system_menu_handler")
	menu_additem(menu,"\wGravity","1",0)
	menu_additem(menu,"\wSpeed","2",0)
	menu_additem(menu,"\wStealth","3",0)
	menu_additem(menu,"\wHealth","4",0)
	menu_setprop(menu,MPROP_EXIT,MEXIT_ALL)
	menu_display(id,menu,0)
}

public award_system_menu_handler(id,menu,item) {
	if(item == MENU_EXIT) {
		menu_destroy(menu)
		return PLUGIN_HANDLED
	}

	if(!is_user_alive(id)) {
		client_print(id,print_chat,"[A_S]: You cannot use Awards System when dead.")
		return PLUGIN_HANDLED
	}

	if(g_iCurrentAward[id]) {
		client_print(id,print_chat,"[A_S]: You already have an award.")
		return PLUGIN_HANDLED
	}

	new iFrags = get_user_frags(id)

	new data[6],szName[64]
	new access,callback

	menu_item_getinfo(menu,item,access,data,charsmax(data),szName,charsmax(szName),callback)

	new key = str_to_num(data)

	switch(key) {
		case 1: {
			if(iFrags>=3) {
				set_user_gravity(id,0.6)
				g_iFrags[id]-=3
			}
		} case 2: {
			if(iFrags>=6) {
				new Float:speed = get_user_maxspeed(id) + 400.0
				set_user_maxspeed(id,speed)
				g_iFrags[id]-=6
			}
		} case 3: {
			if(iFrags>=5) {
				set_user_rendering(id,kRenderFxNone,50,50,50,kRenderTransAdd,50)
				set_user_footsteps(id,1)
				g_iFrags[id]-=5
			}
		} case 4: {
			if(iFrags>=5) {
				set_user_health(id,get_user_health(id) + 50)
				g_iFrags[id]-=5
			}
		} default: {
			return PLUGIN_HANDLED
		}
	}

	if(iFrags==g_iFrags[id]) {
		client_print(id,print_chat,"[A_S]: You don't have enough frags.")
		return PLUGIN_HANDLED
	}

	g_iCurrentAward[id]=key
	set_user_frags(id,g_iFrags[id])
	cmdUpdateScoreBoard(id)
	client_print(id,print_chat,"[A_S]: Award Bought")

	menu_destroy(menu)
	return PLUGIN_HANDLED
}

public speedb(id) {
	if(g_iCurrentAward[id] == 2) {
		new Float:speed = get_user_maxspeed(id) + 400.0
		set_user_maxspeed(id,speed)
	}
}

public preThink(id) {
	if(g_iLastTerr==id)
		return
		
	if(!iAlive[id])
		return
		
	set_pev(id, pev_solid, SOLID_SLIDEBOX)
}

public postThink(id) {
	if(g_iLastTerr==id)
		return

	if(!iAlive[id])
		return
		
	if(!is_wall_between_points(id,g_iLastTerr)) {
		set_pev(id, pev_solid, SOLID_NOT)
	}
}

public Ham_Semiclip_Touched(entity, id) {
	if(pev_valid(entity) && iAlive[id]) {
		new Float:flGametime = get_gametime()
		if(playerDelay[id] > flGametime)
			return
			
		playerDelay[id] = flGametime + 1.0
		
		new Float:flOrigin[3]
		pev(id, pev_origin, flOrigin)
		
		if(!is_hull_vacant(flOrigin, pev(id, pev_flags) & FL_DUCKING ? HULL_HEAD : HULL_HUMAN, id)) {
			ExecuteHamB(Ham_TakeDamage, id, entity, entity, 100.0, DMG_CRUSH)
		}
	}
}

stock bool:is_hull_vacant(const Float:origin[3], hull,id) {
	static tr
	engfunc(EngFunc_TraceHull, origin, origin, 0, hull, id, tr)
	if (!get_tr2(tr, TR_StartSolid) || !get_tr2(tr, TR_AllSolid)) //get_tr2(tr, TR_InOpen))
		return true
	
	return false
}

stock is_wall_between_points(id, entity)
{
	if(!iAlive[entity])
		return 0
	
	new ptr = create_tr2()
 
	new Float:start[3], Float:end[3], Float:endpos[3]
	pev(id, pev_origin, start)
	pev(entity, pev_origin, end)
	
	engfunc(EngFunc_TraceLine, start, end, IGNORE_MONSTERS, id, ptr)
 
	get_tr2(ptr, TR_vecEndPos, endpos)
	
	free_tr2(ptr)
	
	return xs_vec_equal(end, endpos)
}
