
 public Fire_Blast(id, skill, prop)
 {
	new Float: origin[3], Float: aim[3], Float: velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY

	set_pev(Ent, pev_classname, "poke_rock")
	fm_entity_set_model(Ent, MODELS[MDL_ROCK])

	ChangeToBoulder(Ent, skill)

	fm_entity_set_origin(Ent, origin)
	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, ROCK_THROW_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)

	if( !entity_in_water(id) ){
		// Trail on enity.
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
		write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
		write_short(Ent)	// entity:attachment to follow
		write_short(SPRITE_INDEX[SPR_FIRE])	// sprite index
		write_byte(10)	// life in 0.1's
		write_byte(10)		// line width in 0.1's
		write_byte(255)	//r,g,b
		write_byte(255)
		write_byte(255)
		write_byte(255)	// brightness
		message_end()

		//Glow the entity red
		fm_set_rendering(Ent, kRenderFxGlowShell, 255, 130, 5, kRenderNormal, 20)
	}
	return SKILL_USED;
 }
 public Flamethrower(id, skill, prop)
 {
	if ( entity_in_water(id) ){
		poke_print(id, print_chat, "You cannot use Flame Thrower while underwater")
		sound_deny(id)
		g_StopSkill[id] = true;
		return SKILL_READY;
	}

	new flamecount = 3

	new Float:fOrigin[3], Float:fVelocity[3]
	pev(id, pev_origin, fOrigin)
	velocity_by_aim(id, 35, fVelocity)

	new Float:fTemp[3], iFireOrigin[3]
	xs_vec_add(fOrigin, fVelocity, fTemp)
	FVecIVec(fTemp, iFireOrigin)

	new Float:fFireVelocity[3], iFireVelocity[3]
	velocity_by_aim(id, 100, fFireVelocity)
	FVecIVec(fFireVelocity, iFireVelocity)

	new parm[7]
	parm[0] = iFireOrigin[0]
	parm[1] = iFireOrigin[1]
	parm[2] = iFireOrigin[2]
	parm[3] = iFireVelocity[0]
	parm[4] = iFireVelocity[1]
	parm[5] = iFireVelocity[2]
	parm[6] = SPRITE_INDEX[SPR_EXPLODE1]

	create_sprite_spray( parm )
	if( flamecount-1 )
		set_task(0.1, "create_sprite_spray", 0, parm, 7, "a", flamecount-1)

	parm[0] = id
	parm[1] = 1
	parm[2] = skill
	spray_check( parm )
	if( flamecount-1 )
		set_task(0.1, "spray_check", 0, parm, 3, "a", flamecount-1)

	poke_sound(id, CHAN_VOICE, SND_FLAME)

	if( end_rep_use(id, skill, 3.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Fire_Spin(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_FLAME)

	new level = FIRE_SPIN_DAMAGE*SkillLevel(id, skill)
	new radius = FIRE_SPIN_RADIUS + level

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin)
	attack_origin[2] += 30;
	skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE, _, STATUS_BURN, 10)

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
	write_short( SPRITE_INDEX[SPR_FIRE] )
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

	return SKILL_USED;
 }
 public Ember(id, skill, prop)
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
	fm_set_rendering( ent, kRenderFxGlowShell, 255, 130, 5, kRenderNormal, 20);
	set_pev( ent, pev_mins, Float:{-50.0, -50.0, -50.0});
	set_pev( ent, pev_maxs, Float:{50.0, 50.0, 50.0});
	set_pev( ent, pev_solid, SOLID_BBOX);
	set_pev( ent, pev_movetype, MOVETYPE_FLY);

	new parm[3]
	parm[0] = ent;
	parm[1] = skill;
	parm[2] = EMBER_TIME * 10;
	AddLvl(id, parm[2]);

	check_ember(parm)

	return SKILL_USED;
 }
 public check_ember(parm[])
 {
	new ent = parm[0];
	new skill = parm[1];
	parm[2]--;

	if( !parm[2] && pev_valid(ent) )
		fm_remove_entity(ent);
	if( !pev_valid(ent) )
		return;

	new id = pev(ent, pev_owner);

	new range = EMBER_RANGE;
	AddLvl(id, range);

	new aid, players[MAX_PLAYERS], pnum
	poke_get_players(players,pnum,PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_NOTEAM,Pokemod_Team(id))

	for(new i = 0; i<pnum; i++)
	{
		aid = players[i]

		if( !g_Status[aid][STATUS_BURN] ){
			if( fm_entity_range(ent,aid) < range ) {
				ApplyStatus( id, aid, STATUS_BURN, 0.0 )
				poke_print(aid,print_center,"You have walked into %s", SkillToName(skill));
			}
		}
	}

	new Float:ember_origfl[3], ember_orig[3];
	pev( ent, pev_origin, ember_origfl );
	FVecIVec(ember_origfl, ember_orig);

	new parmlol[7];
	parmlol[0] = ember_orig[0];
	parmlol[1] = ember_orig[1];
	parmlol[2] = ember_orig[2];
	parmlol[3] = 0;
	parmlol[4] = 0;
	parmlol[5] = 10;
	parmlol[6] = SPRITE_INDEX[SPR_XFIRE];
	create_sprite_spray( parmlol );

	if( (parm[2] % 10) == 0 ){
		poke_sound(ent, CHAN_VOICE, SND_FLAME)

		message_begin( MSG_PAS, SVC_TEMPENTITY, ember_orig )
		write_byte(TE_BEAMCYLINDER)
		write_coord( ember_orig[0])
		write_coord( ember_orig[1])
		write_coord( ember_orig[2] - 16)
		write_coord( ember_orig[0])
		write_coord( ember_orig[1])
		write_coord( ember_orig[2] - 16 + range)
		write_short( SPRITE_INDEX[SPR_FIRE] )
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

	set_task(0.1, "check_ember", 0, parm, 3);
 }
 public Sunny_Day(id, skill, prop)
 {
	new aid = PokeSkillTarget(id)
	if( ValidPlayer(aid) && Pokemod_Alive(aid) && !Pokemod_FF(id, aid) ){
		poke_print(id, print_center, "You burned someone using Sunny Day")
		poke_print(aid, print_center, "Someone burned you using Sunny Day")
		ApplyStatus( id, aid, STATUS_BURN, get_ppcvar_float(pm_burntime) )
		set_pev(aid, pev_velocity, Float:{0.0,0.0,0.0} );

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Sacred_Fire(id, skill, prop)
 {
	new level = SkillLevel(id, skill);
	new Float:f_origin[3], Float:f_aimvec[3];

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, 	floatround(level*SACRED_RADIUS), 	floatround(level*SACRED_DAMAGE), skill, _, STATUS_BURN, EMBER_TIME);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_FIRE],1,1,1,10+level,0,188,	220,255,255,0);
	origin[id][2] -= 5;
	poke_sound(id,CHAN_VOICE, SND_FLAME);

	if( end_rep_use(id, skill, 2.0) )
		return SKILL_USED;

	return SKILL_READY;
 }
 public Magma_Armor(id, skill, prop)
 {
	if( g_Status[id][STATUS_FROZEN] ){
		RemoveStatus(id, STATUS_FROZEN);
		screenflash(id, 255, 0, 0, 50)
		poke_print(id, print_center, " You are protected from freezing");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Overheat(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_FLAME)

	new level = SHOCK_WAVE_DAMAGE*SkillLevel(id, skill)
	new radius = SHOCK_WAVE_RADIUS + level

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin);
	skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE|AOE_MAXDAM, _, STATUS_BURN, -1);

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
	write_short( SPRITE_INDEX[SPR_FIRE] )
	write_byte( 0 )	// startframe
	write_byte( 0 )	// framerate
	write_byte( 25 )	// life
	write_byte( 16 )	// width
	write_byte( 0 )	// noise
	write_byte( 255 )
	write_byte( 255 )
	write_byte( 0 )
	write_byte( 255 )	//brightness
	write_byte( 0 )	// speed
	message_end()

	return SKILL_USED;
 }
