
 public Bubble_Beam(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new maxbubbles = BUBBLE_BEAM_NUM + level;

	static bubbles[MAX_PLAYERS];
	if( prop & SI_ROUNDRESET ){
		bubbles[id] = maxbubbles;
		return SKILL_READY;
	}

	new Float:damage = BUBBLE_BEAM_DAMAGE

	new in_water = entity_in_water(id)

	if ( in_water ){
		bubbles[id] = maxbubbles
	}
	if(bubbles[id] > 0){

		new Float:f_sorigin[3], Float:f_aimvec[3], aimvec[3];
		new bubsnum;

		get_user_origin(id, origin[id])
		get_user_origin(id, aimvec, 2)
		IVecFVec(aimvec, f_aimvec)

		bubsnum = get_distance(origin[id], aimvec)/15

		poke_sound(id, CHAN_VOICE, SND_SWIM)

		if( !in_water ){
			bubbles[id]--

			message_begin(MSG_BROADCAST, SVC_TEMPENTITY)	//message begin
			write_byte(15)
			write_coord(origin[id][0])		// start position
			write_coord(origin[id][1])
			write_coord(origin[id][2])
			write_coord(aimvec[0])	// end position
			write_coord(aimvec[1])
			write_coord(aimvec[2])
			write_short(SPRITE_INDEX[SPR_BUBBLE])		// sprite index
			write_byte(bubsnum/2)		// count
			write_byte(1/2)		// life in 0.1's
			write_byte(5)		// scale in 0.1's
			write_byte(5)		// velocity along vector in 10's
			write_byte(5)	// randomness of velocity in 10's
			message_end()

			IVecFVec(origin[id], f_sorigin)

			skillLinedamage(id, f_sorigin, f_aimvec, level, floatround(damage), skill)
		}
		else {
			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
			write_byte(114)
			write_coord(origin[id][0])				// min start position
			write_coord(origin[id][1])
			write_coord(origin[id][2])
			write_coord(aimvec[0])			// max start position
			write_coord(aimvec[1])
			write_coord(aimvec[2])
			write_coord(20)	// float height
			write_short(SPRITE_INDEX[SPR_BUBBLE])				// model index
			write_byte(bubsnum)					// count
			write_coord(100)				// speed
			message_end()

			IVecFVec(origin[id], f_sorigin)

			skillLinedamage(id, f_sorigin, f_aimvec, level, floatround(damage), skill)
		}

		poke_print(id,print_center,"You have %d %ss", bubbles[id], SkillToName(skill) )
		return SKILL_USED;
	}
	else {
		poke_print(id, print_chat, "You are out of %s, go into water and keydown.", SkillToName(skill) )
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public Water_Gun(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new maxwaters = WATER_GUN_NUM + level;

	static waters[MAX_PLAYERS];
	if( prop & SI_ROUNDRESET ){
		waters[id] = maxwaters;
		return SKILL_READY;
	}

	new Float:damage = WATER_GUN_DAMAGE;

	new in_water = entity_in_water(id)

	if( in_water ){
		waters[id] = maxwaters
	}
	if(waters[id] > 0){

		new Float:f_sorigin[3], sorigin[3], Float:f_aimvec[3], aimvec[3];

		get_user_origin(id, origin[id])
		get_user_origin(id, aimvec, 2)
		IVecFVec(aimvec, f_aimvec)

		poke_sound(id, CHAN_VOICE, SND_SWIM)

		if( !in_water ){
			waters[id]--

			f_sorigin[0] = float(origin[id][0])
			f_sorigin[1] = float(origin[id][1])
			f_sorigin[2] = float(origin[id][2])

			FVecIVec(f_sorigin, sorigin)

			skillLinedamage(id, f_sorigin, f_aimvec, level, floatround(damage), skill)
			MakeLine(0,sorigin,aimvec,SPRITE_INDEX[SPR_ZBEAM6],1,1,1,50+level,0,0,0,255,255,0)

			poke_print(id,print_center,"You have %d %ss", waters[id], SkillToName(skill) )

			if( end_rep_use(id, skill, 1.0) )
				return SKILL_USED;
			return SKILL_READY;
		}
		else {
			new bubsnum = get_distance(origin[id], aimvec)/15

			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
			write_byte(114)
			write_coord(origin[id][0])				// min start position
			write_coord(origin[id][1])
			write_coord(origin[id][2])
			write_coord(aimvec[0])			// max start position
			write_coord(aimvec[1])
			write_coord(aimvec[2])
			write_coord(20)	// float height
			write_short(SPRITE_INDEX[SPR_BUBBLE])				// model index
			write_byte(bubsnum)					// count
			write_coord(100)				// speed
			message_end()

			IVecFVec(origin[id], f_sorigin)

			skillLinedamage(id, f_sorigin, f_aimvec, level, floatround(damage), skill)
		}

		return SKILL_USED;
	}
	else {
		poke_print(id, print_chat, "You are out of %s, go into water and keydown.", SkillToName(skill) )
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public Hydro_Pump(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new maxhydros = HYDRO_PUMP_NUM + level;

	static hydros[MAX_PLAYERS];
	if( prop & SI_ROUNDRESET ){
		hydros[id] = maxhydros;
		return SKILL_READY;
	}

	new Float:damage = HYDRO_PUMP_DAMAGE;

	new in_water = entity_in_water(id)

	if( in_water ){
		hydros[id] = maxhydros
	}
	if(hydros[id] > 0){

		new Float:f_sorigin[2][3], sorigin[2][3], Float:f_aimvec[3], aimvec[3];

		get_user_origin(id, origin[id])
		get_user_origin(id, aimvec, 2)
		IVecFVec(aimvec, f_aimvec)

		poke_sound(id, CHAN_VOICE, SND_SWIM)

		if ( !in_water ){
			hydros[id]--

			new Float:velocity[3]
			velocity_by_aim(id, 20, velocity)

			f_sorigin[0][0] = origin[id][0] + velocity[1]
			f_sorigin[0][1] = origin[id][1] - velocity[0]
			f_sorigin[0][2] = origin[id][2] + 20.0
			f_sorigin[1][0] = origin[id][0] - velocity[1]
			f_sorigin[1][1] = origin[id][1] + velocity[0]
			f_sorigin[1][2] = origin[id][2] + 20.0

			FVecIVec(f_sorigin[0], sorigin[0])
			FVecIVec(f_sorigin[1], sorigin[1])

			for(new i=0; i<2; i++){
				skillLinedamage(id, f_sorigin[i], f_aimvec, level, floatround(damage), skill)
				MakeLine(0,sorigin[i],aimvec,SPRITE_INDEX[SPR_ZBEAM6],1,1,1,50+level,0,0,0,255,255,0)
			}

			poke_print(id,print_center,"You have %d %ss", hydros[id], SkillToName(skill) )

			if( end_rep_use(id, skill, 1.0) )
				return SKILL_USED;

			return SKILL_READY;
		}
		else {
			new bubsnum = get_distance(origin[id], aimvec)/15

			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
			write_byte(114)
			write_coord(origin[id][0])				// min start position
			write_coord(origin[id][1])
			write_coord(origin[id][2])
			write_coord(aimvec[0])			// max start position
			write_coord(aimvec[1])
			write_coord(aimvec[2])
			write_coord(20)	// float height
			write_short(SPRITE_INDEX[SPR_BUBBLE])				// model index
			write_byte(bubsnum)					// count
			write_coord(100)				// speed
			message_end()

			IVecFVec(origin[id], f_sorigin[0])

			skillLinedamage(id, f_sorigin[0], f_aimvec, level, floatround(damage), skill)
		}

		poke_print(id,print_center,"You have %d %ss", hydros[id], SkillToName(skill) )
		return SKILL_USED;
	}
	else {
		poke_print(id, print_chat, "You are out of %s, go into water and keydown.", SkillToName(skill) )
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public Surf(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:surf_origin[3]
	new surf_ent = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"))
	set_pev(surf_ent, pev_classname, "pokemod_ent")
	set_pev(surf_ent, pev_owner, id)
	set_pev(surf_ent, pev_iuser1, 5+SkillLevel(id, skill))
	set_pev(surf_ent, pev_iuser2, skill)

	//lets use origin multiple times for the heck of not creating more variables (and im lazy)
	pev(id, pev_origin, surf_origin)
	engfunc(EngFunc_SetOrigin, surf_ent, surf_origin)

	pev(id, pev_angles, surf_origin)
	surf_origin[1] -=90			//my stupidity when making the model, so lets rotate it so it looks correct
	if(surf_origin[1]<-180)
		surf_origin[1] += 360
	surf_origin[0] = 0.0
	surf_origin[2] = 0.0
	set_pev(surf_ent, pev_angles, surf_origin)

	velocity_by_aim(id, 100+level*4, surf_origin)
	surf_origin[2] = 0.0

	xs_vec_normalize(surf_origin, surf_origin)
	surf_origin[0] *= 100+level*4
	surf_origin[1] *= 100+level*4
/*
	new while_time = 0
	while(floatsqroot(surf_origin[0]*surf_origin[0] + surf_origin[1]*surf_origin[1])<100+level*4 && while_time<level){
		surf_origin[0] *= 1.1
		surf_origin[1] *= 1.1
		while_time++		//lets only do this so many times
	}
*/
	set_pev(surf_ent, pev_velocity, surf_origin)

	engfunc(EngFunc_SetModel, surf_ent, MODELS[MDL_SURF])

	set_pev(surf_ent, pev_movetype, MOVETYPE_NOCLIP)
	set_pev(surf_ent, pev_solid, SOLID_BBOX)

	set_pev(surf_ent, pev_maxs, Float:{100.0,100.0,30.0})
	set_pev(surf_ent, pev_mins, Float:{-100.0,-100.0,-30.0})

	set_task(0.1,"check_wave",surf_ent)
	return SKILL_USED;
 }
 public check_wave(ent)
 {
	if(!pev_valid(ent))
		return
	new id = pev(ent, pev_owner)

	new shortestDistance = 9999
	new distance, players[32], pnum
	new wave_dist = SURF_DISTANCE
	AddLvl(id, wave_dist)

	// Find all alive players
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)
	new aid

	// Find the closest enemy
	for( new i = 0; i < pnum; i++ ){
		aid = players[i]
		if(!pev_valid(ent))
			return
		if(!pev_valid(aid))
			continue

		#if SURF_DUCK==1
		if( pev(aid,pev_button) & IN_DUCK )
			continue
		#endif

		distance = fm_get_entity_distance(aid, ent)

		if( distance <= shortestDistance )
			shortestDistance = distance
		if( distance <= wave_dist )
			surf_touch(ent, aid)
	}
	if( shortestDistance>=9999 || (Pokemod_Alive(id) && fm_get_entity_distance(id, ent)>1000)){
		fm_remove_entity(ent)
		return
	}
	set_task(0.1, "check_wave", ent)
	poke_sound(ent, CHAN_VOICE, SND_WADE)
 }
 public Withdraw(id, skill, prop)
 {
	Pokemod_Repair(id, WITHDRAW*SkillLevel(id, skill));
	return SKILL_USED;
 }
 public Bubble(id, skill, prop)
 {
	if( entity_in_water(id) ){
		new Float:air_done;
		pev( id, pev_air_finished, air_done );
		set_pev( id, pev_air_finished, air_done + 2.0 );

		new Float:fOrigin[3], Float:fVelocity[3]
		pev(id, pev_origin, fOrigin)
		velocity_by_aim(id, 35, fVelocity)
		
		new Float:fTemp[3], iFireOrigin[3]
		xs_vec_add(fOrigin, fVelocity, fTemp)
		FVecIVec(fTemp, iFireOrigin)

		new Float:fFireVelocity[3], iFireVelocity[3]
		velocity_by_aim(id, 35, fFireVelocity)
		FVecIVec(fFireVelocity, iFireVelocity)

		new parm[7]
		parm[0] = iFireOrigin[0]
		parm[1] = iFireOrigin[1]
		parm[2] = iFireOrigin[2]
		parm[3] = iFireVelocity[0]
		parm[4] = iFireVelocity[1]
		parm[5] = iFireVelocity[2]
		parm[6] = SPRITE_INDEX[SPR_BUBBLE]
		create_sprite_spray( parm )

		poke_sound(id, CHAN_VOICE, SND_SWIM)

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Crabhammer(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) && MaxSkillChance(id,skill, damage) ){
		skilldamage( victim, id, (damage*CRAB_HAMMER-damage), skill );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Waterfall(id, skill, prop)
 {
	if( !PassTest(id, 1) )
		return SKILL_READY;

	new Float:f_aimvec[3];
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

	new ent = fm_create_entity("info_target");
	set_pev( ent, pev_classname, "pokemod_ent");
	fm_entity_set_model(ent, MODELS[MDL_ROCK]);
	fm_entity_set_origin(ent, f_aimvec);
	set_pev( ent, pev_rendermode, 5);
	set_pev( ent, pev_renderamt, 255.0);
	set_pev( ent, pev_owner, id);

	new parm[3]
	parm[0] = ent;
	parm[1] = skill;
	parm[2] = WATERFALL_TIME * 10;
	AddLvl(id, parm[2]);

	check_waterfall(parm)

	return SKILL_USED;
 }
 public check_waterfall(parm[])
 {
	new ent = parm[0];
	new skill = parm[1];
	parm[2]--;

	if( !parm[2] && pev_valid(ent) )
		fm_remove_entity(ent);
	if( !pev_valid(ent) )
		return;

	new id = pev(ent, pev_owner);

	new range = WATERFALL_RANGE;
	AddLvl(id, range);

	new Float:origfl[3], orig[3];
	pev( ent, pev_origin, origfl );
	FVecIVec(origfl, orig);

	skillAoEdamage(id, origfl, range, -1, skill, AOE_MAXDAM|AOE_ATEAM);
	skillAoEdamage(id, origfl, range, 1, skill, AOE_MAXDAM|AOE_VTEAM);

	if( (parm[2] % 2) == 0 ){
		new parmlol[7];
		parmlol[0] = orig[0];
		parmlol[1] = orig[1];
		parmlol[2] = orig[2];
		parmlol[3] = 0;
		parmlol[4] = 0;
		parmlol[5] = 10;
		parmlol[6] = SPRITE_INDEX[SPR_GAS];
		create_sprite_spray( parmlol );
	}

	if( (parm[2] % 10) == 0 ){
		poke_sound(ent, CHAN_VOICE, SND_SWIM)

		message_begin( MSG_PAS, SVC_TEMPENTITY, orig )
		write_byte(TE_BEAMCYLINDER)
		write_coord( orig[0])
		write_coord( orig[1])
		write_coord( orig[2] - 16)
		write_coord( orig[0])
		write_coord( orig[1])
		write_coord( orig[2] - 16 + range)
		write_short( SPRITE_INDEX[SPR_GAS] )
		write_byte( 0 )		// startframe
		write_byte( 0 )		// framerate
		write_byte( 10 )		// life
		write_byte( 16 )	// width
		write_byte( 0 )		// noise
		write_byte( 88 )
		write_byte( 20 )
		write_byte( 255 )
		write_byte( 255 )	//brightness
		write_byte( 0 )		// speed
		message_end()
	}

	set_task(0.1, "check_waterfall", 0, parm, 3);
 }
 public Water_Pulse(id, skill, prop)
 {
	new Float: origin[3], Float: aim[3], Float: velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY

	set_pev(Ent, pev_classname, "poke_rock")
	fm_entity_set_model(Ent, MODELS[MDL_ROCK])

	ChangeToIce(Ent, skill)

	fm_entity_set_origin(Ent, origin)
	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, ROCK_THROW_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_SWIM)

	if( !entity_in_water(id) ){
		// Trail on enity.
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
		write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
		write_short(Ent)	// entity:attachment to follow
		write_short(SPRITE_INDEX[SPR_ZBEAM6])	// sprite index
		write_byte(10)	// life in 0.1's
		write_byte(15)		// line width in 0.1's
		write_byte(0)	//r,g,b
		write_byte(140)
		write_byte(200)
		write_byte(250)	// brightness
		message_end()

		//Glow the entity blue
		fm_set_rendering(Ent, kRenderFxGlowShell, 0, 160, 255, kRenderNormal, 150)
	}
	return SKILL_USED;
 }
 ChangeToIce(Ent, skill)
 {
	set_pev(Ent, pev_iuser2, skill)

	set_pev(Ent, pev_mins, Float:{-3.0, -3.0, -3.0})
	set_pev(Ent, pev_maxs, Float:{3.0, 3.0, 3.0})

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)

	return SKILL_USED
 }
 public Octozooka(id, skill, prop)
 {
	new Float: origin[3], Float: aim[3], Float: velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY

	set_pev(Ent, pev_classname, "poke_rock")
	fm_entity_set_model(Ent, MODELS[MDL_DRKBALL])

	ChangeToBoulder(Ent, skill)

	fm_entity_set_origin(Ent, origin)
	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, ROCK_THROW_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_SWIM)

	if( !entity_in_water(id) ){
		// Trail on enity.
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
		write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
		write_short(Ent)	// entity:attachment to follow
		write_short(SPRITE_INDEX[SPR_GAS])	// sprite index
		write_byte(50)	// life in 0.1's
		write_byte(8)		// line width in 0.1's
		write_byte(200)	//r,g,b
		write_byte(200)
		write_byte(200)
		write_byte(200)	// brightness
		message_end()

		//Glow the entity black
		fm_set_rendering(Ent, kRenderFxGlowShell, 100, 100, 100, kRenderNormal, 200)
	}
	return SKILL_USED;
 }
 public Rain_Dance(id, skill, prop)
 {
	if( g_Status[id][STATUS_BURN] ){
		RemoveStatus(id, STATUS_BURN);
		poke_print(id, print_center, "Rain Dance Protected you from Burning");
		return SKILL_USED;
	}
	return SKILL_READY;
 }

