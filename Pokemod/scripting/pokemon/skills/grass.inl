
 public Solar_Beam(id, skill, prop)
 {
	if( is_user_outside(id) ){
		new level = SkillLevel(id, skill)
		new origin[3], aimvec[3], Float:f_origin[3], Float:f_aimvec[3]

		get_user_origin(id, origin)
		get_user_origin(id, aimvec, 2)
		IVecFVec(origin, f_origin)
		IVecFVec(aimvec, f_aimvec)

		skillLinedamage(id, f_origin, f_aimvec, level/2, level, skill)
		MakeLine(0,origin,aimvec,SPRITE_INDEX[SPR_ZBEAM6],1,1,1,level,0,255,255,255,255,0)
		poke_sound(id,CHAN_VOICE, SND_SUCK)

		if( end_rep_use(id, skill, 1.0) )
			return SKILL_USED;
	}
	else{
		poke_print(id, print_center, "You need to be outside to use %s", SkillToName(skill) )
	}
	return SKILL_READY;
 }
 public Leech_Seed(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);

	static Leeching[MAX_PLAYERS];
	if( prop & SI_SPAWN ){
		Leeching[id] = 0;
		return SKILL_READY;
	}
	else if( prop & SI_SKILLCALL ){
		if( Leeching[id] ) return SKILL_USED;
		return SKILL_READY;
	}

	if( ValidPlayer(victim) && Pokemod_Alive(victim) && (!Pokemod_FF(id, victim) || FFon() ) ){
		new parm[3];
		parm[0] = id;
		parm[1] = victim;
		parm[2] = skill;
		leech_seed_loop( parm );
		Leeching[id]++;
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public leech_seed_loop(parm[])
 {
	new id = parm[0], victim = parm[1], skill = parm[2];

	if( !Pokemod_Alive(id) || !Pokemod_Alive(victim) )
		return;
	if( Leech_Seed(id, skill, SI_SKILLCALL) == SKILL_READY )
		return;

	new aName[32], iName[32], dmg = LEECH_SEED;

	AddLvl(id, dmg)

	sound_hit(id)
	sound_hit(victim)

	MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ROPE],1,1,10,5,0,0,255,0,100,0)

	get_user_name(id, iName, 31)
	get_user_name(victim, aName, 31)

	poke_print(id,print_center,"You have leeched %s", aName)
	poke_print(victim,print_center,"You have been leeched by %s", iName)

	screenflash(id, 0, 255, 0, 50)
	screenflash(victim, 0, 255, 0, 50)

	Pokemod_Heal(id, skilldamage(victim, id, dmg, skill) );

	new Float:tasktime = LEECH_SEED_TIME - f_SkillPercent(id, skill, LEECH_SEED_TIME/2)

	set_task(tasktime, "leech_seed_loop",0, parm, 3)
 }
 public Vine_Whip(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);

	if( ValidPlayer(victim) && Pokemod_Alive(victim) && (!Pokemod_FF(id, victim) || FFon()) ){
		new Float:velocity[3]
		new Float:jump = VINE_WHIP_JUMP
		new whipdamage = VINE_WHIP_DAMAGE

		f_AddLvl(id, jump)
		AddLvl(id, whipdamage)

		skilldamage( victim, id, whipdamage, skill)

		sound_hit(id)
		sound_hit(victim)

		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ROPE],1,1,10,10,10,0,255,0,100,0)

		pev(id, pev_velocity, velocity)

		if( !(PokeInfo[Pokemon[victim]] & PI_HEAVY) )
			velocity[2] += jump

		set_pev(victim, pev_velocity, velocity)
		set_pev(victim, pev_gaitsequence, 6)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Absorb(id, skill, prop)
 {
	new victim = PokeSkillTarget(id)

	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) )
		return SKILL_READY;

	new armor = get_user_armor(victim)
	new level = SkillLevel(id, skill)
	new totake = (armor*level)/(TotalLevels+1)

	if(level<armor)
		clamp(totake, level, armor)
	else
		totake = armor

	Pokemod_Repair(id, totake)
	set_user_armor(victim, armor-totake)

	sound_hit(id)
	sound_hit(victim)

	MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ROPE],1,1,10,5,10,0,255,200,255,0)
	return SKILL_USED;
 }
 public Razor_Leaf(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new aimvec[3], Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id])
	get_user_origin(id, aimvec, 2)
	IVecFVec(origin[id], f_origin)
	IVecFVec(aimvec, f_aimvec)

	skillLinedamage(id, f_origin, f_aimvec, level/3, level*RAZOR_LEAF_FACTOR, skill)

	new sprites = get_distance(origin[id], aimvec)/75

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)	//message begin
	write_byte(15)
	write_coord(origin[id][0])		// start position
	write_coord(origin[id][1])
	write_coord(origin[id][2])
	write_coord(aimvec[0])	// end position
	write_coord(aimvec[1])
	write_coord(aimvec[2])
	write_short(SPRITE_INDEX[SPR_BIGSPIT])		// sprite index
	write_byte(sprites)		// count
	write_byte(1)		// life in 0.1's
	write_byte(level/15)		// scale in 0.1's
	write_byte(5)		// velocity along vector in 10's
	write_byte(5)	// randomness of velocity in 10's
	message_end()

	poke_sound(id,CHAN_VOICE, SND_STEP);

	if( end_rep_use(id, skill, 0.5) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Mega_Drain(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) || knife_weapon(weapon) || grenade_weapon(weapon) )
		return SKILL_READY;
	new vweapon = get_user_weapon(victim);
	if( knife_weapon(vweapon) || grenade_weapon(vweapon) )
		return SKILL_READY;

	if( SkillChance(id,skill) ){

		new wpn[32], ammo;
		new vwpn[32], vammo;
		get_user_weapon(id, ammo);
		get_weaponname(weapon,wpn,31);
		get_user_weapon(victim, vammo);
		get_weaponname(vweapon,vwpn,31);

		#if MOD==DOD
			ammo = dod_get_user_ammo(id, weapon);
			vammo = dod_get_user_ammo(victim, vweapon);
		#endif

		//this makes top level drain half ammo
		new drainlow = (vammo*MEGA_DRAIN_LOW)/100;
		//and bottom level drain full ammo
		new drainhigh = (vammo*MEGA_DRAIN_HIGH)/100;
		new drain = SkillClamp(id, skill, drainlow, drainhigh);
		new newvammo = clamp( vammo-drain, 0, vammo );

		#if MOD==CS
			new iWPNidx = -1;
			while( (iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) ){
				if( id == pev(iWPNidx, pev_owner) ){
					cs_set_weapon_ammo(iWPNidx, ammo+drain);
				}
			}
			iWPNidx = -1;
			while( (iWPNidx = fm_find_ent_by_class(iWPNidx, vwpn)) ){
				if( victim == pev(iWPNidx, pev_owner) ){
					cs_set_weapon_ammo(iWPNidx, newvammo);
				}
			}
		#endif
		#if MOD==DOD
			dod_set_user_ammo(id, weapon, ammo+drain);
			dod_set_user_ammo(victim, vweapon, newvammo);
		#endif
		#if MOD==NS
			new iWPNidx = -1;
			while( (iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) ){
				if( id == pev(iWPNidx, pev_owner) ){
					ns_set_weap_clip(iWPNidx, ammo+drain);
				}
			}
			iWPNidx = -1;
			while( (iWPNidx = fm_find_ent_by_class(iWPNidx, vwpn)) ){
				if( victim == pev(iWPNidx, pev_owner) ){
					ns_set_weap_clip(iWPNidx, newvammo);
				}
			}
		#endif

		sound_hit(id);
		sound_hit(victim);
		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_XPLODE],1,1,5,40,99,150,150,150,100,0);

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Spore(id, skill, prop)
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

	IVecFVec(aim[id], f_aimvec);

	new ent = fm_create_entity("info_target");
	set_pev( ent, pev_classname, "pokemod_ent");

	fm_entity_set_model(ent, MODELS[MDL_FUNGUS]);
	set_pev( ent, pev_mins, Float:{-50.0, -50.0, -50.0});
	set_pev( ent, pev_maxs, Float:{50.0, 50.0, 50.0});

	fm_entity_set_origin(ent, f_aimvec);

	#if SPORE_SOLID == 1
	set_pev( ent, pev_solid, SOLID_BBOX);
	#else
	set_pev( ent, pev_solid, SOLID_NOT);
	#endif

	set_pev( ent, pev_movetype, MOVETYPE_FLY);

	set_pev( ent, pev_rendermode, 5);
	set_pev( ent, pev_renderamt, 255.0);
	set_pev( ent, pev_owner, id);
	fm_set_rendering( ent, kRenderFxGlowShell, 255, 0, 255, kRenderNormal, 20);

	set_pev( ent, pev_takedamage, 1.0);
	set_pev( ent, pev_health, POKE_HP_OFFSET+50.0+SkillLevel(id, skill));

	new parm[4]
	parm[0] = ent;
	parm[1] = skill;
	parm[2] = SPORE_TIME * 10;
	parm[3] = 1;
	AddLvl(id, parm[2]);

	check_spore(parm)

	return SKILL_USED;
 }
 public Stun_Spore(id, skill, prop)
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

	IVecFVec(aim[id], f_aimvec);

	new ent = fm_create_entity("info_target");
	set_pev( ent, pev_classname, "pokemod_ent");

	fm_entity_set_model(ent, MODELS[MDL_FUNGUS]);
	set_pev( ent, pev_mins, Float:{-50.0, -50.0, -50.0});
	set_pev( ent, pev_maxs, Float:{50.0, 50.0, 50.0});

	fm_entity_set_origin(ent, f_aimvec);

	#if SPORE_SOLID == 1
	set_pev( ent, pev_solid, SOLID_BBOX);
	#else
	set_pev( ent, pev_solid, SOLID_NOT);
	#endif

	set_pev( ent, pev_movetype, MOVETYPE_FLY);

	set_pev( ent, pev_rendermode, 5);
	set_pev( ent, pev_renderamt, 255.0);
	set_pev( ent, pev_owner, id);

	set_pev( ent, pev_takedamage, 1.0);
	set_pev( ent, pev_health, POKE_HP_OFFSET+50.0+SkillLevel(id, skill));

	new parm[4]
	parm[0] = ent;
	parm[1] = skill;
	parm[2] = SPORE_TIME * 10;
	parm[3] = 0;
	AddLvl(id, parm[2]);

	check_spore(parm)

	return SKILL_USED;
 }
 public Aromatherapy(id, skill, prop)
 {
	if( HasStatus(id) ){
		ResetStatus(id);
		poke_print(id, print_center, "Aromatherapy has prevented a status");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Synthesis(id, skill, prop)
 {
	new heal

	sound_cure(id)
	screenflash(id, 0, 255, 75, 100)

	heal = SkillClamp(id, skill, 25, 150)
	Pokemod_Heal(id, heal)

	return SKILL_USED;
 }
 public Leaf_Storm(id, skill, prop)
 {
	new level = SkillLevel(id, skill);
	new Float:f_origin[3], Float:f_aimvec[3];

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, floatround(level*LEAF_STORM_RADIUS), floatround(level*LEAF_STORM_DAMAGE), skill, _, STATUS_POISON, -1);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_BIGSPIT],1,1,1,10+level,0,0,255,80,255,0);
	origin[id][2] -= 5;
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_BIGSPIT],1,1,1,10+level,0,0,255,0,255,0);

	poke_sound(id,CHAN_VOICE, SND_STEP);

	if( end_rep_use(id, skill, 2.0) )
		return SKILL_USED;

	return SKILL_READY;
 }
 public Energy_Ball(id, skill, prop)
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
		write_short(SPRITE_INDEX[SPR_TRAIL])	// sprite index
		write_byte(10)	// life in 0.1's
		write_byte(10)		// line width in 0.1's
		write_byte(40)	//r,g,b
		write_byte(255)
		write_byte(0)
		write_byte(200)	// brightness
		message_end()

		//Glow the entity green
		fm_set_rendering(Ent, kRenderFxGlowShell, 50, 255, 0, kRenderNormal, 100)
	}
	return SKILL_USED;
 }
 public Giga_Drain(id, skill, prop)
 {
	new damage;
	new victim = PokeSkillTarget(id, damage);

	new heal = floatround( f_SkillClamp(id, skill, damage*0.2, damage*0.5 ) );
	Pokemod_Heal(id, heal);
	screenflash(id, 0, 255, 0, 50)
	screenflash(victim, 0, 255, 0, 50)

	return SKILL_USED;
 }

