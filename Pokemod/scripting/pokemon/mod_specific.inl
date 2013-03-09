
// Here are things that don't have equivalents in other mods, ie Rescueing Hostages, Changing Classes, etc.

 #if MOD==CS
 public pokemon_money(id)
	money[id] = read_data(1)

 public message_Health(msgid, dest, id)
 {
	if( Pokemod_Alive(id) && get_msg_arg_int(1) > 255)
		set_msg_arg_int(1, ARG_BYTE, 255);
	return PLUGIN_CONTINUE;
 }
 #endif

 #if MOD==DOD
 #endif

 #if MOD==NS
 public client_built(id,sid,type,impulse)
 {
	new parm[4]
	parm[0] = id
	parm[1] = sid
	parm[2] = type
	parm[3] = impulse
	set_task(1.0, "check_ns_build",0, parm, 4)
 }
 public check_ns_build(parm[])
 {
	new id = parm[0]
	new sid = parm[1]
	new type = parm[2]
	new impulse = parm[3]

	if(!pev_valid(sid))
		return

	new classname[32]
	pev(sid, pev_classname, classname, 31)

	debugMessage(id, 7, "Mod", "created sid:%d class:%s type:%d impulse:%d",sid,classname, type, impulse)

	if(get_ppcvar_num(pm_pokecenter)>0 && 
		(
			(type==1 && equali(classname,"resourcetower")) || 
			(type==2 && equali(classname,"alienresourcetower"))
		)
	){
		new Float:temporigin[3], Float:angles[3]
		pev(sid, pev_origin, temporigin)
//		temporigin[2] += 10.0	//just put it up a lil
		pev(sid, pev_angles, angles)

		new oak_num = find_free_oak()
		if(oak_num==-1)
			return
		PCent[oak_num] = fm_create_entity("info_target")
		new oak_props[MAX_OAKPROP]
		get_ppcvar_string( pm_oak, oak_props, MAX_OAKPROP-1)
		create_oak(PCent[oak_num], temporigin, angles, type, oak_props)

		set_pev(PCent[oak_num],pev_fuser1,1000.0)
		set_pev(PCent[oak_num],pev_fuser2,1000.0)
		set_pev(PCent[oak_num],pev_team,type)		// Use pev(id, pev_team) to get the team and not get_team(id) so MvM works.
		ns_set_struct_owner(PCent[oak_num], id)		//this make the id as owner of structur

		poke_UnStuck(PCent[oak_num], false)

//		UnSolid(sid)
//		entity_set_int(sid, EV_INT_rendermode, 5)
//		entity_set_float(sid, EV_FL_renderamt, 255.0)
	}
 }
 public client_changeclass(id,newclass,oldclass)
 {
	//update their new maxhp
	switch(newclass)
	{
		//things that we don't want to check maxhp for
		case CLASS_NOTEAM, CLASS_DEAD, CLASS_COMMANDER, CLASS_UNKNOWN: return
		default:	set_task(0.1,"CheckMaxHP",id)
	}
 }
 #endif


