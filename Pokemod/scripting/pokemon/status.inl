
 /////////////////////////////
 //  Sets a players status  //
 /////////////////////////////
 SetStatus(id, PM_STATUS:status, amount=1, bool:set=true)
 {
	if( !ValidPlayer(id) )
		return;
	if(set)
		g_Status[id][status] = amount
	else
		g_Status[id][status] += amount

	debugMessage(id,5,"Player", "was set with status %d, amount %d, set %d",_:status, amount, set);

	#if AUTO_ITEMS > 0
	if( g_Status[id][status] > 0 && PokeItem(id) ){
		#if AUTO_ITEMS==1
		if(is_user_bot(id)){
		#endif
			for( new i = 0 ; i < ItemsCreated ; i++ ){

				if( !HasPokeItem(id,i) || ItemInfo[i] & II_DISABLE )
					continue

				if(		 ItemInfo[i] & II_POISON && status == STATUS_POISON)
					UseItem(id, i)
				else if( ItemInfo[i] & II_ASLEEP && status == STATUS_ASLEEP)
					UseItem(id, i)
				else if( ItemInfo[i] & II_BURN && status == STATUS_BURN)
					UseItem(id, i)
				else if( ItemInfo[i] & II_FROZEN && status == STATUS_FROZEN)
					UseItem(id, i)
				else if( ItemInfo[i] & II_PARALYZE && status == STATUS_PARALYZE)
					UseItem(id, i)
				else if( ItemInfo[i] & II_REMEDY && HasStatus(id))
					UseItem(id, i)
			}
		#if AUTO_ITEMS==1
		}
		#endif
	}
	#endif

	if( !set || amount )
		skills_handler(id, SI_STATUS);
 }
 AddStatus(id, PM_STATUS:status, amount=1)
	SetStatus(id, status, amount, false)
 RemoveStatus(id, PM_STATUS:status)
	SetStatus(id, status, 0)

 ///////////////////////////////////////////////////
 //  Things to do if they are in Water/PokeWater  //
 ///////////////////////////////////////////////////
 InWater(id)
 {
	if( PokeType[Pokemon[id]] & TYPE_WATER ){
		Pokemod_Heal(id,get_ppcvar_num(pm_waterheal))
	}
	if(g_Status[id][STATUS_BURN]){
		RemoveStatus(id, STATUS_BURN)
		stopFireSound(id)
	}
 }
 
 ///////////////////////////////
 //  Applies a status affect  //
 ///////////////////////////////
 ApplyStatus( effecter, id, PM_STATUS:status, Float:time )
 {
	if( !ValidPlayer(id) )
		return;

	switch(status)
	{
		case STATUS_ASLEEP:{
			g_StopSkill[id] = true;
			set_task( 0.1, "hsleep", id );
			set_task( time, "unsleep", id );
		}
		case STATUS_SLOW:{
			set_task( 0.1, "DelayedSetSpeed", id );
			set_task( time, "unslow", id );
		}
		case STATUS_CONFUSE:{
			pev( id, pev_origin, confuse[id] );
			set_task( 1.0, "Confuse", id );
		}
		case STATUS_PARALYZE:{
			set_task( 0.1, "DelayedSetSpeed", id );
			set_pev( id, pev_velocity, Float:{0.0,0.0,0.0} );
			set_task( time, "unparalyze", id );
		}
		case STATUS_POISON:{
			new parm[2];
			parm[0] = id;
			parm[1] = effecter;
			set_task( 0.1, "Poison", 0, parm, 2 );
		}
		case STATUS_BURN:{
			burn_victim( id, effecter );
		}
		case STATUS_FROZEN:{
			Freeze( id );
		}
	}
	switch(status)
	{
		case STATUS_POISON:{
			poke_print( effecter, print_center, LANG, effecter,"POKEMOD_STATPOISON")
			poke_print( id, print_center, LANG, id,"POKEMOD_STATPOISONED")
			SetStatus( id, STATUS_POISON, floatround(time) );
		}
		case STATUS_FROZEN:{
			AddStatus( id, STATUS_FROZEN );
		}
		default:{
			SetStatus( id, status );
		}
	}
 }

 //////////////////////////////
 //  When someone is asleep  //
 //////////////////////////////
 public hsleep(id)
 {
 	if( Pokemod_Alive(id) && g_Status[id][STATUS_ASLEEP]){
		screenflash(id, 0, 0, 0, 255)
		SetSpeed(id)
		set_task(0.1, "hsleep", id)
	}
 }
 
 ///////////////////////
 //  Awakens someone  //
 ///////////////////////
 public unsleep(id)
 {
	if(g_Status[id][STATUS_ASLEEP]){
		screenflash(id)
		RemoveStatus(id, STATUS_ASLEEP)
		SetSpeed(id)
		poke_print(id,print_center,LANG, id,"POKEMOD_STATWAKE")
	}
 }
 
 ///////////////////////////////
 //  Gets rid of Slow status  //
 ///////////////////////////////
 public unslow(id)
 {
	if(g_Status[id][STATUS_SLOW]){
		RemoveStatus(id, STATUS_SLOW)
		SetSpeed(id)
	}
 }
 
 ///////////////////////////////////
 //  Gets rid of Paralyze status  //
 ///////////////////////////////////
 public unparalyze(id)
 {
	if(g_Status[id][STATUS_PARALYZE]){
		RemoveStatus(id, STATUS_PARALYZE)
		SetSpeed(id)
	}
 }
 
 //////////////
 //  Poison  //
 //////////////
 public Poison(parm[])
 {
	new id = parm[0], inflictor = parm[1]

	if(g_Status[id][STATUS_POISON] > 0 && Pokemod_Alive(id)){

		AddStatus(id, STATUS_POISON, -1)

		if( !g_Status[id][STATUS_ASLEEP] )
			screenflash(id, 128, 0, 128, 100, 1.0)

		pokedamage( id, inflictor, get_ppcvar_num(pm_poisondamage), "Poison", TYPE_POISON)
		poke_sound( id, CHAN_AUTO, SND_PAIN)
		set_task(1.0, "Poison",0, parm, 2)
	}
	else
		RemoveStatus(id, STATUS_POISON)
 }

 ///////////////////////////
 //  Resets all Statuses  //
 ///////////////////////////
 ResetStatus(id, all=0)
 {
	if(g_Status[id][STATUS_BURN]){
		RemoveStatus(id, STATUS_BURN)
		stopFireSound(id)
	}

	for(new PM_STATUS:i=STATUS_BURN; i<MAX_STATUS; i++){
		//statuses we don't want to reset
		if(all==0 && (i==STATUS_BIKE || i==STATUS_OAK_STOP))
			continue
		SetStatus(id, i, 0)
	}
	screenflash(id,0,0,0,0)

	SetSpeed(id)
 }
 
 /////////////////////////////
 //  Light someone on fire  //
 /////////////////////////////
 public burn_victim(id, killer)
 {
	if( entity_in_water(id) || g_Status[id][STATUS_BURN] ) return
	if( !FFon() && Pokemod_FF(id, killer) ) return

	new name[32]
	get_user_name(killer, name, 31)
	poke_print(id,print_center,LANG, id,"POKEMOD_STATBURN", name)

	poke_sound(id, CHAN_ITEM, SND_BURN)

	new args[2]
	args[0] = id
	args[1] = killer
	new burn_seconds = get_ppcvar_num(pm_burntime)
	set_task(0.5, "on_fire", 0, args, 2, "a", burn_seconds*2)
	set_task(burn_seconds+0.5, "stopFireSound", id)
 }
 
 ///////////////////
 //  Fire effects //
 ///////////////////
 public on_fire(args[])
 {
	new id = args[0]
	new killer = args[1]

	if( !Pokemod_Connected(id) || !Pokemod_Alive(id) || entity_in_water(id) )
		RemoveStatus(id, STATUS_BURN)

	if ( !g_Status[id][STATUS_BURN] )
		return

	new burn_origin[3], Float:f_burn_origin[3];
	get_user_origin(id, burn_origin);
	IVecFVec(burn_origin, f_burn_origin);

	// Additive sprite, plays 1 cycle
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(17)				// TE_SPRITE
	write_coord(burn_origin[0])	// coord, coord, coord (position)
	write_coord(burn_origin[1])
	write_coord(burn_origin[2]+10)
	write_short(SPRITE_INDEX[SPR_XFIRE])	// short (sprite index)
	write_byte(30)		// byte (scale in 0.1's)
	write_byte(200)	// byte (brightness)
	message_end()

	// Smoke
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(5)					// TE_SMOKE
	write_coord(burn_origin[0])	// coord, coord, coord (position)
	write_coord(burn_origin[1])
	write_coord(burn_origin[2]+100)
	write_short(SPRITE_INDEX[SPR_STEAM1])		// short (sprite index)
	write_byte(60)		// byte (scale in 0.1's)
	write_byte(15)		// byte (framerate)
	message_end()

	//	doing area effect that way we can catch other people on fire
	AoEdamage(killer, f_burn_origin, 50, get_ppcvar_num(pm_burndamage), "Burn", AOE_VTEAM, TYPE_FIRE, _, STATUS_BURN, -1)
	AoEdamage(id, f_burn_origin, 50, get_ppcvar_num(pm_burndamage), "Burn", AOE_VTEAM, TYPE_FIRE, _, STATUS_BURN, -1)
 }
 
 ////////////////////////
 //  Stop the burning  //
 ////////////////////////
 public stopFireSound(id)
 {
	RemoveStatus(id, STATUS_BURN)
	emit_sound(id, CHAN_ITEM, SOUNDS[SND_BURN], 1.0, ATTN_NORM, (1<<5), PITCH_NORM)
 }
 
 //////////////////////
 //  Freeze someone  //
 //////////////////////
 Freeze(id)
 {
	SetSpeed(id);
	screenflash(id, 0, 50, 200, 180);
	poke_print(id, print_center, LANG, id,"POKEMOD_STATFROZ");
	if(Health(id) > 20.0)
		PokeEntDamage(id, "Frozen", 2, DMG_FREEZE);
 }
 
 ///////////////////////////////////
 //  If someone has any statuses  //
 ///////////////////////////////////
 HasStatus(id)
 {
	for(new PM_STATUS:i=STATUS_BURN; i<MAX_STATUS; i++){
		//statuses we don't want to know about
		if(i==STATUS_BIKE || i==STATUS_OAK_STOP)
			continue
		if(g_Status[id][i]>0)
			return g_Status[id][i]
	}
	return false
 }
 
 ///////////////////////
 //  Confuse someone  //
 ///////////////////////
 public Confuse(id)
 {
	if(!Pokemod_Alive(id))
		return
	new Float:tempconfuse[3]
	tempconfuse[0] = confuse[id][0]
	tempconfuse[1] = confuse[id][1]
	tempconfuse[2] = confuse[id][2]
	pev(id, pev_origin, confuse[id])
	fm_entity_set_origin(id, tempconfuse)
	poke_print(id, print_center, LANG, id,"POKEMOD_STATCONF" )
	if(!poke_UnStuck(id, false)){
		user_kill(id)
		poke_printlang(id,"POKEMOD_STATCONFD" )
	}
 }
 
 
 

