
 public Poison_Powder(id, skill, prop)
 {
	new powdernum = POISON_POWDERS

	//make one immediately
	new parm[3]
	parm[0] = id;
	parm[1] = 1;
	parm[2] = 0;

	make_powder(parm)
	if( --powdernum )
		set_task(0.5,"make_powder", 0, parm, 3, "a", powdernum)
	return SKILL_USED;
 }
 public Sleep_Powder(id, skill, prop)
 {
	new powdernum = SLEEP_POWDERS

	//make one immediately
	new parm[3]
	parm[0] = id;
	parm[1] = 0;
	parm[2] = 1;

	make_powder(parm)
	if( --powdernum )
		set_task(0.5,"make_powder", 0, parm, 3, "a", powdernum)
	return SKILL_USED;
 }
 public Acid(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && MaxSkillChance(id,skill,50) && !g_Status[victim][STATUS_POISON] ){
		ApplyStatus( id, victim, STATUS_POISON, get_ppcvar_float(pm_poisontime) )
		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ARBOK],1,1,5,20,0, 0,200,150, 100, 0)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Acid_Armor(id, skill, prop)
 {
	if( g_Status[id][STATUS_POISON] ){
		RemoveStatus(id, STATUS_POISON);
		poke_print(id, print_center, "Acid Armor Protected you from Poison");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Poison_Sting(id, skill, prop)
 {
	new victim = PokeSkillTarget(id);
	if( ValidPlayer(victim) && Pokemod_Alive(victim) && SkillChance(id,skill) && !g_Status[victim][STATUS_POISON] ){
		ApplyStatus( id, victim, STATUS_POISON, get_ppcvar_float(pm_poisontime) )
		MakeLine_Ent(0,id,victim,SPRITE_INDEX[SPR_ARBOK],1,1,5,20,0,150,0,150,100,0)
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Poison_Gas(id, skill, prop)
 {
	Gas(id, _, true);
	return SKILL_USED;
 }
 public Smog(id, skill, prop)
 {
	Gas(id, true);
	return SKILL_USED;
 }
 public Toxic(id, skill, prop)
 {
	Gas(id, _, true);
	return SKILL_USED;
 }
 public Insomnia(id, skill, prop)
 {
	if( g_Status[id][STATUS_ASLEEP] ){
		RemoveStatus(id, STATUS_ASLEEP);
		poke_print(id, print_center, "Insomnia protected you from going to sleep");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
 public Minimize(id, skill, prop)
 {
	if( g_Status[id][STATUS_SLOW] ){
		RemoveStatus(id, STATUS_SLOW);
		poke_print(id, print_center, "Your enemy tried to slow you down");
		return SKILL_USED;
	}
	return SKILL_READY;
 }
