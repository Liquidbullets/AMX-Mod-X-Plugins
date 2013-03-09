
 ////////////////////
 //  Makes an oak  //
 ////////////////////
 public npc_pokecenter(team)
 {
	if(ValidPC(team) || !PMon() )
		return 0

	new entSpawn
	new Float:spawnAngle[3] 
	new Float:spawnOrigin[3]

	PCent[team] = fm_create_entity("info_target")
	if(!PCent[team])
		return 0

	entSpawn = find_free_spawn(team, spawnOrigin, spawnAngle)

	if (!entSpawn)
	{
		// Didn't find a free spawn spot. Try again in 1 seconds...
		fm_remove_entity(PCent[team])
		set_task(1.0,"npc_pokecenter", team)
		return 0
	}

	spawnOrigin[2] += 10

	new oak_props[MAX_OAKPROP]
	get_ppcvar_string( pm_oak, oak_props, MAX_OAKPROP-1)

	if( !create_oak(PCent[team], spawnOrigin, spawnAngle, team, oak_props) ){
		fm_remove_entity( PCent[team] )
		return 0
	}

	return 1
 }
 create_oak(oak, Float:origin[3], Float:angles[3], team=0, props[MAX_OAKPROP], hp=-1)
 {
	if(find_oak(oak, origin, 100.0)){
		debugMessage(0,3,"NPC", "There is an npc close to here already.")
		return 0
	}

	set_pev(oak, pev_classname, "npc_pokecenter");
	set_pev(oak, pev_globalname, props);

	fm_entity_set_model(oak, MODELS[MDL_OAK]);
	set_pev(oak, pev_solid, SOLID_BBOX)

	set_pev(oak, pev_controller_0, 125);
	set_pev(oak, pev_controller_1, 125);
	set_pev(oak, pev_controller_2, 125);
	set_pev(oak, pev_controller_3, 125);

	fm_entity_set_size(oak, Float:{-16.0,-16.0,-40.0},Float:{16.0,16.0,40.0})

	fm_entity_set_origin(oak, origin);

	set_pev(oak, pev_angles, angles)

	set_pev(oak, pev_animtime,2.0);
	set_pev(oak, pev_framerate,1.0);
	set_pev(oak, pev_sequence,0);

	set_pev(oak, pev_movetype, 6)

	if(hp>0){
		set_pev(oak, pev_takedamage, 1.0)
		set_pev(oak, pev_health, float(hp))
	}
	else if(get_ppcvar_num(pm_oakhp)>0){
		set_pev(oak, pev_takedamage, 1.0)
		set_pev(oak, pev_health, get_ppcvar_float(pm_oakhp))
	}

	set_pev(oak, pev_team, team)

	fm_drop_to_floor(oak)

	return 1
 }
 //without an oak_to_ignore, it will reset that oak before it is changed
 find_oak(oak_to_ignore,Float:origin[3],Float:distance)
 {
	new Float:temp_origin[3]
	for(new i=0; i<MAX_OAKS; i++){
		if(PCent[i]!=oak_to_ignore && ValidPC(i)){
			pev(PCent[i],pev_origin,temp_origin)
			if(get_distance_f(temp_origin,origin)<distance)
				return true
		}
	}
	return false
 }
 
 //////////////////////////////////
 //  Starts the pokecenter heal  //
 //////////////////////////////////
 public start_heal(id)
 {
	if(NearOakCheck(id)<1){
		poke_print(id, print_chat, "%L: %L", id,"POKEMOD_OAKNAME", id,"POKEMOD_OAKFAR");
		return;
	}
	if(g_Status[id][STATUS_BIKE]){
		poke_print(id, print_chat,"%L: %L", id,"POKEMOD_OAKNAME", id,"POKEMOD_OAKRIDING");
		return;
	}
	poke_sound(id, CHAN_ITEM, SND_POKECENTER);
	AddStatus(id, STATUS_OAK_STOP);
	SetSpeed(id);
	set_task(2.0,"finish_heal", id);
 }
 
 ////////////////////////////////////
 //  Finishes the pokecenter heal  //
 ////////////////////////////////////
 public finish_heal(id)
 {
	RemoveStatus(id, STATUS_OAK_STOP);

	if(!PMon() || !Pokemod_Alive(id)) return;

	if(NearOakCheck(id)>0){
		CheckMaxHP(id);
		poke_print(id, print_chat, "%L: %L", id,"POKEMOD_OAKNAME", id,"POKEMOD_OAKHEALED");
		Pokemod_Heal(id, abs(PlayerStat[id][STAT_MAXHP]) );
		ResetStatus(id);

		#if PC_RESET_SKILLS == 2
			ResetSkills(id, true)
		#endif
		#if PC_RESET_SKILLS == 1
			ResetSkills(id, false)
		#endif
	}
	else {
		poke_print(id, print_chat, "%L: %L", id,"POKEMOD_OAKNAME", id,"POKEMOD_OAKFAR");
	}

	SetSpeed(id);
 }
 
 /////////////////////////////////
 //  Makes Pokeballs at Spawns  //
 /////////////////////////////////
 public SpawnPokeballs()
 {
	if( PM_disabled != PM_ON )
		return

	for(new i=0; i < POKEBALL_NUMBER; i++){
		MakeSpawnPokeball(1, i)
		MakeSpawnPokeball(2, i)
	}
 }
 
 /////////////////////////////////
 //  Makes Pokeballs at Spawns  //
 /////////////////////////////////
 MakeSpawnPokeball(team, num)
 {
	if(pev_valid(Pokeballs[team][num]))
		return

	new poke, special_chance = random_num(1, 100);
	do poke = random_num(1,PokesCreated);
	while(	PokeDisabled(poke)
	||		PokeSkill[poke] == -1
	||		(PokeInfo[poke] & PI_SPECIAL && special_chance > POKEBALL_SPECIAL)
	||		PokeInfo[poke] & PI_NOFIND
	)

	new ent, entSpawn
	new Float:spawnAngle[3] 
	new Float:spawnOrigin[3]

	ent = fm_create_entity("info_target")

	if(!pev_valid(ent))
		return

	Pokeballs[team][num] = ent
	set_pev(ent, pev_classname, "pokemon")
	set_pev(ent, pev_targetname, PokeToName(poke))
	set_pev(ent, pev_team, team)

	fm_entity_set_model(ent, MODELS[MDL_POKEBALL])
	set_pev(ent,pev_solid, SOLID_BBOX)

	fm_entity_set_size(ent,Float:{-5.0,-5.0,-5.0},Float:{5.0,5.0,5.0})

	entSpawn = find_free_spawn(team, spawnOrigin, spawnAngle)

	if(!entSpawn){
		fm_remove_entity(ent)
		return
	}

	spawnOrigin[2] += 10

	fm_entity_set_origin(ent, spawnOrigin);
	set_pev(ent, pev_angles, spawnAngle)

	set_pev(ent, pev_movetype, MOVETYPE_TOSS)
 }
 
 find_free_oak()
 {
	for(new i=0; i<MAX_OAKS; i++){
		if(!ValidPC(i))
			return i
	}
	return -1
 }
 
 
 
 
 
 