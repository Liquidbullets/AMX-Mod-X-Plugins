
 public Mist(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:mist_origin[3];
	pev(id, pev_origin, mist_origin)
	skillAoEdamage(id, mist_origin, level*10, clamp(-1*level/9,-100,-1), skill)

	FVecIVec( mist_origin, origin[id] );

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
	write_byte(TE_SPRITE)
	write_coord(origin[id][0]) // start position
	write_coord(origin[id][1])
	write_coord(origin[id][2])
	write_short(SPRITE_INDEX[SPR_GAS]) // sprite index
	write_byte(10) // scale in 0.1's
	write_byte(100) // brightness
	message_end()

	poke_sound(id, CHAN_VOICE, SND_WADE)

	ResetStatus(id)

	if( end_rep_use(id, skill, 2.5) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Blizzard(id, skill, prop)
 {
	if( prop & SI_RESET ){
		remove_task(BLIZZARD_TASK+id);
		return SKILL_READY;
	}

	sound_cast(id)
	set_bind(id, WAIT_TIME)

	new parm[3];
	parm[0] = id;
	parm[1] = skill;
	parm[2] = floatround(BLIZZARD_TIME*2+SkillLevel(id, skill)/10);

	set_task(0.5, "Do_Blizzard", id+BLIZZARD_TASK, parm, 3)
	return SKILL_READY;
 }
 public Do_Blizzard(parm[])
 {
	new id = parm[0];
	if( !Pokemod_Alive(id) )
		return

	parm[2]--;
	if( !parm[2] ){
		bind_used(parm[0], parm[1]);
		return;
	}

	new level = SkillLevel(id, parm[1])
	new players[MAX_PLAYERS], pnum, aid
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	for(new i=0; i<pnum; i++){
		aid = players[i]
		if( fm_entity_range(id,aid) > level*10 )
			continue

		make_snow(aid)
		if( !Pokemod_FF(id, aid) ){
			ApplyStatus( id, aid, STATUS_FROZEN, 0.0 )
		}
	}
	set_task(0.5, "Do_Blizzard", id+BLIZZARD_TASK, parm, 3)
 }
 public Haze(id, skill, prop)
 {
	if( HasStatus(id) ){
		ResetStatus(id);
		poke_print(id, print_center, "Haze Prevented a Status");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Ice_Beam(id, skill, prop)
 {
	new level = SkillLevel(id, skill);
	new Float:f_origin[3], Float:f_aimvec[3];

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, floatround(level*ICE_BEAM_RADIUS), floatround(level*ICE_BEAM_DAMAGE), skill, _, STATUS_FROZEN, -1);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_SNOW],1,1,1,10+level,0,255,255,255,255,0);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_SNOW],1,1,1,20+level*2,0,255,255,255,255,0);
	origin[id][2] -= 5;
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_STEAM1],1,1,1,10+level,0,230,230,250,255,0);

	poke_sound(id,CHAN_VOICE, SND_CAST);

	if( end_rep_use(id, skill, 2.0) )
		return SKILL_USED;

	return SKILL_READY;
 }
 public Icy_Wind(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) && !g_Status[victim][STATUS_FROZEN]){
		poke_print(victim,print_center,"You were hit by an icy wind")
		poke_print(id,print_center,"You hit someone with icy wind")

		ApplyStatus( id, victim, STATUS_FROZEN, THUNDER_PUNCH_TIME )

		sound_hit(id)
		sound_hit(victim)

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Avalanche(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_WADE)

	new level = SHOCK_WAVE_DAMAGE*SkillLevel(id, skill)
	new radius = SHOCK_WAVE_RADIUS + level

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin);
	skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE|AOE_MAXDAM, _, STATUS_FROZEN, -1);

	FVecIVec( attack_origin, origin[id] );

	// blast circles
	message_begin( MSG_PAS, SVC_TEMPENTITY, origin[id] )
	write_byte(TE_BEAMCYLINDER)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16 + radius/2)
	write_short( SPRITE_INDEX[SPR_STEAM1] )
	write_byte( 0 )	// startframe
	write_byte( 0 )	// framerate
	write_byte( 25 )	// life
	write_byte( 16 )	// width
	write_byte( 0 )	// noise
	write_byte( 0 )   // r
	write_byte( 0 )   // g
	write_byte( 200 ) // b
	write_byte( 150 )	//brightness
	write_byte( 0 )	// speed
	message_end()

	return SKILL_USED;
 }
