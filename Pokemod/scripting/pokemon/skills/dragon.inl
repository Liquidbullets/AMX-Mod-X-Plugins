
 public Dragon_Rage(id, skill, prop)
 {
	new damage, victim = PokeSkillTarget(id, damage);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) ){
		new percent_damage = SkillPercent(id, skill, clamp(40-damage,0,40))
		if( percent_damage ){
			skilldamage(victim, id, percent_damage, skill, TYPE_NONE)
			poke_sound(id, CHAN_VOICE, SND_AG_DIE)
			return SKILL_USED;
		}
	}
	return SKILL_READY;
 }
 public Faint_Attack(id, skill, prop)
 {
	if( prop & SI_RESET ){

		set_user_rendering(id);

		return SKILL_READY;
	}

	//if they have the skill active
	if( SkillLevel(id, skill) ){

		new visibility = SkillClamp( id, skill, 225, 135 );

		// if they have knife out
		if( knife_weapon( get_user_weapon(id) ) )
	
			visibility /= 6;

		fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, 		visibility);

		return SKILL_USED;

	}

	else set_user_rendering(id);

	return SKILL_READY;
 }
 public Reversal(id, skill, prop)
 {
	new damage, weapon;
	PokeSkillTarget(id, damage, weapon);
	new max = damage
	if( SkillTrulyActive(id, skill) )
		max += REFLECT_ACTIVE
	if( MaxSkillChance(id,skill, max) && knife_weapon(weapon) ){
		screenflash(id, 255, 0, 255, 50)
		Pokemod_Heal(id,damage)

		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Mirror_Coat(id, skill, prop)
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
