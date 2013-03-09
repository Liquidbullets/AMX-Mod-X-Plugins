
 public Disable(id, skill, prop)
 {
	new aid = PokeSkillTarget(id);

	if( ValidPlayer(aid) && Pokemod_Alive(aid) && !Pokemod_FF(id, aid) ){
		bind_used(aid,_,DISABLE_TIME)
		sound_cast(id)
		sound_cast(aid)
		set_pev(aid, pev_gaitsequence, 6)
		poke_print(id, print_center, "You disabled somebodies pokeskill")
		poke_print(aid, print_center, "Your pokeskill has been disabled")
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Spike_Cannon(id, skill, prop)
 {
	static bool:Firing[MAX_PLAYERS];
	if( prop & SI_RELEASE || prop & SI_RESET ){
		Firing[id] = false;
		return SKILL_READY;
	}
	else if( prop & SI_BIND ){
		if( !Firing[id] ){
			Firing[id] = true;
			CallSkill(id, skill, SI_SKILLCALL);
		}
		return SKILL_READY;
	}
	else if( prop & SI_SKILLCALL ){
		if( !Firing[id] )
			return SKILL_READY;
	}

	new Float:origin[3], Float:aim[3], Float:velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	//move it a little in front of them
	velocity_by_aim(id, 20, velocity)
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];

	Ent = fm_create_entity("info_target")
	if(!Ent) return SKILL_READY;

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, SPIKE_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_HAIR])

	new Float:MinBox[3] = {-5.0, -5.0, -5.0}
	new Float:MaxBox[3] = {5.0, 5.0, 5.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	fm_entity_set_origin(Ent, origin)

	aim[0] += 270;
	while( aim[0] < -90.0 )
		aim[0] += 180.0;
	while( aim[0] > 90.0 )
		aim[0] -= 180.0;
	aim[0] *= -1;

	if( aim[0] > 0.0 )
		aim[2] = 180.0;

	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_BOUNCEMISSILE)
	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, 2000 , velocity)
	set_pev(Ent, pev_velocity, velocity)

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)

	DelayedCallSkill(0.2, id, skill, SI_SKILLCALL)

	return SKILL_USED;
 }
 public Roar(id, skill, prop)
 {
	poke_sound(id, CHAN_VOICE, SND_AG_DIE)

	#if MOD==NS
		new Float:energy
	#endif

	new ammo, bpammo, wpnid
	new players[MAX_PLAYERS], pnum, aid, Float:totake
	poke_get_players(players, pnum, PLAYER_ALIVE)
	#if MOD==CS || MOD==NS
		new iWPNidx, wpn[32]
	#endif

	for ( new i = 0; i < pnum; i++ )
	{
		aid = players[i]
		if( Pokemod_FF(id, aid) ) continue;

		#if MOD==CS
			wpnid = get_user_weapon(aid, ammo, bpammo)
			if(!wpnid)
				continue

			//can't do anything with these weapons
			if(knife_weapon(wpnid) || grenade_weapon(wpnid) || wpnid==CSW_C4)
				continue

			get_weaponname(wpnid,wpn,31)
			totake = float(SkillPercent(id, skill, ammo))

			iWPNidx = -1
			while ((iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) != 0) {
				if (aid == pev(iWPNidx, pev_owner)) {
					cs_set_weapon_ammo(iWPNidx, floatround(ammo-totake))
					break
				}
			}
			cs_set_user_bpammo(aid, wpnid, floatround(bpammo+totake))
		#endif
		#if MOD==DOD
			wpnid = get_user_weapon(aid,ammo,bpammo)
			if(!wpnid)
				continue
			if(knife_weapon(wpnid) || grenade_weapon(wpnid))
				continue
			ammo = dod_get_user_ammo(aid,wpnid)
			totake = float(SkillPercent(id, skill, ammo))
			dod_set_user_ammo(aid, wpnid, floatround(ammo-totake))
		#endif
		#if MOD==NS
			if(Pokemod_Team(aid)==1){//Marines
				wpnid = get_user_weapon(aid, ammo, bpammo)
				if(!wpnid)
					continue
				get_weaponname(wpnid,wpn,31)

				//can't do anything with these weapons
				if(knife_weapon(wpnid) || grenade_weapon(wpnid))
					continue

				totake = float(SkillPercent(id, skill, ammo))

				iWPNidx = -1
				while ((iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) != 0) {
					if (aid == pev(iWPNidx, pev_owner)) {
						ns_set_weap_clip(iWPNidx, floatround(ammo-totake))
						break
					}
				}
				ns_set_weap_reserve(aid, wpnid, floatround(bpammo+totake))
			}
			else{//Aliens
				energy = ns_get_energy(aid)
				totake = (energy*SkillLevel(id, skill))*0.01
				ns_set_energy(aid, energy-totake)
			}
		#endif
		poke_print(aid, print_center, "Someone roared at you")
		poke_sound(aid, CHAN_VOICE, SND_AG_DIE)
	}
	return SKILL_USED;
 }
 public Smokescreen(id, skill, prop)
 {
	new smokecount = 1

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
	parm[6] = SPRITE_INDEX[SPR_BALLSMOKE]

	poke_sound(id, CHAN_VOICE, SND_FLAME)

	create_sprite_spray( parm )
	if( smokecount-1 )
		set_task(0.1, "create_sprite_spray", 0, parm, 7, "a", smokecount-1)

	parm[0] = id
	parm[1] = 0
	parm[2] = skill
	spray_check( parm )
	if( smokecount-1 )
		set_task(0.1, "spray_check", 0, parm, 3, "a", smokecount-1)

	if( end_rep_use(id, skill, 3.0) )
		return SKILL_USED;

	return SKILL_READY
 }
 public Sand_Attack(id, skill, prop)
 {
	new smokecount = 1

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
	parm[6] = SPRITE_INDEX[SPR_XPLODE]

	poke_sound(id, CHAN_VOICE, SND_JUMPLAND)

	create_sprite_spray( parm )
	if( smokecount-1 )
		set_task(0.1, "create_sprite_spray", 0, parm, 7, "a", smokecount-1)

	parm[0] = id
	parm[1] = 2
	parm[2] = skill
	spray_check( parm )
	if( smokecount-1 )
		set_task(0.1, "spray_check", 0, parm, 3, "a", smokecount-1)

	if( end_rep_use(id, skill, 5.0) )
		return SKILL_USED;

	return SKILL_READY
 }
 public Recover(id, skill, prop)
 {
	new heal

	sound_cure(id)
	screenflash(id, 128, 0, 128, 50)

	heal = SkillClamp(id, skill, 25, 150)
	Pokemod_Heal(id, heal)

	return SKILL_USED;
 }
 public Pay_Day(id, skill, prop)
 {
	new damage;
	PokeSkillTarget(id, damage);

	AddLvl(id, damage);
	Pokemod_Pay(id, damage);

	return SKILL_USED;
 }
 public Swift(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && PassTest(id, 1) ){
		get_user_origin(id, origin[id]);
		get_user_origin(victim, aim[id]);
		set_user_origin(id, aim[id]);

		MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_ROPE],0,0,10,15,0,255,255,255,255,0)

		sound_cast(id)
		poke_UnStuck(id)

		return SKILL_USED;
	}
	else{
		sound_deny(id)
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public Scratch(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		set_pev(victim, pev_velocity, Float:{0.0,0.0,0.0});
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Fury_Attack(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		new edamage = (PlayerStat[id][STAT_MAXAP] - get_user_armor(id)) / FURY_ATTACK_FACTOR
		poke_print(id,print_center,"%s did %d extra damage", SkillToName(skill), skilldamage( victim, id, edamage, skill) );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Fury_Swipes(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		new edamage = (PlayerStat[id][STAT_MAXHP] - Health(id)) / FURY_SWIPES_FACTOR
		poke_print(id,print_center,"%s did %d extra damage", SkillToName(skill), skilldamage( victim, id, edamage, skill) );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Vice_Grip(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		ApplyStatus( id, victim, STATUS_PARALYZE, VICE_GRIP );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Dizzy_Punch(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		skilldamage(victim, id, DIZZY_PUNCH, skill);
		if( ValidPlayer(victim) )
			screenshake(victim, DIZZY_PUNCH_DIZZY, 5.0, DIZZY_PUNCH_DIZZY);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Thrash(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		skilldamage(victim, id, THRASH_DAMAGE, skill);
		screenshake(victim, THRASH, 5.0, THRASH);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Slash(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		new Float:victim_origin[3];
		pev( victim, pev_origin, victim_origin );
		skillAoEdamage(id, victim_origin, SkillLevel(id, skill)*5+100, damage, skill, AOE_IGNORE|AOE_MAXDAM);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Egg_Bomb(id, skill, prop)
 {
	if( !PlayerEnt[id] ){
		if( !Find_EggBomb(id) ){
			give_grenade(id)
			return SKILL_USED;
		}
		g_StopSkill[id] = true;
	}
	else if( PlayerEnt[id]!=id && pev_valid(PlayerEnt[id]) ){
		//stop them from geting a nade immediately
		sound_cast(id)
		set_pev(PlayerEnt[id], pev_dmgtime, 0.1)
		set_pev(PlayerEnt[id], pev_nextthink, 0.1)
		set_pev(PlayerEnt[id], pev_owner, id)
		PlayerEnt[id] = 0
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 Find_EggBomb(id)
 {
	new iGrenade = -1, model[72]
	while ( (iGrenade = fm_find_ent_by_class(iGrenade, "grenade")) > 0 ) {
		pev(iGrenade, pev_model, model, 71)
		if ( id == pev(iGrenade, pev_owner) && !equali(model, MODELS[MDL_POKEBALL]) && !equali(model, "models/w_c4.mdl")){
			PlayerEnt[id] = iGrenade
			egg_bomb_change(id)
			return true;
		}
	}
	#if MOD==DOD
	iGrenade = -1
	while ( (iGrenade = fm_find_ent_by_class(iGrenade, "stickgrenade")) > 0 ) {
		pev(iGrenade, pev_model, model, 71)
		if ( id == pev(iGrenade, pev_owner) && !equali(model, MODELS[MDL_POKEBALL])){
			PlayerEnt[id] = iGrenade
			egg_bomb_change(id)
			return true;
		}
	}
	#endif
	#if MOD==NS
	iGrenade = -1
	while ( (iGrenade = fm_find_ent_by_class(iGrenade, "handgrenade")) > 0 ) {
		pev(iGrenade, pev_model, model, 71)
		if ( id == pev(iGrenade, pev_owner) && !equali(model, MODELS[MDL_POKEBALL])){
			PlayerEnt[id] = iGrenade
			egg_bomb_change(id)
			return true;
		}
	}
	#endif
	return false;
 }
 public egg_bomb_nade(id)
 {
	if( !PlayerEnt[id] )
		return
	if(!PMon() || !Pokemod_Alive(id)){
		set_pev(PlayerEnt[id], pev_dmgtime, 0.1)
		set_pev(PlayerEnt[id], pev_nextthink, 0.1)
		set_pev(PlayerEnt[id], pev_owner, id)
		PlayerEnt[id] = 0
	}
	else if( pev_valid(PlayerEnt[id]) ){
		new Float:timer
		pev(PlayerEnt[id], pev_nextthink, timer)
		timer += 2.0
		set_pev(PlayerEnt[id], pev_nextthink, timer)
		set_task(1.0, "egg_bomb_nade", id)
	}
 }
 egg_bomb_change(id)
 {
	new nade = PlayerEnt[id];

	fm_entity_set_model(nade, MODELS[MDL_POKEBALL])
	set_pev(nade, pev_solid, SOLID_BBOX)
	set_pev(nade, pev_owner, 0)
	fm_entity_set_size(nade,Float:{-5.0,-5.0,-5.0},Float:{5.0,5.0,5.0})

	egg_bomb_nade(id)
 }
 public Growth(id, skill, prop)
 {
	if( BindTime(id) ){
		new Float:reduce = f_SkillPercent(id, skill, GROWTH_REDUCE);
		BindUsed[id] -= reduce;
		poke_print(id, print_center, "%s is reducing your skill time", SkillToName(skill));
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Rage(id, skill, prop)
 {
	static rage[MAX_PLAYERS];
	if( prop & SI_ROUNDRESET )
		rage[id] = 0;

	new damage, victim = PokeSkillTarget(id, damage);
	if( prop & SI_DAMAGED || prop & SI_PDAMAGED ){
		new ragenums = damage / RAGE_FACTOR
		rage[id] += ragenums

		if(rage[id] > RAGE_MAX)
			rage[id] = RAGE_MAX

		poke_print(id,print_center,"Your rage is at %d extra damage", rage[id])
	}
	else if( rage[id] && Pokemod_Alive(victim) ){
		poke_print(id,print_center,"Your rage did %d extra damage", skilldamage( victim, id, rage[id], skill))
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 
 public Wrap(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) && SkillChance(id,skill) ){
		ApplyStatus( id, victim, STATUS_PARALYZE, 1.0 );
		poke_print( id, print_center, "You wrapped someone up" );
		poke_print( victim, print_center, "You have been wrapped" );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Constrict(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && MaxSkillChance(id,skill,50) && !g_Status[victim][STATUS_PARALYZE] ){
		ApplyStatus( id, victim, STATUS_PARALYZE, CONSTRICT_TIME )
		poke_print(id,print_center,"You constricted an enemy")
		poke_print(victim,print_center,"You have been constricted")
		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ROPE],1,1,5,20,0, 0,200,150, 100, 0)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Bide(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	new edamage = (PlayerStat[id][STAT_MAXHP] - Health(id)) / BIDE_FACTOR
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && edamage>0 && MaxSkillChance(id,skill,edamage) ){
		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ROPE],1,1,10,5,10, 139,59,19, 255,0)
		poke_print(id,print_center,"Your bide did %d extra damage", skilldamage( victim, id, edamage, skill) )
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Defense_Curl(id, skill, prop)
 {
	new damage;
	PokeSkillTarget(id, damage);
	Pokemod_Repair(id, damage / DEFENSE_CURL)
	return SKILL_USED
 }
 public Harden(id, skill, prop)
 {
	new armr = get_user_armor(id);
	if( !armr ){
		sound_deny(id);
		poke_print(id, print_center, "You need to have armor to swap");
		g_StopSkill[id] = true;
		return SKILL_READY;
	}

	new hp = Health(id);

	PlayerStat[id][STAT_MAXHP] = armr;
	set_user_health(id, PlayerStat[id][STAT_MAXHP]);
	set_user_armor(id, hp);
	return SKILL_USED;
 }
 public Flash(id, skill, prop)
 {
	new damage, victim = PokeSkillTarget(id, damage);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && MaxSkillChance(id,skill,damage) ){
		new alpha = SkillClamp(id, skill, FLASH_ALPHA_MIN, FLASH_ALPHA_MAX);
		screenflash(victim, 255,255,255, alpha, FLASH_TIME);
		poke_print(id,print_center,"You hit someone with flash");
		poke_print(victim,print_center,"You were hit by a flash of light");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Reflect(id, skill, prop)
 {
	new damage, weapon;
	PokeSkillTarget(id, damage, weapon);
	new max = damage
	if( SkillTrulyActive(id, skill) )
		max += REFLECT_ACTIVE
	if( MaxSkillChance(id,skill, max) && grenade_weapon(weapon) ){
		screenflash(id, 255, 0, 255, 50)
		Pokemod_Heal(id,damage)
		poke_print(id,print_center,"You reflected shrapnel from a grenade")
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Guillotine(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && MaxSkillChance(id,skill, damage) && knife_weapon(weapon) ){
		skilldamage( victim, id, AUTO_KILL, skill)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Glare(id, skill, prop)
 {
	new aid = PokeSkillTarget(id)
	if( ValidPlayer(aid) && Pokemod_Alive(aid) && !Pokemod_FF(id, aid) ){
		poke_print(id, print_center, "You Glared at someone")
		poke_print(aid, print_center, "Someone Glared at you")
		ApplyStatus( id, aid, STATUS_PARALYZE, 1.0 )
		set_pev(aid, pev_velocity, Float:{0.0,0.0,0.0} );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Bite(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !ValidPlayer(victim) )
		return SKILL_READY;
	poke_print(victim, print_center, "Someone is biting you");
	new damage = skilldamage(victim, id, BITE, skill);
	if( !damage )
		return SKILL_READY;
	Pokemod_Heal(id, damage );
	return SKILL_USED;
 }
 public Hyper_Fang(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	new div = PlayerStat[victim][STAT_MAXHP] / HYPER_FANG
	if( Pokemod_Alive(victim) && damage < div && knife_weapon( weapon ) ){
		skilldamage(victim, id, div-damage, skill);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Super_Fang(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	new div = PlayerStat[victim][STAT_MAXHP] / SUPER_FANG
	if( Pokemod_Alive(victim) && damage < div && knife_weapon( weapon ) ){
		skilldamage(victim, id, div-damage, skill)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Double_Edge(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if(Pokemod_Alive(victim) || knife_weapon(weapon))
		return SKILL_READY;

	new amount
	new level = SkillLevel(id, skill)

	if( SkillTrulyActive(id, skill) )
		level *= 2

	#if MOD==CS
		amount = cs_get_user_bpammo(id, weapon)
		if(amount==0 && grenade_weapon(weapon)){
			give_grenade(id)
			amount = 1
		}
		cs_set_user_bpammo(id, weapon, amount+level)
	#endif
	#if MOD==DOD
		amount = dod_get_user_ammo(id, weapon)
		if(amount==0 && grenade_weapon(weapon)){
			give_grenade(id)
			amount = 1
		}
		dod_set_user_ammo(id, weapon, amount+level)
	#endif
	#if MOD==NS
		amount = ns_get_weap_reserve(id, weapon)
		if(amount==0 && grenade_weapon(weapon)){
			give_grenade(id)
			amount = 1
		}
		ns_set_weap_reserve(id, weapon, amount+level)
	#endif

	return SKILL_USED;
 }
 public Tri_Attack(id, skill, prop)
 {
	new weapon;
	PokeSkillTarget(id, _, weapon);
	if(knife_weapon(weapon)) return SKILL_READY;

	new wpn[32], ammo, bpammo
	get_user_weapon(id, ammo, bpammo)
	get_weaponname(weapon,wpn,31)

	new addition = 1
	if( SkillTrulyActive(id, skill) )
		addition++

	if( SkillChance(id,skill) ){

		//can't do anything with these weapons
		if(grenade_weapon(weapon)){
			new parm[3];
			parm[0] = id;
			parm[1] = addition;
			parm[2] = 1;
			set_task(0.1, "delayed_grenade", 0, parm, 3);
			return SKILL_USED;
		}
		#if MOD==CS
			new iWPNidx = -1
			while ((iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) != 0) {
				if (id == pev(iWPNidx, pev_owner)) {
					cs_set_weapon_ammo(iWPNidx, ammo+addition)
				}
			}
		#endif
		#if MOD==DOD
			new amount = dod_get_user_ammo(aid, weapon)
			dod_set_user_ammo(id, weapon, amount+addition)
		#endif
		#if MOD==NS
			new iWPNidx = -1
			while ((iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) != 0) {
				if (id == pev(iWPNidx, pev_owner)) {
					ns_set_weap_clip(iWPNidx, ammo+addition)
				}
			}
		#endif

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Splash(id, skill, prop)
 {
	if( entity_on_ground(id) ){
		InWater(id);
		poke_sound(id, CHAN_VOICE, SND_WADE);
		new Float:velo[3];
		pev(id, pev_velocity, velo);
		velo[0] = velo[1] = 0.0;
		set_pev(id, pev_velocity, velo );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Substitute(id, skill, prop)
 {
	if( Health(id) < PlayerStat[id][STAT_MAXHP]/SUBSTITUTE){
		EscapeRope(id, 0)
		return SKILL_USED;
	}
	else if( prop & SI_BIND )
		poke_print(id, print_center, "Your HP is not low enough (%d)",PlayerStat[id][STAT_MAXHP]/SUBSTITUTE)
	return SKILL_READY;
 }
 public Gust(id, skill, prop)
 {
	new gust = SkillLevel(id, skill) / GUST_FACTOR
	new Float:velocity[3], Float:Cvelocity[3]

	poke_sound(id, CHAN_VOICE, SND_JUMPLAND)

	velocity_by_aim(id, gust*GUST_VELOCITY, velocity)

	new players[MAX_PLAYERS], pnum, aid
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	for(new i=0; i<pnum; i++){
		aid = players[i]
		if(fm_is_ent_visible(id, aid)){
			pev(aid, pev_velocity, Cvelocity)
			Cvelocity[0] += velocity[0]
			Cvelocity[1] += velocity[1]
			Cvelocity[2] += velocity[2]
			set_pev(aid, pev_velocity ,Cvelocity)

			if( id != aid ) skilldamage(aid, id, gust, skill)

			poke_print(aid, print_center, "You were hit by a %s", SkillToName(skill) )
			poke_sound(aid, CHAN_VOICE, SND_JUMPLAND)
		}
	}
	return SKILL_USED;
 }
 public Whirlwind(id, skill, prop)
 {
	new whirlwind = SkillLevel(id, skill) / WHIRLWIND_FACTOR
	new Float:velocity[3], Float:Cvelocity[3]

	poke_sound(id, CHAN_VOICE, SND_JUMPLAND)

	velocity_by_aim(id, whirlwind*WHIRLWIND_VELOCITY, velocity)

	new players[MAX_PLAYERS], pnum, aid
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)

	for(new i=0; i<pnum; i++){
		aid = players[i]
		if(fm_is_ent_visible(id, aid)){
			pev(aid, pev_velocity, Cvelocity)
			Cvelocity[0] += velocity[0]
			Cvelocity[1] += velocity[1]
			Cvelocity[2] += velocity[2]
			set_pev(aid, pev_velocity ,Cvelocity)

			if( id != aid ) skilldamage(aid, id, whirlwind, skill)

			poke_print(aid, print_center, "You were hit by a %s", SkillToName(skill) )
			poke_sound(aid, CHAN_VOICE, SND_JUMPLAND)
		}
	}
	return SKILL_USED;
 }
 public Double_Slap(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) )
		return SKILL_READY;

	new slap = SLAP * SkillLevel(id, skill)
	new damage = slap/DOUBLE_SLAP

	if(damage<1)
		damage = 1

	new Float:velocity[3]
	velocity[0] = random_float(-1.0*slap, 1.0*slap)
	velocity[1] = random_float(-1.0*slap, 1.0*slap)
	velocity[2] = random_float(-1.0*slap, 1.0*slap)

	set_pev(victim, pev_velocity, velocity)
	skilldamage(victim, id, damage, skill)
	return SKILL_USED;
 }
 public Skullbash(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) )
		return SKILL_READY;

	new slap = SLAP * SkillLevel(id, skill)
	new damage = slap/SKULLBASH

	if(damage<1)
		damage = 1

	new Float:velocity[3]
	velocity[0] = random_float(-1.0*slap, 1.0*slap)
	velocity[1] = random_float(-1.0*slap, 1.0*slap)
	velocity[2] = random_float(-1.0*slap, 1.0*slap)

	set_pev(victim, pev_velocity, velocity)
	skilldamage(victim, id, damage, skill)
	return SKILL_USED;
 }
 public Body_Slam(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) )
		return SKILL_READY;

	new slap = SLAP * SkillLevel(id, skill)
	new damage = slap/BODY_SLAM

	if(damage<1)
		damage = 1

	if( ValidPlayer(victim) ){
		new Float:velocity[3]
		velocity[0] = random_float(-1.0*slap, 1.0*slap)
		velocity[1] = random_float(-1.0*slap, 1.0*slap)
		velocity[2] = random_float(-1.0*slap, 1.0*slap)

		set_pev(victim, pev_velocity, velocity)
	}

	skilldamage(victim, id, damage, skill)

	return SKILL_USED;
 }
 public Mega_Punch(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !fm_is_ent_visible(id, victim) )
		return SKILL_READY;

	if( ValidPlayer(victim) ){
		new Float:velocity[3];
		velocity_by_aim(id, MEGA_PUNCH, velocity);
		velocity[2] = floatmax(velocity[2], 50.0);

		set_pev(victim, pev_velocity, velocity);
	}
	skilldamage(victim, id, MEGA_PUNCH_DAMAGE, skill);
	return SKILL_USED;
 }
 public Mega_Kick(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !fm_is_ent_visible(id, victim) )
		return SKILL_READY;

	if( ValidPlayer(victim) ){
		new Float:velocity[3];
		velocity_by_aim(id, MEGA_KICK, velocity);
		velocity[2] = floatmax(velocity[2], float(MEGA_KICK_UP));

		set_pev(victim, pev_velocity, velocity);
	}
	skilldamage(victim, id, MEGA_KICK_DAMAGE, skill);
	return SKILL_USED;
 }
 public Double_Team(id, skill, prop)
 {
	if(!PassTest(id, 0))
		return SKILL_READY;

	new aid, Float:lastpercent = 1.00, lastid = 0, Float:percentage
	new players[MAX_PLAYERS], pnum
	poke_get_players(players, pnum, PLAYER_ALIVE|PLAYER_MORTAL)
	for(new i=0; i<pnum; i++){
		aid = players[i]
		if( Pokemod_FF(id, aid) != 1 )	//don't want to know about them
			continue

		percentage = float(Health(aid))/float(PlayerStat[aid][STAT_MAXHP])
		if(percentage<lastpercent){
			lastpercent = percentage
			lastid = aid
		}
	}
	if(Pokemod_Alive(lastid)){
		new Float:origin[3]
		pev(lastid, pev_origin, origin)
		origin[2] += 40	//set them above the player
		set_pev(id, pev_origin, origin)
		pev(lastid, pev_angles, origin)
		set_pev(id, pev_angles, origin)
		set_pev(id, pev_fixangle, 1)

		poke_UnStuck(id)
		sound_cast(id)

		new name[32]
		get_user_name(lastid, name, 31)
		poke_print(id, print_center, "You are double teaming with %s",name)
		return SKILL_USED;
	}
	else{
		sound_deny(id)
		poke_print(id, print_center, "You have no injured teammates")
		g_StopSkill[id] = true;
	}
	return SKILL_READY;
 }
 public Quick_Attack(id, skill, prop)
 {
	new Float:velocity[3], Float:add_to[3]
	pev(id, pev_velocity, velocity)
	velocity_by_aim(id, QUICK_ATTACK+SkillLevel(id, skill), add_to)
	velocity[0] += add_to[0]
	velocity[1] += add_to[1]

	if( entity_on_ground(id) )
		velocity[2] = add_to[2]		//add some just so they get a little boost into the air
	else
		velocity[2] += floatclamp(add_to[2],0.0,40.0)	//if theyre in the air, dont force them upward
	set_pev(id, pev_velocity, velocity)

	if( end_rep_use(id, skill, 5.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Hyper_Beam(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3]

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, level*2, level, skill)
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_ZBEAM6],1,1,1,level*20,0,255,255,255,255,0)
	poke_sound(id, CHAN_VOICE, SND_AG_DIE)
	set_pev(id, pev_velocity, Float:{0.0,0.0,0.0})

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Conversion(id, skill, prop)
 {
	if( prop & SI_RELEASE ){
		if( Pokemod_Alive(id) )
			fm_set_rendering(id);
		SetSpeed(id);
		return SKILL_READY;
	}
	if( prop & SI_SPEED ){
		if( ActiveSkill(id) == skill && prop & SI_SKILLCALL ){
			new slowdown = SkillClamp(id, skill, CONVERSION_SPEED_MIN, CONVERSION_SPEED_MAX)
			PlayerStat[id][STAT_SPEED] += slowdown;
		}
		return SKILL_READY;
	}

	fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, CONVERSION_ALPHA);
	screenflash(id, 128, 128, 128, 128);
	SetSpeed(id, true);

	if( end_rep_use(id, skill, 5.0+SkillPercent(id, skill, CONVERSION_TIME)) ){
		Conversion(id, skill, SI_RELEASE);
		return SKILL_USED;
	}

	return SKILL_READY;
 }
 public Strength(id, skill, prop)
 {
	if( prop & SI_RELEASE ){
		if( ValidPlayer(PlayerEnt[id]) && Pokemod_Alive(PlayerEnt[id]) )
			poke_UnStuck(PlayerEnt[id], false);
		PlayerEnt[id] = 0;
		return SKILL_READY;
	}

	new ent, body;
	get_user_aiming(id, ent, body, STRENGTH_DIST);
	if(!pev_valid(PlayerEnt[id])){
		if(pev_valid(ent)){
			new class[32];
			pev(ent, pev_classname, class, 31);

			#if STRENGTH_NPC == 0
			if( equal(class, "npc", 3) ){
				poke_print(id, print_center, "You cannot move this object");
				return SKILL_READY;
			}
			#endif

			if(!equal(class, "worldspawn")
			&& containi(class, "grenade") == -1
			&& !equal(class, "func_", 5)){
				if( ValidPlayer(ent) && PokeInfo[Pokemon[ent]] & PI_HEAVY ){
					poke_print(id, print_center, "%s is too heavy to pick up",PokeToName(Pokemon[ent]));
					return SKILL_READY;
				}
				PlayerEnt[id] = ent;
			}
			else{
				return SKILL_READY;
			}
		}
		else
			return SKILL_READY;
	}

	new Float:player_origin[3];
	new Float:ent_origin[3], Float:velocity[3];

	pev(id, pev_origin, player_origin);
	ent_origin[0] = player_origin[0];
	ent_origin[1] = player_origin[1];
	ent_origin[2] = player_origin[2];
	velocity_by_aim(id, STRENGTH_DIST/2, velocity);

	ent_origin[0] += velocity[0];
	ent_origin[1] += velocity[1];
	ent_origin[2] += velocity[2];

	engfunc(EngFunc_SetOrigin, PlayerEnt[id], ent_origin);

	#if STRENGTH_DAMAGE > 0
	if( ValidPlayer(PlayerEnt[id]) && Pokemod_Alive(PlayerEnt[id]) )
		skilldamage(PlayerEnt[id], id, STRENGTH_DAMAGE, skill);
	#endif
	return SKILL_READY;
 }
 public Supersonic(id, skill, prop)
 {
	new players[32], pnum, aid, apple_count
	new bool:in_water = entity_in_water(id);

	poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL )
	for( new i=0; i<pnum; i++)
	{
		aid = players[i];
		if( Pokemod_FF(id, aid) ) continue;

		if( (in_water && entity_in_water(aid))
		|| fm_is_ent_visible(id, aid) ){
			pev(aid, pev_origin, confuse[aid]);
			set_task(1.0, "Confuse", aid);
			apple_count++;
		}
	}
	if( ActionPrint(id, print_center, apple_count, "confused") ){
		sound_cast(id);
		return SKILL_USED;
	}
	else{
		g_StopSkill[id] = true;
		sound_deny(id);
	}
	return SKILL_READY;
 }
 public Tackle(id, skill, prop)
 {
	if( entity_on_ground(id) ){
		new Float:velocity[3], kick = 250+SkillLevel(id, skill)/2;

		velocity_by_aim(id, kick, velocity);
		velocity[2] = floatmax(velocity[2], float(kick));

		set_pev(id, pev_velocity, velocity);
		poke_sound(id,CHAN_VOICE, SND_STEP);
		set_pev(id, pev_gaitsequence, 6);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Double_Kick(id, skill, prop)
 {
	if( entity_on_ground(id) ){
		new Float:velocity[3], kick = (250+SkillLevel(id, skill)/2)*2;

		velocity_by_aim(id, kick, velocity);
		velocity[2] = floatmax(velocity[2], float(kick));

		set_pev(id, pev_velocity, velocity);
		poke_sound(id,CHAN_VOICE, SND_STEP);
		set_pev(id, pev_gaitsequence, 6);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Tail_Whip(id, skill, prop)
 {
	//make players fly away, and remove their armor
	new players[32], pnum, aid, level = SkillLevel(id, skill), apple_count;
	new Float:start_origin[3], Float:velocity[3];

	poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL )
	for( new i=0; i<pnum; i++)
	{
		aid = players[i];

		if( Pokemod_FF(id, aid) || !fm_is_ent_visible(id,aid) )
			continue;

		pev(id, pev_origin, start_origin);
		pev(aid, pev_origin, velocity);

		start_origin[0] = velocity[0] - start_origin[0];
		start_origin[1] = velocity[1] - start_origin[1];
		start_origin[2] = velocity[2] - start_origin[2];
		xs_vec_normalize( start_origin, velocity );

		velocity[0] *= TAIL_WHIP;
		velocity[1] *= TAIL_WHIP;
		velocity[2] *= TAIL_WHIP;

		set_pev(aid, pev_velocity, velocity);
		set_user_armor(aid, max(get_user_armor(aid) - level, 0) );
		apple_count++;
	}
	if( ActionPrint(id, print_center, apple_count, "tail whipped") ){
		sound_cast(id);
		return SKILL_USED;
	}
	else{
		g_StopSkill[id] = true;
		sound_deny(id);
	}
	return SKILL_READY;
 }
 public Self_Destruct(id, skill, prop)
 {
	if( Pokemod_Alive(id) )
		if( !skilldamage(id, id, AUTO_KILL, skill) )
			return;
	BlowUp(id);
	new parm[2]
	parm[0] = id
	parm[1] = skill
	set_task(0.5, "DestructDamage", 0, parm, 2)
 }
 public Explosion(id, skill, prop)
 {
	if( Pokemod_Alive(id) )
		if( !skilldamage(id, id, AUTO_KILL, skill) )
			return;
	BlowUp(id);
	new parm[2]
	parm[0] = id
	parm[1] = skill
	set_task(0.5, "DestructDamage", 0, parm, 2)
 }
 public DestructDamage(parm[])
 {
	new id = parm[0], skill = parm[1], Float:f_explode[3];
	IVecFVec(origin[id], f_explode);

	new level = SkillLevel(id, skill)
	new maxdamage = EXPLOSION_DAMAGE+level;
	new damradius = EXPLOSION_RADIUS+level;

	skillAoEdamage(id, f_explode, damradius, maxdamage, skill);
 }
 public Barrage(id, skill, prop)
 {
	new eggs;
	if( SkillTrulyActive(id, skill) )
		eggs = random_num( BARRAGE_MIN2, BARRAGE_MAX2 );
	else
		eggs = random_num( BARRAGE_MIN1, BARRAGE_MAX1 );
	new parm[3];
	parm[0] = id;
	parm[1] = eggs;
	parm[2] = 0;
	set_task(0.1,"delayed_grenade",0,parm,3);
 }
 public Horn_Attack(id, skill, prop)
 {
	new Float: origin[3], Float: aim[3], Float: velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	//move it a little in front of them
	velocity_by_aim(id, 20, velocity)
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY;

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, HORN_ATTACK_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_ROCKET])

	new Float:MinBox[3] = {-3.0, -3.0, -3.0}
	new Float:MaxBox[3] = {3.0, 3.0, 3.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	fm_entity_set_origin(Ent, origin)

	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)
	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, HORN_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	// Trail on enity.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
	write_short(Ent)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_STEAM1])	// sprite index
	write_byte(5)	// life in 0.1's
	write_byte(5)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)
	return SKILL_USED;
 }
 public Horn_Drill(id, skill, prop)
 {
	new Float: origin[3], Float: aim[3], Float: velocity[3], Ent

	pev(id, pev_origin, origin)
	pev(id, pev_v_angle, aim)

	//move it a little in front of them
	velocity_by_aim(id, 20, velocity)
	origin[0] += velocity[0];
	origin[1] += velocity[1];
	origin[2] += velocity[2];

	Ent = fm_create_entity("info_target")

	if (!Ent) return SKILL_READY;

	set_pev(Ent, pev_classname, "poke_attack")
	set_pev(Ent, pev_iuser1, HORN_DRILL_DAMAGE)
	set_pev(Ent, pev_iuser2, skill)
	fm_entity_set_model(Ent, MODELS[MDL_ROCKET])

	new Float:MinBox[3] = {-3.0, -3.0, -3.0}
	new Float:MaxBox[3] = {3.0, 3.0, 3.0}
	set_pev(Ent, pev_mins, MinBox)
	set_pev(Ent, pev_maxs, MaxBox)

	fm_entity_set_origin(Ent, origin)

	set_pev(Ent, pev_angles, aim)

	set_pev(Ent, pev_solid, SOLID_TRIGGER)
	set_pev(Ent, pev_movetype, MOVETYPE_FLY)
	set_pev(Ent, pev_owner, id)

	velocity_by_aim(id, HORN_VEC , velocity)
	set_pev(Ent, pev_velocity, velocity)

	// Trail on enity.
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMFOLLOW)		// TE_BEAMFOLLOW
	write_short(Ent)	// entity:attachment to follow
	write_short(SPRITE_INDEX[SPR_BIGSPIT])	// sprite index
	write_byte(5)	// life in 0.1's
	write_byte(5)		// line width in 0.1's
	write_byte(255)	//r,g,b
	write_byte(255)
	write_byte(255)
	write_byte(255)	// brightness
	message_end()

	poke_sound(id,CHAN_VOICE,SND_TU_FIRE)
	return SKILL_USED;
 }
 public Leer(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !ValidPlayer(victim) || !Pokemod_Alive(victim) || !get_user_armor(victim) )
		return SKILL_READY;

	set_user_armor(victim, 0);
	return SKILL_USED;
 }
 public Aurora_Beam(id, skill, prop)
 {
	new level = SkillLevel(id, skill)
	new Float:f_origin[3], Float:f_aimvec[3];

	get_user_origin(id, origin[id]);
	get_user_origin(id, aim[id], 2);
	IVecFVec(origin[id], f_origin);
	IVecFVec(aim[id], f_aimvec);

	skillLinedamage(id, f_origin, f_aimvec, floatround(level*AURORA_BEAM_RADIUS), floatround(AURORA_BEAM_DAMAGE*level), skill);
	MakeLine(0,origin[id],aim[id],SPRITE_INDEX[SPR_BIGSPIT],1,1,1,level,0,255,255,255,255,0);

	new sprites = get_distance(origin[id], aim[id])/75
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)	//message begin
	write_byte(15)
	write_coord(origin[id][0])		// start position
	write_coord(origin[id][1])
	write_coord(origin[id][2])
	write_coord(aim[id][0])	// end position
	write_coord(aim[id][1])
	write_coord(aim[id][2])
	write_short(SPRITE_INDEX[SPR_MUZZLEFLASH1])		// sprite index
	write_byte(sprites)		// count
	write_byte(1)		// life in 0.1's
	write_byte(level/15)		// scale in 0.1's
	write_byte(5)		// velocity along vector in 10's
	write_byte(5)	// randomness of velocity in 10's
	message_end()

	poke_sound(id,CHAN_VOICE, SND_CAST);

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;

	return SKILL_READY;
 }
 public Sonic_Boom(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_BIGGUN)

	new level = SONIC_BOOM_DAMAGE*SkillLevel(id, skill)
	new radius = SONIC_BOOM_RADIUS + level

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin);
	skillAoEdamage(id, attack_origin, radius, level, skill, AOE_IGNORE);

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
	write_short( SPRITE_INDEX[SPR_SHOCKWAVE] )
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
 public Stomp(id, skill, props)
 {
	poke_sound(id, CHAN_VOICE, SND_BIGGUN);

	new level = SkillLevel(id, skill)
	new radius = STOMP_RADIUS + level;

	new Float:attack_origin[3];
	pev(id, pev_origin, attack_origin);
	skillAoEdamage(id, attack_origin, radius, max(10,level), skill, AOE_IGNORE|AOE_MAXDAM, _, STATUS_PARALYZE, -1);

	FVecIVec( attack_origin, origin[id] );

	for( new i=1; i<4; i++ ){
		// blast circles
		message_begin( MSG_PAS, SVC_TEMPENTITY, origin[id] )
		write_byte(TE_BEAMCYLINDER)
		write_coord( origin[id][0])
		write_coord( origin[id][1])
		write_coord( origin[id][2] - 16)
		write_coord( origin[id][0])
		write_coord( origin[id][1])
		write_coord( origin[id][2] - 16 + radius/i)
		write_short( SPRITE_INDEX[SPR_SHOCKWAVE] )
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

	return SKILL_USED;
 }
 public Transform(id, skill, prop)
 {
	new Ent = 0;
	if( prop & SI_RESET ){
		while( (Ent = fm_find_ent_by_owner(Ent, "transform_ent", id) ) )
			fm_remove_entity(Ent);
		return SKILL_READY;
	}

	new bool:found_ent = false;
	while( (Ent = fm_find_ent_by_owner(Ent, "transform_ent", id) ) ){
		found_ent = true;
		break;
	}

	new model[32], Float:origin[3];
	new aiment, body;
	get_user_aiming(id, aiment, body);
	pev(aiment, pev_origin, origin);

	if(	!pev_valid(aiment)
	||	(origin[0] == 0.0 && origin[1] == 0.0 && origin[2] == 0.0)
	){
		aiment = -1;
		new Float:f_aimvec[3], counter;

		get_user_origin(id, aim[id], 2);
		IVecFVec(aim[id], f_aimvec);

		while( (aiment = fm_find_ent_in_sphere(aiment,f_aimvec,50.0)) && counter++ < 50 ){
			if( aiment == id )
				continue;
			pev(aiment, pev_origin, origin);
			pev(aiment, pev_model, model, 31);
			if(origin[0] != 0.0 && origin[1] != 0.0 && origin[2] != 0.0 && !equal(model,EMPTY) )
				break;
		}
	}

	if( pev_valid(aiment) ){
		pev(aiment, pev_origin, origin);
		if( origin[0] != 0.0 && origin[1] != 0.0 && origin[2] != 0.0 ){

			if( !found_ent ){
				Ent = fm_create_entity("info_target")
				if( !Ent ) return SKILL_READY;

				set_pev(Ent, pev_classname, "transform_ent");
				set_pev(Ent, pev_owner, id);

				set_pev(Ent, pev_movetype, MOVETYPE_FOLLOW);
				set_pev(Ent, pev_aiment, id);
			}

			if( Pokemod_Alive(aiment) )
				fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, 0);
			else
				fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, TRANSFORM_ALPHA);

			new char_helper;
			pev(aiment, pev_model, model, 31);
			fm_entity_set_model(Ent, model);

			while( (char_helper=containi(model,"/")) > -1 )
				copy(model, 31, model[char_helper+1]);
			if( (char_helper=containi(model,"w_")) == 0 )
				replace(model, 31, "w_", "");
			if( (char_helper=containi(model,".")) > -1 )
				model[char_helper] = '\0';

			poke_print(id, print_center, "Transformed into %s.",model);

			g_StopSkill[id] = true;
			return SKILL_USED;
		}
		else poke_print(id, print_center, "This is not a valid object to transform into.");
	}
	else poke_print(id, print_center, "You must aim at an object to transform into it.");

	return SKILL_READY;
 }
 public Sing(id, skill, prop)
 {
	new players[MAX_PLAYERS], pnum, aid;
	poke_get_players(players, pnum, PLAYER_ALIVE);

	new radius = SING_RADIUS + SkillLevel(id, skill)
	new affected;

	for ( new i = 0; i < pnum; i++ )
	{
		aid = players[i];
		poke_sound(aid, CHAN_VOICE, SND_SING);

		if( !Pokemod_FF(id, aid) && fm_entity_range(id,aid) < radius ) {
			poke_print(aid, print_center, "You fell asleep to someone's singing.");
			ApplyStatus( id, aid, STATUS_ASLEEP, SING_SLEEP );
			affected++;
		}
	}
	ActionPrint(id, print_center, affected, "sang to");
	return SKILL_USED;
 }
 public Screech(id, skill, prop)
 {
	new players[MAX_PLAYERS], pnum, aid;
	poke_get_players(players, pnum, PLAYER_ALIVE);

	new level = SkillLevel(id, skill)
	new radius = SCREECH_RADIUS + level;
	new Float:delay = 0.5 + level/TotalLevels;

	poke_sound(id, CHAN_VOICE, SND_SUCK);
	for ( new i = 0; i < pnum; i++ )
	{
		aid = players[i];
		if( Pokemod_FF(id, aid) ) continue;

		if( fm_entity_range(id,aid) < radius ) {
			poke_print(aid, print_center, "Someone screeched at you.");
			bind_used(aid, _, delay);
			poke_sound(aid, CHAN_VOICE, SND_SUCK);
			set_pev(aid, pev_velocity, Float:{0.0,0.0,0.0});
		}
	}

	if( end_rep_use(id, skill, 1.0) )
		return SKILL_USED;
	return SKILL_READY;
 }
 public Softboiled(id, skill, prop)
 {
	new heal

	sound_cure(id)
	screenflash(id, 128, 128, 128, 128)

	heal = SkillClamp(id, skill, 50, 125)
	Pokemod_Heal(id, heal)

	return SKILL_USED;
 }
 public Present(id, skill, prop)
 {
	new damage, weapon, victim = PokeSkillTarget(id, damage, weapon);
	if( Pokemod_Alive(victim) && grenade_weapon( weapon ) ){
		new Float:victim_origin[3];
		pev( victim, pev_origin, victim_origin );
		skillAoEdamage(id, victim_origin, SkillLevel(id, skill)*5+100, 			damage, skill, AOE_IGNORE|AOE_MAXDAM);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
