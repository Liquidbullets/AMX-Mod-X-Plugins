
 public Thunderbolt(id, skill, prop)
 {
	poke_sound(id, CHAN_VOICE, SND_SPARK)
	new players[MAX_PLAYERS], pnum, aid, damage = SkillLevel(id, skill)
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL )
	for( new i = 0; i < pnum; i++ )
	{
		aid = players[i]
		if( !Pokemod_FF(id, aid) && fm_entity_range(id, aid)<THUNDERBOLT_DISTANCE && fm_is_ent_visible(id, aid))
		{
			MakeLine_Ent(0,id,aid,SPRITE_INDEX[SPR_LGTNING],1,1,10,damage*2,5,250,250,10,100,0)

			skilldamage(aid, id, damage, skill)
			poke_print(aid, print_center, "You were hit by a thunderbolt")
			poke_sound(aid, CHAN_VOICE, SND_SPARK)
		}
	}
	return SKILL_USED;
 }
 public Thunder(id, skill, prop)
 {
	poke_sound(id, CHAN_VOICE, SND_BIGGUN)

	new origin[3], toporigin[3], temporigin[3], Float:tempdist
	new aid, tid, x, damage = SkillLevel(id, skill) / THUNDER_DAMAGE
	new players[MAX_PLAYERS], pnum, i;
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL )

	get_user_origin(id, origin)
	get_user_origin(id, toporigin)
	for( i=0 ; i<5; i++){
		temporigin[0] = toporigin[0] + random_num(-1*damage, damage)
		temporigin[1] = toporigin[1] + random_num(-1*damage, damage)
		temporigin[2] = toporigin[2] + random_num(-1*damage, damage)
		MakeLine(0,origin,temporigin,SPRITE_INDEX[SPR_LGTNING],1,1,10,damage,damage,250,250,10,100,0)
	}

	#if THUNDER_KILL == 0
		new hp;
	#endif

	for( i=0; i < pnum; i++ ){
		aid = players[i]
		if( Pokemod_FF(id, aid) ) continue;

		tempdist = fm_entity_range(id, aid)
		if(	(tempdist<THUNDER_RADIUS && is_user_outside(id) )
		||	(tempdist<THUNDER_RADIUS*2 && fm_is_ent_visible(id, aid) )
		){
			get_user_origin(aid, origin)
			toporigin[0] = origin[0]
			toporigin[1] = origin[1]
			toporigin[2] = origin[2]+1000
			MakeLine(0,origin,toporigin,SPRITE_INDEX[SPR_LGTNING],1,1,10,damage*2,10,250,250,10,100,0)

			#if THUNDER_KILL == 0
			hp = Health(aid);
			if( hp > 1 ){
				if( hp > damage ) skilldamage(aid, id, damage, skill)
				else skilldamage(aid, id, hp-1, skill)
			}
			#else
				skilldamage(aid, id, damage, skill)
			#endif

			poke_print(aid, print_center, "You were hit by thunder")
			poke_sound(aid, CHAN_VOICE, SND_SPARK)

			for ( x = 0; x < pnum; x++ )
			{
				tid = players[x]
				if( aid==tid && Pokemod_FF(id, tid) )
					continue
				if(!fm_is_ent_visible(id,tid) && fm_is_ent_visible(aid, tid) && fm_entity_range(aid, tid)<THUNDER_RADIUS2)
				{
					MakeLine_Ent(0,aid,tid,SPRITE_INDEX[SPR_LGTNING],1,1,10,damage,10,250,250,10,100,0)

					#if THUNDER_KILL == 0
					hp = Health(tid);
					if( hp > 1 ){
						if( hp > damage/THUNDER_DAMAGE2 ) skilldamage(tid, id, damage/THUNDER_DAMAGE2, skill)
						else skilldamage(tid, id, hp-1, skill)
					}
					#else
						skilldamage(tid, id, damage/THUNDER_DAMAGE2, skill)
					#endif

					poke_print(tid, print_center, "You were hit by thunder")
					poke_sound(tid, CHAN_VOICE, SND_SPARK)
				}
			}
		}
	}
	return SKILL_USED;
 }
 public Thundershock(id, skill, prop)
 {
	new Float:f_aimvec[3];
	poke_sound(id, CHAN_VOICE, SND_BIGGUN);
	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 3);

	if(-50>aim[id][0]-origin[id][0]>50)			//only change it if they are at least that far away
		aim[id][0] += ( ( aim[id][0] - origin[id][0] > 0 ) ? -50 : 50 );
	if(-50>aim[id][1]-origin[id][1]>50)			//only change it if they are at least that far away
		aim[id][1] += ( ( aim[id][1] - origin[id][1] > 0 ) ? -50 : 50 );
	if(-50>aim[id][2]-origin[id][2]>50)			//only change it if they are at least that far away
		aim[id][2] += ( ( aim[id][2] - origin[id][2] > 0 ) ? -50 : 50 );
	else
		aim[id][2] += 40;
	IVecFVec(aim[id], f_aimvec);

	new level = SkillLevel(id, skill) + 20;

	new ent = fm_create_entity("info_target");
	set_pev( ent, pev_classname, "pokemod_ent");
	fm_entity_set_model(ent, MODELS[MDL_ROCK]);
	fm_entity_set_origin(ent, f_aimvec);
	set_pev( ent, pev_rendermode, 5);
	set_pev( ent, pev_renderamt, 0.0);
	set_pev( ent, pev_owner, id);

	MakeLine_Ent(0,id,ent,SPRITE_INDEX[SPR_LGTNING],1,1,5,level*2,10,250,250,10,100,0)

	new parm[4];
	parm[0] = ent;
	parm[1] = skill;
	parm[2] = level;
	parm[3] = 1+(level/THUNDERSHOCKS);
	set_task(0.5,"check_thundershock", _, parm, 4 )

	return SKILL_USED
 }
 public check_thundershock(parm[])
 {
	new ent = parm[0];
	new skill = parm[1];
	new level = parm[2];
	parm[3]--;

	if( !parm[3] && pev_valid(ent) )
		fm_remove_entity(ent);

	if( !pev_valid(ent) )
		return;

	new id = pev(ent, pev_owner);

	new temporigin[3], shockorigin[3], Float:f_shockorigin[3];
	pev(ent, pev_origin, f_shockorigin);
	FVecIVec( f_shockorigin, shockorigin );
	for( new i=0 ; i<5; i++){
		temporigin[0] = shockorigin[0] + random_num(-1*level, level)
		temporigin[1] = shockorigin[1] + random_num(-1*level, level)
		temporigin[2] = shockorigin[2] + random_num(-1*level, level)
		MakeLine(0,shockorigin,temporigin,SPRITE_INDEX[SPR_LGTNING],1,1,10,level,level,250,250,10,100,0)
	}

	poke_sound(id, CHAN_VOICE, SND_SPARK);

	skillAoEdamage(id, f_shockorigin, level, 10+level, skill, AOE_NORMAL, _, STATUS_PARALYZE);

	set_task(0.5, "check_thundershock", 0, parm, 4);
 }
 public Thunder_Wave(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) ){
		ApplyStatus( id, victim, STATUS_PARALYZE, THUNDER_WAVE_TIME )
		poke_print( victim, print_center, "You were hit by a Thunder Wave")
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Shock_Wave(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_SPARK)

	new level = SHOCK_WAVE_DAMAGE*SkillLevel(id, skill)
	new radius = SHOCK_WAVE_RADIUS + level

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin);
	skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE|AOE_MAXDAM, _, STATUS_PARALYZE, -1);

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
	write_short( SPRITE_INDEX[SPR_LGTNING] )
	write_byte( 0 )		// startframe
	write_byte( 0 )		// framerate
	write_byte( 25 )		// life
	write_byte( 16 )	// width
	write_byte( 0 )		// noise
	write_byte( 255 )
	write_byte( 255 )
	write_byte( 0 )
	write_byte( 255 )	//brightness
	write_byte( 0 )		// speed
	message_end()

	return SKILL_USED;
 }
 public Zap_Cannon(id, skill, prop)
 {
	new level = SkillLevel(id, skill);
	new Float:f_origin[3], Float:f_aimvec[3];

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, floatround(level*ZAP_CANNON_RADIUS), floatround(level*ZAP_CANNON_DAMAGE), skill, _, STATUS_PARALYZE, -1);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_LGTNING],1,1,1,10+level,0,70,70,70,255,0);
	origin[id][2] -= 5;
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_LGTNING],1,1,1,10+level,0,255,255,0,255,0);

	poke_sound(id,CHAN_VOICE, SND_BIGGUN);

	if( end_rep_use(id, skill, 2.0) )
		return SKILL_USED;

	return SKILL_READY;
 }
