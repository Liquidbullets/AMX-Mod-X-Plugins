
 public Dig(id, skill, prop)
 {
	#if DIG_KNIFE == 1
		new weapon;
		PokeSkillTarget(id, _, weapon);
		// if they try to change weapons
		if( prop & SI_SPEED ){
			if( !knife_weapon(weapon) ){
				poke_UnStuck(id);
				poke_print(id, print_chat, "You cannot dig with weapons.");
			}
			return SKILL_READY;
		}
	#else
		if( prop & SI_SPEED ) return SKILL_READY;
	#endif

	new Float:start_origin[3], Float:velocity[3], dig_length = 50+SkillLevel(id, skill);

	pev(id, pev_origin, start_origin);
	get_user_origin(id, aim[id], 3);
	IVecFVec( aim[id], velocity );

	if( get_distance_f( start_origin, velocity ) > dig_length ){
		if( !fm_trace_hull(start_origin, poke_getHullSize(id), id) ){
			g_StopSkill[id] = true;
			sound_deny(id);
			return SKILL_READY;
		}
	}

	velocity_by_aim(id, dig_length, velocity);
	start_origin[0] += velocity[0];
	start_origin[1] += velocity[1];
	start_origin[2] += velocity[2];
	fm_entity_set_origin(id, start_origin);

	#if DIG_KNIFE == 1
		if( !knife_weapon(weapon) ){
			poke_UnStuck(id);
			poke_print(id, print_chat, "You cannot dig with weapons.");
		}
	#endif

	sound_hit(id);

	return SKILL_USED;
 }
 public Earthquake(id, skill, prop)
 {
	new aid, players[MAX_PLAYERS], pnum
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	new maxdamage = EARTHQUAKE_DAMAGE, radius = EARTHQUAKE_RADIUS, dist;
	AddLvl(id, radius);

	get_user_origin(id, origin[id]);
	poke_sound(id, CHAN_VOICE, SND_BIGGUN)

	for(new i = 0; i<pnum; i++)
	{
		aid = players[i];
		screenshake(aid, 100, 5.0, 100);

		if( !Pokemod_FF(id, aid) && !entity_in_water(aid) && entity_on_ground(aid) ){

			get_user_origin(aid, origin[aid]);
			dist = get_distance( origin[id], origin[aid] );

			if( dist <= radius ){
				poke_sound(aid, CHAN_VOICE, SND_BIGGUN);
				skilldamage(aid, id, maxdamage - ( ( maxdamage * dist) / radius ), skill);
			}

		}
	}

	return SKILL_USED;
 }
 public Fissure(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && MaxSkillChance(id,skill,FISSURE) ){
		skilldamage(victim, id, AUTO_KILL, skill);
		poke_sound(id, CHAN_VOICE, SND_BIGGUN);
		screenshake(id, 100, 5.0, 100);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Bone_Club(id, skill, prop)
 {
	new Ent = 0;
	while( (Ent = fm_find_ent_by_owner(Ent, "poke_attack", id) ) ){
		if( pev(Ent, pev_iuser2) == skill ){
			poke_print(id, print_center, "You already have a %s on the map, go find it", SkillToName(skill) );
			return SKILL_READY;
		}
	}

	new Float: origin[3], Float: aim[3], Float: velocity[3]

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	Ent = fm_create_entity("info_target")

	if( !Ent ) return SKILL_READY;

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, BONE_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_BONE])

	new Float:MinBox[3] = {-5.0, -5.0, -5.0}
	new Float:MaxBox[3] = {5.0, 5.0, 5.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	//lets move it a little away from them
	velocity_by_aim(id, 40, velocity);
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];
	fm_entity_set_origin(Ent, origin);

	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)
	set_pev(Ent, pev_owner, id)

	set_pev(Ent, pev_animtime, 1.0);
	set_pev(Ent, pev_framerate, 1.0);
	set_pev(Ent, pev_sequence, 1);

	velocity_by_aim(id, BONE_VEC, velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)

	// Trail on enity.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
	write_short(Ent)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_XPLODE])	// sprite index
	write_byte(10)	// life in 0.1's
	write_byte(10)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	return SKILL_USED;
 }
 public Bonemerang(id, skill, prop)
 {
	new Ent = 0;
	while( (Ent = fm_find_ent_by_owner(Ent, "poke_attack", id) ) ){
		if( pev(Ent, pev_iuser2) == skill ){
			poke_print(id, print_center, "You called your %s back", SkillToName(skill) );
			entity_set_follow(Ent, id, 1.0*BONE_VEC);
			return SKILL_READY;
		}
	}

	new Float: origin[3], Float: aim[3], Float: velocity[3]

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	Ent = fm_create_entity("info_target")

	if( !Ent ) return SKILL_READY;

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, BONE_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_BONE])

	new Float:MinBox[3] = {-5.0, -5.0, -5.0}
	new Float:MaxBox[3] = {5.0, 5.0, 5.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	//lets move it a little away from them
	velocity_by_aim(id, 40, velocity);
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];
	fm_entity_set_origin(Ent, origin);

	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)
	set_pev(Ent, pev_owner, id)

	set_pev(Ent, pev_animtime, 1.0);
	set_pev(Ent, pev_framerate, 1.0);
	set_pev(Ent, pev_sequence, 1);

	velocity_by_aim(id, BONE_VEC, velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)

	// Trail on enity.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
	write_short(Ent)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_XPLODE])	// sprite index
	write_byte(10)	// life in 0.1's
	write_byte(10)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	return SKILL_USED;
 }
 public Aeroblast(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id])
	get_user_origin(id, aim[id], 2)
	IVecFVec(origin[id], f_origin)
	IVecFVec(aim[id], f_aimvec)

	skillLinedamage(id, f_origin, f_aimvec, level/4, level, skill, _, STATUS_SLOW, -1)
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_ARBOK],1,1,1,level,0,255,255,255,255,0)
	poke_sound(id,CHAN_VOICE, SND_AG_DIE)

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
