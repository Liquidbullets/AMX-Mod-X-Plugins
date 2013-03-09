
 public Rock_Throw(id, skill, prop)
 {
	new Float:origin[3], Float:angle[3], Float:velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, angle)
	origin[2] += 20

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY

	ChangeToBoulder(Ent, skill)
	set_pev(Ent, pev_classname, "poke_rock")
	fm_entity_set_model(Ent, MODELS[MDL_ROCK])
	set_pev(Ent, pev_owner, id)

	fm_entity_set_origin(Ent, origin)
	set_pev(Ent, pev_angles, angle)

	velocity_by_aim(id, ROCK_THROW_VEC, velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE, SND_TU_FIRE)

	return SKILL_USED;
 }
 public Rock_Slide(id, skill, prop)
 {
	new Float:f_aimvec[3], Ent
	new rockammount = 1+(SkillLevel(id, skill) / ROCK_SLIDE_LEVEL)

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

	for(new i=0; i<rockammount; i++){
		Ent = fm_create_entity("info_target")

		if (!Ent) continue

		set_pev(Ent, pev_classname, "pokemod_ent")
		set_pev(Ent, pev_owner, id)

		IVecFVec(aim[id], f_aimvec)
		f_aimvec[0] += random_float(-100.0,100.0)
		f_aimvec[1] += random_float(-100.0,100.0)

		fm_entity_set_origin(Ent, f_aimvec)

		ChangeToBoulder(Ent, skill)
		set_pev(Ent, pev_velocity, Float:{0.0,0.0,1000.0})

		set_task(0.1, "CheckBoulder", Ent)
	}
	return SKILL_USED
 }
 public CheckBoulder(ent)
 {
	if( pev_valid(ent) ){
		new Float:velocity[3]
		pev(ent, pev_velocity, velocity)
		if(velocity[2]<1 || velocity[0] || velocity[1]){
			poke_sound(ent,CHAN_VOICE, SND_TU_FIRE)
			set_pev(ent, pev_velocity, Float:{0.0,0.0,-50.0} )
			set_pev(ent, pev_classname, "poke_rock")
			fm_entity_set_model(ent, MODELS[MDL_ROCK])
		}
		else
			set_task(0.1, "CheckBoulder", ent)
	}
 }
 ChangeToBoulder(Ent, skill)
 {
	set_pev(Ent, pev_iuser2, skill)

	set_pev(Ent, pev_mins, Float:{-10.0, -10.0, -10.0})
	set_pev(Ent, pev_maxs, Float:{10.0, 10.0, 10.0})

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_TOSS)

	return SKILL_USED
 }
 public Stone_Edge(id, skill, prop)
 {
	new damage, victim = PokeSkillTarget(id, damage);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && 	MaxSkillChance(id,skill,damage) ){
		screenshake(victim, 100, 3.0, 100);
		poke_print(id,print_center,"You hit someone with stone edge");
		poke_print(victim,print_center,"You were hit by a stone edge");
		return SKILL_USED;
	}
	return SKILL_READY;
 }

