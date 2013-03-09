
 public Night_Shade(id, skill, prop)
 {
	if( BindTime(id) ){
		fm_set_rendering(id)
		screenflash(id)
	}
	else{
		new max = clamp(NIGHTSHADE_MAX_INVIS-SkillLevel(id, skill), 1, 255)
		new min = clamp(NIGHTSHADE_MIN_INVIS, 0, max)
		new fade, mult = (max - min)/9
		switch(gtime){
			case 0: fade = max
			case 1,19: fade = mult*8 + min
			case 2,18: fade = mult*7 + min
			case 3,17: fade = mult*6 + min
			case 4,16: fade = mult*5 + min
			case 5,15: fade = mult*4 + min
			case 6,14: fade = mult*3 + min
			case 7,13: fade = mult*2 + min
			case 8,9,11,12: fade = mult + min
			case 10: fade = min
		}

		fade = clamp(fade, min, max)	//don't want to set it to something too high or low

		fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, fade);
		if( fade < 33 ) fade = 33;
		screenflash(id, 0, 0, 0, 255-fade, 2.0)

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Confuse_Ray(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id])
	get_user_origin(id, aim[id], 2)
	IVecFVec(origin[id], f_origin)
	IVecFVec(aim[id], f_aimvec)

	skillLinedamage(id, f_origin, f_aimvec, level/2, level, skill, _, STATUS_CONFUSE, -1)
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_TRAIL],1,1,1,level,0,255,255,255,255,0)
	poke_sound(id,CHAN_VOICE, SND_TU_PING)

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Shadow_Ball(id, skill, prop)
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
	fm_entity_set_model(newEnt, MODELS[MDL_DRKBALL])

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
	set_pev( newEnt, pev_renderamt, 200.0 )
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

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(22)		// TE_BEAMFOLLOW
	write_short(newEnt)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_ZBEAM6])	// sprite index
	write_byte(50)	// life in 0.1's
	write_byte(8)		// line width in 0.5's
	write_byte(200)	
	write_byte(200)
	write_byte(200)  //r,g,b
	write_byte(200)	// brightness
	message_end()

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(22)		// TE_BEAMFOLLOW
	write_short(newEnt)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_BALLSMOKE])	// sprite index
	write_byte(50)	// life in 0.1's
	write_byte(10)		// line width in 0.5's
	write_byte(0)	
	write_byte(0)
	write_byte(0)  //r,g,b
	write_byte(255)	// brightness
	message_end()

	sound_cast(id)
	set_bind(id, WAIT_TIME)
	screenflash(id, 0, 0, 0, 255)
	return SKILL_READY;
 }
