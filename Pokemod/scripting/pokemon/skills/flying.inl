
 public Fly(id, skill, prop)
 {
	new jumpvec = FLY;
	AddLvl(id, jumpvec);

	new Float:velocity[3];
	pev(id, pev_velocity, velocity);

	if(velocity[2] < 1.5*jumpvec)
		velocity[2] += jumpvec;

	if( pev(id, pev_button)&IN_FORWARD
	&& sqroot(floatround(velocity[0]*velocity[0] + velocity[1]*velocity[1])) < jumpvec ){
		new Float:vec[3];
		velocity_by_aim(id, jumpvec , vec);
		velocity[0] += vec[0];
		velocity[1] += vec[1];
	}

	poke_sound(id, CHAN_VOICE, SND_JUMPLAND)
	set_pev(id, pev_velocity, velocity);
	set_pev(id, pev_gaitsequence, 6);

	#if FLY_FLAP==1
		g_lastSkill[id] = 0.0;
		g_StopSkill[id] = true;
	#endif

	if( end_rep_use( id, skill, FLY_FLAPS/10.0 ) )
		return SKILL_USED;

	return SKILL_READY;
 }
 public Sky_Attack(id, skill, prop)
 {
	if(!PassTest(id, 0))
		return SKILL_READY;

	new aid, lastid = 0, Float:lastdist = 999.9
	new players[MAX_PLAYERS], pnum
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)
	for(new i=0; i<pnum; i++){
		aid = players[i]

		if( !Pokemod_FF(id, aid)
		&& ( is_user_outside(aid) || fm_is_ent_visible(id, aid) )
		&& fm_entity_range(id,aid) < lastdist ){
			lastdist = fm_entity_range(id, aid)
			lastid = aid
		}
	}
	if(Pokemod_Alive(lastid) && lastid!=id){
		new Float:delaytime = 5.0
		aim[id][0] = lastid
		aim[id][1] = lastid
		aim[id][2] = lastid
		new Float:tasktime = f_SkillPercent(id, skill, delaytime);
		new parm[2];
		parm[0] = id;
		parm[1] = skill;
		set_task((delaytime-tasktime) + 1.0, "finish_Sky_Attack", 0, parm, 2)

		new Float:origin[3]
		pev(id, pev_origin, origin)
		origin[2] += 10000
		set_pev(id, pev_origin, origin)

		//make their screen black (this makes it so the map isn't traced onto their screen)
		screenflash(id, 255, 255, 255, 255, 0.1)

		new name[32]
		get_user_name(lastid, name, 31)
		poke_print(id, print_center, "Sky attacking %s...",name)
		sound_cast(id)
		set_bind(id, WAIT_TIME)
	}
	else{
		sound_deny(id)
		poke_print(id, print_center, "There is no enemy near you.")
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public finish_Sky_Attack(parm[2])
 {
	new id = parm[0], skill = parm[1];
	new checker = aim[id][0]
	//stop if their aim was reset, so these should all be the same
	if( !Pokemod_Alive(id) || !checker || checker!=aim[id][1] || checker!=aim[id][2])
		return

	new lastid = checker
	new bool:fire_attack = (PokeType[ Pokemon[id] ] & TYPE_FIRE ) ? true : false ;

	new Float:temporigin[3]
	pev(lastid, pev_origin, temporigin)
	temporigin[2] += 40 //set them a little above the player
	set_pev(id, pev_origin, temporigin)
	pev(lastid, pev_angles, temporigin)
	set_pev(id, pev_angles, temporigin)
	set_pev(id, pev_fixangle, 1)

	poke_UnStuck(id)
	bind_used(id, skill)
	poke_sound(id, CHAN_VOICE, SND_BIGGUN)

	poke_print(lastid, print_center, "You have been sky attacked")

	new level = SKY_ATTACK_DAMAGE*SkillLevel(id, skill)
	new radius = SKY_ATTACK_RADIUS + level

	new Float:attack_origin[3];
	pev(lastid, pev_origin, attack_origin)	//theoretically, they should be right above lastid, but we want the damage to emit from lastid
	if( fire_attack )
		skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE, _, STATUS_BURN, 2)
	else
		skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE)

	FVecIVec( attack_origin, origin[id] );

	// blast circles
	message_begin( MSG_PAS, SVC_TEMPENTITY, origin[id] )
	write_byte(TE_BEAMCYLINDER)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16 + radius)
	write_short( (fire_attack) ? SPRITE_INDEX[SPR_FIRE] : SPRITE_INDEX[SPR_SHOCKWAVE] )
	write_byte( 0 )		// startframe
	write_byte( 0 )		// framerate
	write_byte( 10 )		// life
	write_byte( 16 )	// width
	write_byte( 0 )		// noise
	write_byte( 188 )
	write_byte( 220 )
	write_byte( 255 )
	write_byte( 255 )	//brightness
	write_byte( 0 )		// speed
	message_end()
 }
 public Peck(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( skilldamage(victim, id, PECK, skill) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Drill_Peck(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( skilldamage(victim, id, DRILL_PECK, skill) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Steel_Wing(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && PassTest(id, 1) ){
		get_user_origin(id, origin[id]);
		get_user_origin(victim, aim[id]);
		set_user_origin(id, aim[id]);

		ApplyStatus( id, victim, STATUS_SLOW, THUNDER_PUNCH_TIME )
		poke_print( victim, print_center, "You’ve Been Hit with Steel Wing")
	
		poke_UnStuck(id)

		return SKILL_USED;
	}
	else{
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }