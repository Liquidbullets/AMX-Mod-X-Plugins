#include <amxmodx>
#include <engine>

new g_szTargetname[12]

public plugin_init() {
	register_plugin("deathrun_fixes1","0.0.1","Firippu")

	new szMapName[18]
	get_mapname(szMapName,17)

	if(equali(szMapName,"deathrun_baw")) {
		strcat(g_szTargetname,"brokenwheel",11)
		register_touch("player","func_door_rotating","fwdTouchedDoorRotating")
	} else if(equali(szMapName,"deathrun_extreme")) {
		strcat(g_szTargetname,"rummet",6)
		register_touch("player","func_door_rotating","fwdTouchedDoorRotating")
	} else if(equali(szMapName,"deathrun_cartoon")) {
		fnCreateTriggerHurt(Float:{-384.0,1296.0,-1344.0},Float:{32.0,1344.0,-1120.0})
	} else if(equali(szMapName,"deathrun_dojo")) {
		fnCreateTriggerHurt(Float:{1464.0,-1856.0,64.0},Float:{1520.0,-1176.0,344.0})
	} else if(equali(szMapName,"deathrun_temple")) {
		deathrun_temple()
	}
}

deathrun_temple() {
	new iEntity
	for(new i=0; i<2; i++) {
		iEntity=find_ent_by_tname(iEntity,"brolucka")
		static Float:vOrigin[3]
		entity_get_vector(iEntity,EV_VEC_origin,vOrigin)

		if(i == 0) {
			vOrigin[1] += 6.0
		} else {
			vOrigin[1] -= 6.0
		}

		entity_set_origin(iEntity,vOrigin)
	}
}

public fnCreateTriggerHurt(Float:Mins[3],Float:Maxs[3]) {
	new iEntity
	if((iEntity=create_entity("trigger_hurt"))) {
		DispatchKeyValue(iEntity,"dmg","200")
		DispatchSpawn(iEntity)
		entity_set_size(iEntity,Mins,Maxs)
	}
}

public fwdTouchedDoorRotating(iPlayer,iEntity) {
	static szTargetname[12]
	entity_get_string(iEntity,EV_SZ_targetname,szTargetname,11)
	if(equal(szTargetname,g_szTargetname)) {
		if(entity_get_int(iPlayer,EV_INT_button) & IN_DUCK) {
			client_cmd(iPlayer,"-duck")
		}
	}
}
