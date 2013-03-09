 public Counter(id, skill, prop)
 {
	static counter[MAX_PLAYERS];
	if( prop & SI_ROUNDRESET ){
		counter[id] = 0;
		return SKILL_READY;
	}

	new damage, victim = PokeSkillTarget(id, damage);
	if( prop & SI_DAMAGED || prop & SI_PDAMAGED ){
		new counternums = damage / COUNTER_FACTOR
		counter[id] += counternums

		if(counter[id] > COUNTER_MAX)
			counter[id] = COUNTER_MAX

		poke_print(id,print_center,"Your counter is at %d extra damage", counter[id])
	}

	if( Pokemod_Alive(victim) && MaxSkillChance(id,skill,counter[id]) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		poke_print(id,print_center,"You countered with %d damage", skilldamage( victim, id, counter[id], skill))
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Karate_Chop(id, skill, prop)
 {
	new weapon, victim = PokeSkillTarget(id, _, weapon);
	if( Pokemod_Alive(victim) && knife_weapon( weapon ) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		skilldamage(victim, id, KARATE_CHOP, skill);
		if( ValidPlayer(victim) )
			screenflash(victim, 0, 0, 0, 255, 1.0);
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Submission(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		if( ValidPlayer(victim) )
			ApplyStatus( id, victim, STATUS_PARALYZE, SUBMISSION_TIME );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Take_Down(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		if( ValidPlayer(victim) )
			ApplyStatus( id, victim, STATUS_PARALYZE, TAKE_DOWN_TIME );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Low_Kick(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		skilldamage(victim, id, LOW_KICK, skill);

		if( ValidPlayer( victim ) ){
			new ran = random_num(1,180), Float:look[3];
			pev(victim, pev_angles, look);
			if(look[0]<180) look[0] += ran;
			else look[0] -= ran;
			ran = random_num(1,180);
			if(look[1]<180) look[1] += ran;
			else look[1] -= ran;
			set_pev(victim, pev_angles, look);
			set_pev(victim, pev_fixangle, 1);
		}

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Seismic_Toss(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !fm_is_ent_visible(id, victim) || !ValidPlayer(victim) )
		return SKILL_READY;

	new Float:velocity[3];
	velocity_by_aim(id, SEISMIC_TOSS, velocity);
	velocity[2] = floatmax(velocity[2], 50.0);

	set_pev(victim, pev_velocity, velocity);
	skilldamage(victim, id, SEISMIC_TOSS_DAMAGE, skill);
	return SKILL_USED;
 }
 public Triple_Kick(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( !Pokemod_Alive(victim) || !fm_is_ent_visible(id, victim) )
		return SKILL_READY;

	if( ValidPlayer(victim) ){
		new Float:velocity[3];
		velocity_by_aim(id, TRIPLE_KICK, velocity);
		velocity[2] = floatmax(velocity[2], float(TRIPLE_KICK_UP));

		set_pev(victim, pev_velocity, velocity);
	}
	skilldamage(victim, id, TRIPLE_KICK_DAMAGE, skill);
	return SKILL_USED;
 }
 public Fire_Punch(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_FLAME);
		if( ValidPlayer(victim) )
			ApplyStatus( id, victim, STATUS_BURN, THUNDER_PUNCH_TIME );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Thunder_Punch(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_BIGGUN);
		if( ValidPlayer(victim) )
			ApplyStatus( id, victim, STATUS_PARALYZE, THUNDER_PUNCH_TIME );
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Lovely_Kiss(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( Pokemod_Alive(victim) ){
		poke_sound(id, CHAN_VOICE, SND_BODYDROP);
		if( ValidPlayer(victim) )
			ApplyStatus( id, victim, STATUS_ASLEEP, THUNDER_WAVE_TIME );
		return SKILL_USED;
	}
	return SKILL_READY;
 }

