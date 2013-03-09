 
 public String_Shot(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) && !g_Status[victim][STATUS_SLOW]){
		poke_print(victim,print_center,"You were hit by a string shot")
		poke_print(id,print_center,"You hit someone with string shot")

		ApplyStatus( id, victim, STATUS_SLOW, STRING_SHOT_TIME )

		sound_hit(id)
		sound_hit(victim)

		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ARBOK],1,1,10,10,0,89,89,89,100,0)

		return SKILL_USED;
	}
	return SKILL_READY;
 } 
 public Leech_Life(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !ValidPlayer(victim) )
		return SKILL_READY;
	poke_print(victim, print_center, "Someone is draining life from you")
	new damage = skilldamage(victim, id, LEECHLIFE, skill);
	if( !damage )
		return SKILL_READY;
	Pokemod_Heal(id, damage );
	return SKILL_USED;
 }
 public Pin_Missile(id, skill, prop)
 {
	new Ent = fm_create_entity("info_target")
	if (!Ent) return SKILL_READY;

	new Float:origin[3], Float:angle[3], Float:velocity[3]

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, angle)

	//move it a little in front of them
	velocity_by_aim(id, 20, velocity)
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, PIN_MISSLE_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_ROCKET])

	new Float:MinBox[3] = {-3.0, -3.0, -3.0}
	new Float:MaxBox[3] = {3.0, 3.0, 3.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	fm_entity_set_origin(Ent, origin)

	set_pev(Ent, pev_angles, angle)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)
	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, PIN_MISSLE_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	// Trail on enity.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
	write_short(Ent)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_MUZZLEFLASH1])	// sprite index
	write_byte(10)	// life in 0.1's
	write_byte(5)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)
	return SKILL_USED;
 }
 public Spider_Web(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id])
	get_user_origin(id, aim[id], 2)
	IVecFVec(origin[id], f_origin)
	IVecFVec(aim[id], f_aimvec)

	skillLinedamage(id, f_origin, f_aimvec, level/6, level, skill, _, STATUS_PARALYZE, -1)
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_ARBOK],1,1,10,10,0,89,89,89,100,0)

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
