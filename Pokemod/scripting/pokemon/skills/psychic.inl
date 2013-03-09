
 public Agility(id, skill, prop)
 {
	PlayerStat[id][STAT_SPEED] += SkillClamp(id, skill, AGILITY_SPEED_MIN, AGILITY_SPEED_MAX );
	return SKILL_USED;
 }
 public Confusion(id, skill, prop)
 {
	get_user_origin(id, origin[id])

	new aid, players[MAX_PLAYERS], pnum, applecount = 0
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	new maxdist = CONFUSION_DISTANCE, distance
	AddLvl(id, maxdist)

	for(new i = 0; i<pnum; i++)
	{
		aid = players[i]
		if( Pokemod_FF(id, aid) ) continue;

		get_user_origin(aid, origin[aid])

		distance = get_distance(origin[aid], origin[id])

		if(distance <= maxdist){
			pev(aid, pev_origin, confuse[aid])
			set_task(1.0,"Confuse",aid)
			set_task(3.0,"Confuse",aid)

			applecount++
		}
	}
	if( ActionPrint(id, print_center, applecount, "will confuse") ){
		screenflash(id, 0, 0, 255, 50)
		sound_cast(id)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Amnesia(id, skill, prop)
 {
	get_user_origin(id, origin[id])

	new aid, players[MAX_PLAYERS], pnum, applecount = 0
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	new maxdist = CONFUSION_DISTANCE, distance
	AddLvl(id, maxdist)

	for(new i = 0; i<pnum; i++)
	{
		aid = players[i]
		if( Pokemod_FF(id, aid) ) continue;

		get_user_origin(aid, origin[aid])

		distance = get_distance(origin[aid], origin[id])

		if(distance <= maxdist){
			pev(aid, pev_origin, confuse[aid])
			set_task(1.0,"Confuse",aid)
			set_task(3.0,"Confuse",aid)

			applecount++

			set_task(5.0,"Confuse",aid)
		}
	}
	if( ActionPrint(id, print_center, applecount, "will confuse") ){
		screenflash(id, 0, 0, 255, 50)
		sound_cast(id)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Teleport(id, skill, prop)
 {
	if( PassTest(id, 1) ){
		get_user_origin(id, origin[id])
		get_user_origin(id, aim[id], 3)

		//Thanks to WC3 mod
		// Change coordinates to make sure player won't get stuck in the ground/wall
		if(-50>aim[id][0]-origin[id][0]>50)			//only change it if they are at least that far away
			aim[id][0] += ( ( aim[id][0] - origin[id][0] > 0 ) ? -50 : 50 );
		if(-50>aim[id][1]-origin[id][1]>50)			//only change it if they are at least that far away
			aim[id][1] += ( ( aim[id][1] - origin[id][1] > 0 ) ? -50 : 50 );
		if(-50>aim[id][2]-origin[id][2]>50)			//only change it if they are at least that far away
			aim[id][2] += ( ( aim[id][2] - origin[id][2] > 0 ) ? -50 : 50 );
		else
			aim[id][2] += 40;

		set_bind(id, WAIT_TIME)
		sound_cast(id)

		screenflash(id, 255, _, 255, 100, 100.0, 5)

		new parm[2];
		parm[0] = id;
		parm[1] = skill;
		set_task( TELEPORT_TIME , "finish_teleport", 0, parm, 2)
	}
	else{
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public finish_teleport(parm[])
 {
	new id = parm[0], skill = parm[1];
	if(PassTest(id, 0) && Pokemod_Alive(id)){
		bind_used(id,skill)

		get_user_origin(id, origin[id])
		set_user_origin(id, aim[id])

		MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_TRAIL],0,0,10,15,0,255,255,255,255,0)

		screenflash(id, 0, 0, 0, 0)
		sound_cast(id)

		poke_UnStuck(id)
	}
 }
 public Psybeam(id, skill, prop)
 {
	if( pev_valid(PlayerEnt[id]) ){
		sound_deny(id)
		g_StopSkill[id] = true;
		return SKILL_READY;
	}

	new Float:fl_Origin[3], Float:fl_Angles[3], Float:fl_vAngle[3]
	new Float:VecMins[3] = {-2.0,-2.0,-2.0}
	new Float:VecMaxs[3] = {2.0,2.0,2.0}

	// Get users postion and angles (angles are probably not needed in this case)
	pev(id, pev_origin, fl_Origin)
	pev(id, pev_angles, fl_Angles)
	pev(id, pev_v_angle, fl_vAngle)

	new newEnt = fm_create_entity("info_target")
	if( !newEnt )
		return SKILL_READY;

	PlayerEnt[id] = newEnt

	set_pev(newEnt, pev_classname, "psy_beam")
	fm_entity_set_model(newEnt, MODELS[MDL_SKULL])

	// Set entity size
	set_pev(newEnt, pev_mins, VecMins)
	set_pev(newEnt, pev_maxs, VecMaxs)

	// Change height of entity origin to hands
	fl_Origin[2] += 6

	// Set entity postion and angles
	fm_entity_set_origin(newEnt, fl_Origin)
	set_pev( newEnt, pev_angles, fl_Angles )
	set_pev( newEnt, pev_v_angle, fl_vAngle )

	// Set properties of the entity
	set_pev( newEnt, pev_solid, 2 )
	set_pev( newEnt, pev_movetype, 5 )
	set_pev( newEnt, pev_rendermode, 5 )
	set_pev( newEnt, pev_renderamt, 255.0 )
	set_pev( newEnt, pev_owner, id )
	set_pev( newEnt, pev_iuser2, skill )

	new Float:Velocity[3]
	new Speed = PSYBEAM_SPEED

	velocity_by_aim(id, Speed, Velocity)
	set_pev(newEnt, pev_velocity, Velocity)

	// Pass varibles used to guide entity with
	new args[3]
	args[0] = id
	args[1] = newEnt
	args[2] = Speed

	set_task(0.1, "guide_beam", newEnt, args, 3)
	fm_attach_view(id, newEnt)

	// Trail on enity. It's flawed by not being removable, so make it last long.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(22)		// TE_BEAMFOLLOW
	write_short(newEnt)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_TRAIL])	// sprite index
	write_byte(50)	// life in 0.1's
	write_byte(7)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	sound_cast(id)
	set_bind(id, WAIT_TIME)
	screenflash(id, 255, 0, 255, 100)
	return SKILL_READY;
 }
 public guide_beam(args[])
 {
	new id = args[0]
	new entID = args[1]
	new speed = args[2]

	if ( !pev_valid(entID) ) return

	if ( !Pokemod_Connected(id) || !Pokemod_Alive(id) ) {
		touches(entID, 0)
		return
	}

	new Float:Velocity[3], Float:Angles[3]
	velocity_by_aim(id, speed, Velocity)
	pev(id, pev_v_angle, Angles)
	set_pev(entID, pev_velocity, Velocity)
	set_pev(entID, pev_angles, Angles)

	set_task(0.1, "guide_beam", entID, args, 3)
 }
 public Barrier(id, skill, prop)
 {
	new in_area = 0, newEnt = -1
	new Float:fl_Origin[3]
	pev(id, pev_origin, fl_Origin)

	#if BARRIER_LIMIT > 0
		new gname[25]
		while((newEnt = fm_find_ent_in_sphere(newEnt, fl_Origin, 500.0)) != 0){
			pev(newEnt, pev_globalname, gname, 24)
			if(equali(gname,"mime_wall"))
				in_area++
		}
		if(in_area > BARRIER_LIMIT){
			poke_print(id, print_center, "Too many barriers nearby")
			return SKILL_READY;
		}
		newEnt = -1
		in_area = 0
	#endif

	while((newEnt = fm_find_ent_by_class(newEnt, "pokemod_ent")) != 0){
		in_area++
	}
	if(in_area > MAX_POKE_ENTS){
		poke_print(id, print_center, "Too many entities on map")
		return SKILL_READY;
	}

	sound_cast(id)

	new Float:velocity[3]
	new Float:VecMins[3] = {-1.0,-1.0,-30.0}
	new Float:VecMaxs[3] = {1.0,1.0,30.0}

	//lets move it a little away from them
	velocity_by_aim(id, 50, velocity)

	new Float:wall_hp = f_SkillClamp( id, skill, BARRIER_MIN_HP, BARRIER_MAX_HP );

	new Float:final_origin[3], temp_num;
	pev(id, pev_origin, fl_Origin)
	fl_Origin[0] += velocity[0]
	fl_Origin[1] += velocity[1]
	fl_Origin[2] += velocity[2]

	for( new i=2; i<8; i++ ){
		newEnt = fm_create_entity("info_target")

		if(!pev_valid(newEnt))
			continue;

		set_pev(newEnt, pev_classname, "pokemod_ent");
		set_pev(newEnt, pev_globalname, "mime_wall");
		fm_entity_set_model(newEnt, MODELS[MDL_HAIR]);

		set_pev(newEnt, pev_mins, VecMins);
		set_pev(newEnt, pev_maxs, VecMaxs);

		temp_num = i/2
		velocity_by_aim(id, temp_num*5, velocity);

		if( is_num_odd( i ) ){
			final_origin[0] = fl_Origin[0] + velocity[1];
			final_origin[1] = fl_Origin[1] - velocity[0];
		}
		else{
			final_origin[0] = fl_Origin[0] - velocity[1];
			final_origin[1] = fl_Origin[1] + velocity[0];
		}
		final_origin[2] = fl_Origin[2];

		fm_entity_set_origin(newEnt, final_origin);

		pev(id, pev_v_angle, velocity);
		set_pev(newEnt, pev_angles, velocity);

		set_pev(newEnt, pev_solid, SOLID_BBOX);
		set_pev(newEnt, pev_movetype, MOVETYPE_FLY);

		set_pev(newEnt, pev_takedamage, 1.0);
		set_pev(newEnt, pev_health, wall_hp);
	}

	return SKILL_USED;
 }
 public Hypnosis(id, skill, prop)
 {
	new body, aid
	get_user_aiming(id, aid, body, HYPNOSIS_MAX_DISTANCE)

	if( !ValidPlayer(aid) || !Pokemod_Alive(aid))
		return SKILL_READY;

	if(!g_Status[aid][STATUS_ASLEEP]){
		if( Pokemod_FF(id, aid) ) return SKILL_READY;

		sound_cast(id)
		ApplyStatus( id, aid, STATUS_ASLEEP, get_ppcvar_float(pm_sleeptime) )

		poke_print(id,print_center,"They are now asleep")
		poke_print(aid,print_center,"You have been put to sleep")
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Dream_Eater(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);

	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) || !g_Status[victim][STATUS_ASLEEP] )
		return SKILL_READY;

	sound_cure(id)

	new eatdamage = DREAM_EAT_DAMAGE
	AddLvl(id, eatdamage)

	Pokemod_Heal( id, skilldamage( victim, id, eatdamage, skill) )

	poke_print(id,print_center,"You ate a dream")
	poke_print(victim,print_center,"Your dream has been eaten")

	return SKILL_USED;
 }
 public Rest(id, skill, prop)
 {
	new rest = REST
	screenflash(id, 0, 0, 0, 255-SkillPercent(id, skill, 222))
	set_pev(id, pev_velocity, Float:{0.0,0.0,0.0});
	Pokemod_Heal(id, rest)
	return SKILL_USED;
 }
 public Light_Screen(id, skill, prop)
 {
	new damage;
	PokeSkillTarget(id, damage);
	Pokemod_Heal(id, damage/2);
	return SKILL_USED;
 }
 public Psychic(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new damage = (level*PSYCHIC_DAMAGE)/(TotalLevels+1);

	new players[MAX_PLAYERS], pnum, tid;
	poke_get_players(players, pnum, PLAYER_ALIVE);

	sound_cast(id);
	g_StopSkill[id] = true;

	for(new i=0;i<pnum;i++){
		tid = players[i]
		if( Pokemod_FF(id, tid) || fm_entity_range(id,tid) > (PSYCHIC_RANGE+level*5) )
			continue

		skilldamage(tid, id, damage, skill)
		bind_used(tid, _, PSYCHIC_DELAY)

		get_user_origin(tid, origin[tid])

		message_begin( MSG_PAS, SVC_TEMPENTITY, origin[tid] )
		write_byte(TE_BEAMDISK)
		write_coord( origin[tid][0])
		write_coord( origin[tid][1])
		write_coord( origin[tid][2])
		write_coord( origin[tid][0])
		write_coord( origin[tid][1])
		write_coord( origin[tid][2] + 10 + level)
		write_short( SPRITE_INDEX[SPR_SHOCKWAVE] )
		write_byte( 0 )		// startframe
		write_byte( 0 )		// framerate
		write_byte( 10 )		// life
		write_byte( 16 )	// width
		write_byte( 0 )		// noise
		write_byte( 188 )	//r
		write_byte( 20 )	//g
		write_byte( 188 )	//b
		write_byte( 255 )	//brightness
		write_byte( 0 )		// speed
		message_end()
	}

	if( end_rep_use(id, skill, 0.5) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Psywave(id, skill, prop)
 {
	new damage, victim = PokeSkillTarget(id, damage);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) ){
		new percent_damage = SkillPercent(id, skill, damage);
		skilldamage(victim, id, percent_damage, skill);

		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ARBOK],1,1,5,40,99,150,0,150,100,0);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Future_Sight(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new damage = (level*FUTURE_SIGHT_DAMAGE)/(TotalLevels+1);

	new players[MAX_PLAYERS], pnum, tid;
	poke_get_players(players, pnum, PLAYER_ALIVE);

	g_StopSkill[id] = true;

	for(new i=0;i<pnum;i++){
		tid = players[i]
		if( Pokemod_FF(id, tid) || fm_entity_range(id,tid) > (PSYCHIC_RANGE+level*5) )
			continue

		skilldamage(tid, id, damage, skill)
		bind_used(tid, _, PSYCHIC_DELAY)

		get_user_origin(tid, origin[tid])

		message_begin( MSG_PAS, SVC_TEMPENTITY, origin[tid] )
		write_byte(TE_BEAMDISK)
		write_coord( origin[tid][0])
		write_coord( origin[tid][1])
		write_coord( origin[tid][2])
		write_coord( origin[tid][0])
		write_coord( origin[tid][1])
		write_coord( origin[tid][2] + 10 + level)
		write_short( SPRITE_INDEX[SPR_SHOCKWAVE] )
		write_byte( 0 )		// startframe
		write_byte( 0 )		// framerate
		write_byte( 10 )		// life
		write_byte( 16 )	// width
		write_byte( 0 )		// noise
		write_byte( 188 )	//r
		write_byte( 20 )	//g
		write_byte( 188 )	//b
		write_byte( 255 )	//brightness
		write_byte( 0 )		// speed
		message_end()
	}

	if( end_rep_use(id, skill, 0.5) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Hidden_Power(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id])
	get_user_origin(id, aim[id], 2)
	IVecFVec(origin[id], f_origin)
	IVecFVec(aim[id], f_aimvec)

	skillLinedamage(id, f_origin, f_aimvec, level/6, level, skill, _, STATUS_ASLEEP, -1)
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_TRAIL],1,1,1,level,0,255,255,255,255,0)
	poke_sound(id,CHAN_VOICE, SND_AG_DIE)

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
