/*
installation:
	Use the map specific plugin method, instructions shown in link below
	http://wiki.amxmodx.org/Configuring_AMX_Mod_X#Map_Specific_Plugins

cvars:
	"amx_sib2_dmt #"  // Toggles deathmatch -- 0 off -- 1 on
	"amx_sib2_wrc #"  // Amount of seconds weapon reset will occur,deathmatch must be activated
	"amx_sib2_prd #"  // Amount of seconds player respawn delays, deathmatch must be activated

commands:
	"say /respawn"  // This respawns the player under specific conditions, deathmatch must be activated
*/

#include <amxmodx>
#include <cstrike>
#include <hamsandwich>
#include <engine>
#include <fun>

#define VERSION "1.0.1"

#define OFFSET_SLOT_PRIMARY 368
#define OFFSET_SLOT_SECONDARY 369
#define OFFSET_SLOT_GRENADE 371

new const g_szJailTeleportModels[][] = {
	"*10",
	"*11"
}

new g_szThinkerClassname[]="sib2_thinker"

new bool:g_bPlayerQueued[33]

new g_pCvarWeaponResetCycle,
	g_pCvarDeathMatchToggle,
	g_pCvarPlayerRespawnDelay

new g_iTotalWeaponsRemoved,
	g_iTotalWeaponsRespawned

new g_iNewRoundEntity,
	g_iRemoveWeaponEntity,
	g_iRespawnWeaponEntity

public plugin_init() {
	register_plugin("surf_icebob2",VERSION,"Firippu")

	g_pCvarDeathMatchToggle = register_cvar("amx_sib2_dmt","0")
	g_pCvarWeaponResetCycle = register_cvar("amx_sib2_wrc","180")
	g_pCvarPlayerRespawnDelay = register_cvar("amx_sib2_prd","3")

	register_clcmd("say /respawn","cmdRespawn")

	register_logevent("eNewRound",2,"1=Round_Start")

	register_touch("trigger_teleport","player","fwdPlayerTouchedTeleport")
	register_touch("cycler_sprite","player","fwdPlayerTouchedCycler")

	RegisterHam(Ham_Killed,"player","fwdPlayerKilled",1)

	new iEntity

	if((iEntity = create_entity("info_target"))) {
		entity_set_string(iEntity,EV_SZ_classname,g_szThinkerClassname)
		entity_set_float(iEntity,EV_FL_nextthink,get_gametime() + get_pcvar_float(g_pCvarWeaponResetCycle))
		register_think(g_szThinkerClassname,"ThinkerThought")
	}

	if((iEntity = create_entity("info_target"))) {
		entity_set_string(iEntity,EV_SZ_classname,"trigger_teleport")
		entity_set_string(iEntity,EV_SZ_target,"up")
		entity_set_int(iEntity,EV_INT_solid,SOLID_TRIGGER)
		entity_set_size(iEntity,Float:{832.0,-3296.0,-3553.0},Float:{2752.0,-2848.0,-3553.0})
	}

	for(new i=0; i<sizeof g_szJailTeleportModels; i++) {
		if((iEntity = find_ent_by_model(-1,"trigger_teleport",g_szJailTeleportModels[i]))) {
			static Float:vOrigin[3]

			entity_get_vector(iEntity,EV_VEC_origin,vOrigin)

			vOrigin[2] += 31.0

			entity_set_origin(iEntity,vOrigin)
		}
	}

	if((iEntity = find_ent_by_class(-1,"button_target"))) {
		entity_set_string(iEntity,EV_SZ_target,"jaildoors")
	}
}

public client_connect(iPlayer) {
	g_bPlayerQueued[iPlayer] = false
}

public eNewRound() {
	while((g_iNewRoundEntity = find_ent_by_class(g_iNewRoundEntity,"cycler_sprite")) != 0) {
		static szModel[21]

		entity_get_string(g_iNewRoundEntity,EV_SZ_model,szModel,20)

		if(equal(szModel,"models/props/atm.mdl")) {
			entity_set_size(g_iNewRoundEntity,Float:{-0.1,-0.1,-0.1},Float:{0.1,0.1,0.1})
			set_rendering(g_iNewRoundEntity,kRenderFxNone,0,0,0,kRenderTransTexture,0)
		} else {
			entity_set_int(g_iNewRoundEntity,EV_INT_movetype,MOVETYPE_FLY)
			entity_set_vector(g_iNewRoundEntity,EV_VEC_maxs,Float:{5.0,5.0,1.0})
			set_rendering(g_iNewRoundEntity,kRenderFxNone,0,0,0,kRenderTransTexture,255)
		}
	}
}

public fwdPlayerTouchedTeleport(iEntity,iPlayer) {
	static szTarget[10],Float:vOrigin[3]

	entity_get_string(iEntity,EV_SZ_target,szTarget,9)
	entity_get_vector(iPlayer,EV_VEC_origin,vOrigin)

	if(equal(szTarget,"jjj")) {
		set_user_velocity(iPlayer,Float:{0.0,0.0,0.0})

		switch(random_num(1,2)) {
			case 1: {
				vOrigin[0] = 1770.0
			} case 2: {
				vOrigin[0] = 1810.0
			}
		}

		vOrigin[1] = -3585.0
		vOrigin[2] =  -150.0
	} else if(equal(szTarget,"moveback")) {
		vOrigin[1] = -3950.0
		vOrigin[2] += 1760.0
	} else if(equal(szTarget,"moveback2")) {
		vOrigin[1] = -3808.0
		vOrigin[2] += 2688.0
	} else if(equal(szTarget,"up")) {
		vOrigin[2] += 2048.0
	}

	entity_set_origin(iPlayer,vOrigin)
}

public fwdPlayerTouchedCycler(iEntity,iPlayer) {
	static szTargetname[8],szModel[21]

	entity_get_string(iEntity,EV_SZ_targetname,szTargetname,7)
	entity_get_string(iEntity,EV_SZ_model,szModel,20)

	if(equal(szModel,"models/props/atm.mdl")) {
		static Float:vOrigin[3]

		entity_get_vector(iPlayer,EV_VEC_origin,vOrigin)

		if(equal(szTargetname,"part2")) {
			vOrigin[0] = random_float(-1392.0,-912.0)
			vOrigin[1] = random_float(-3640.0,-3360.0)
			vOrigin[2] += -2912.0
		} else if(equal(szTargetname,"done")) {
			vOrigin[0] = random_float(-180.0,130.0)
			vOrigin[1] = random_float(-3770.0,-3380.0)
			vOrigin[2] = 2084.0
		} else if(equal(szTargetname,"top")) {
			vOrigin[0] = random_float(1590.0,1990.0)
			vOrigin[1] = random_float(-2180.0,-2000.0)
			vOrigin[2] = -251.0
		} else if(equal(szTargetname,"l")) {
			vOrigin[0] = random_float(-220.0,220.0)
			vOrigin[1] = random_float(2720.0,2920.0)
			vOrigin[2] = 2196.0
		} else if(equal(szTargetname,"r")) {
			vOrigin[0] = random_float(3360.0,3800.0)
			vOrigin[1] = random_float(2720.0,2920.0)
			vOrigin[2] = 2196.0
		} else if(equal(szTargetname,"ct")) {
			vOrigin[0] = random_float(820.0,1210.0)
			vOrigin[1] = random_float(-3270.0,-2900.0)
			vOrigin[2] = -1467.0
		} else if(equal(szTargetname,"t")) {
			vOrigin[0] = random_float(2370.0,2750.0)
			vOrigin[1] = random_float(-3270.0,-2900.0)
			vOrigin[2] = -1467.0
		}

		entity_set_origin(iPlayer,vOrigin)

		return HAM_SUPERCEDE
	}

	if(get_pdata_cbase(iPlayer,OFFSET_SLOT_PRIMARY)<0) {
		if(equal(szTargetname,"scout")) {
			GivePlayerWeapon(iPlayer,"weapon_scout",iEntity)
		} else if(equal(szTargetname,"m4a1")) {
			GivePlayerWeapon(iPlayer,"weapon_m4a1",iEntity)
		} else if(equal(szTargetname,"ak47")) {
			GivePlayerWeapon(iPlayer,"weapon_ak47",iEntity)
		} else if(equal(szTargetname,"m249")) {
			GivePlayerWeapon(iPlayer,"weapon_m249",iEntity)
		} else if(equal(szTargetname,"shotgun")) {
			GivePlayerWeapon(iPlayer,"weapon_m3",iEntity)
		} else if(equal(szTargetname,"mp5")) {
			GivePlayerWeapon(iPlayer,"weapon_mp5navy",iEntity)
		} else if(equal(szTargetname,"awp")) {
			GivePlayerWeapon(iPlayer,"weapon_awp",iEntity)
		}
	}

	if(get_pdata_cbase(iPlayer,OFFSET_SLOT_SECONDARY)<0) {
		if(equal(szTargetname,"deagle")) {
			GivePlayerWeapon(iPlayer,"weapon_deagle",iEntity)
		}
	}

	if(get_pdata_cbase(iPlayer,OFFSET_SLOT_GRENADE)<0) {
		if(equal(szTargetname,"grenade")) {
			GivePlayerWeapon(iPlayer,"weapon_hegrenade",iEntity)
		}
	}

	return HAM_IGNORED
}

public cmdRespawn(iPlayer) {
	fwdPlayerKilled(iPlayer)
}

public bool:bPlayerInTeam(iPlayer) {
	switch(cs_get_user_team(iPlayer)) {
		case 1..2: {
			return true
		}
	}

	return false
}

public fwdPlayerKilled(iPlayer) {
	if(get_pcvar_num(g_pCvarDeathMatchToggle)) {
		if(!is_user_alive(iPlayer) && !g_bPlayerQueued[iPlayer] && bPlayerInTeam(iPlayer)) {
			g_bPlayerQueued[iPlayer] = true

			client_print(iPlayer,print_chat,"You will respawn in %d seconds.",get_pcvar_num(g_pCvarPlayerRespawnDelay))

			set_task(get_pcvar_float(g_pCvarPlayerRespawnDelay),"RespawnPlayer",iPlayer)
		} else {
			client_print(iPlayer,print_chat,"You do not meet the requirements to use respawn.")
		}
	}
}

public RespawnPlayer(iPlayer) {
	if(!is_user_alive(iPlayer) && g_bPlayerQueued[iPlayer] && bPlayerInTeam(iPlayer)) {
		ExecuteHam(Ham_CS_RoundRespawn,iPlayer)
	}

	g_bPlayerQueued[iPlayer] = false
}

GivePlayerWeapon(iPlayer,szWeapon[],iEntity) {
	set_rendering(iEntity,kRenderFxNone,0,0,0,kRenderTransTexture,0)
	entity_set_int(iEntity,EV_INT_movetype,MOVETYPE_NOCLIP)

	for(new i=0; i<10; i++) {
		give_item(iPlayer,szWeapon)
	}

	return HAM_IGNORED
}

public ThinkerThought(iEntity) {
	if(get_pcvar_num(g_pCvarDeathMatchToggle)) {
		RemoveWeapons()
		RespawnWeapons()

		if(g_iTotalWeaponsRemoved && g_iTotalWeaponsRespawned) {
			client_print(0,print_chat,"[Weapons Reset] Removed: %d, Respawned: %d",g_iTotalWeaponsRemoved,g_iTotalWeaponsRespawned)
		} else if(g_iTotalWeaponsRemoved && !g_iTotalWeaponsRespawned) {
			client_print(0,print_chat,"[Weapons Reset] Removed: %d",g_iTotalWeaponsRemoved)
		} else if(!g_iTotalWeaponsRemoved && g_iTotalWeaponsRespawned) {
			client_print(0,print_chat,"[Weapons Reset] Respawned: %d",g_iTotalWeaponsRespawned)
		}

		g_iTotalWeaponsRemoved = 0
		g_iTotalWeaponsRespawned = 0
	}

	entity_set_float(iEntity,EV_FL_nextthink,get_gametime() + get_pcvar_float(g_pCvarWeaponResetCycle))
}

public RemoveWeapons() {
	while((g_iRemoveWeaponEntity = find_ent_by_class(g_iRemoveWeaponEntity,"weaponbox")) != 0) {
		call_think(g_iRemoveWeaponEntity)

		g_iTotalWeaponsRemoved++
	}
}

public RespawnWeapons() {
	while((g_iRespawnWeaponEntity = find_ent_by_class(g_iRespawnWeaponEntity,"cycler_sprite")) != 0) {
		static szModel[21]

		entity_get_string(g_iRespawnWeaponEntity,EV_SZ_model,szModel,20)

		if(!equal(szModel,"models/props/atm.mdl") && entity_get_float(g_iRespawnWeaponEntity,EV_FL_renderamt) != 255) {
			entity_set_int(g_iRespawnWeaponEntity,EV_INT_movetype,MOVETYPE_FLY)
			set_rendering(g_iRespawnWeaponEntity,kRenderFxNone,0,0,0,kRenderTransTexture,255)

			g_iTotalWeaponsRespawned++
		}
	}
}

public plugin_precache() {
	precache_model("models/props/atm.mdl")
	precache_generic("czcs_office.wad")
}
