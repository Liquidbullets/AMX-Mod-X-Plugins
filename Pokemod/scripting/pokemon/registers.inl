
 ///////////////////////////////////////////
 //  Pokemon skills that have to do with  //
 //    being damaged or giving damage     //
 ///////////////////////////////////////////
 #if MOD==CS || MOD==NS
	public player_damage(id)
	{
		new aid, weapon;
		aid = get_user_attacker(id, weapon);
		pokemon_damage( aid, id, read_data(2), weapon );
	}
 #endif
 #if MOD==DOD
	public client_damage(aid, id, damage, weapon, body, TA)
		pokemon_damage( aid, id, damage, weapon );
 #endif
 public pokemon_damage( aid, id, damage, weapon )
 {
	if( PMon() )
	{
		if(PM_disabled == PM_XP)
			return

		if(Pokemod_Connected(aid)){
			new aname[32]
			get_user_name(aid, aname, 31)
			debugMessage(id,5,"Player", "took %d damage from %s<%s>",damage,aname,PokeToName(Pokemon[aid]))
		}
		else{
			debugMessage(id,5,"Player", "took %d damage",damage)
			aid = 0
		}

		PlayerDamageInfo[id][PD_ATTACKER] = aid;
		PlayerDamageInfo[id][PD_DAMAGE] = damage;
		PlayerDamageInfo[id][PD_WEAPON] = weapon;

		#if DAMAGE_AWAKEN==1
		//wake them up if they were sleeping
		if(g_Status[id][STATUS_ASLEEP])
			unsleep(id)
		#endif

		if( Pokemod_Alive(id) ){
			new holder
			for(new i=0; i < ItemsCreated; i++)
				if( ItemInfo[i] & II_ARMOR && HasPokeItem(id, i) )
					holder += ItemArmor[i]*PlayersItem[id][i]
			Pokemod_Repair(id, holder)

			#if AUTO_ITEMS > 0
				AutoItems(id)
			#endif
		}

		if( aid != id ){
			skills_handler(aid, SI_ATTACK);
			skills_handler(id, SI_DAMAGED);
		}

		if( Pokemod_Alive(id) && aid ){ //need to keep making sure theyre alive incase they died by a pokeskill
			for(new i=0; i < ItemsCreated; i++)
				if( ItemInfo[i] & II_DAMAGE && HasPokeItem(aid, i) )
					pokedamage(id, aid, (PlayersItem[aid][i]*ItemDamage[i]), ItemName[i], TYPE_NONE, _, _, _, false)
		}

		//Clear the players damage information
		PlayerDamageInfo[id][PD_ATTACKER] = PlayerDamageInfo[id][PD_DAMAGE] = PlayerDamageInfo[id][PD_WEAPON] = 0;
	}
 }
 
 //////////////////////
 //  Every ResetHUD  //
 //////////////////////
 public pokemon_spawn(id)
 {
	//things that we need to do immediately, and doesnt matter if we do it twice
	get_user_origin(id, aim[id]);
	pev(id, pev_origin, confuse[id]);
	Pokemod_Team(id, true);

	//Stop if PokeMod is not on
	if( !PMon() ) return PLUGIN_CONTINUE;

	//lets only do this once, incase they were respawned
	if( !task_exists(id+SPAWN_TASK) ) set_task(1.0, "spawn_stuff", id+SPAWN_TASK);

	return PLUGIN_CONTINUE;
 }
 public spawn_stuff(id)
 {
	id -= SPAWN_TASK;

	//Stop if its not a valid user
	if( !ValidPlayer(id) || !Pokemod_Alive(id, true) )
		return PLUGIN_CONTINUE;

	//Stop if they are not on a normal team
	if( 1 > Pokemod_Team(id) > 2 ) return PLUGIN_CONTINUE;

	SetAction( id );

	#if CHANGE_POKES != 0
	ChangedPoke[id] = CHANGE_POKES;
	#endif

	if(g_isWild[id]==-1)
		g_isWild[id] = 0;

	new i;
	debugMessage(id,3,"Player", "spawned with %s", PokeToName(Pokemon[id]));

	if( !Pokemod_Connected(id) )
		debugMessage(id,2,"Player", "Data Load%s", LoadXP(id)?"ed Successfully":" Failed" );

	#if ALLOW_WILD==1
	//give them a new pokemon if theyre being wild
	if(g_isWild[id]>0 && g_Died[id])
		AssignWild(id);
	#endif

	//Auto Things for bots
	if(is_user_bot(id))
		BotCenter(id);

	RefreshPoke(id);

	//let them select a starting pokemon
	if(!PlayersPoke[id][0]					//if they dont have a pokemon out
	&& !PlayersPoke[id][MAX_ACTIVE_POKES]	//if they dont have a pokemon in pc
	&& Pokemod_Connected(id))						//and they have their xp loaded
		StartMenu(id);

	#if MOD==CS
		new current = cs_get_user_money(id);

		if(current==16000 && money[id]>16000)
			Pokemod_Pay(id, money[id]-current);
		else
			money[id] = current;
	#endif

	//Do other stuff such as setting hp and removing pokemon statuses

	CheckMaxHP(id);
	CheckMaxArmor(id);
	if( Health(id) < abs( PlayerStat[id][STAT_MAXHP] ) )
		set_user_health(id, abs( PlayerStat[id][STAT_MAXHP] ) );

	for( i=0; i < ItemsCreated; i++)
		if( ItemInfo[i] & II_ARMOR && HasPokeItem(id, i) )
			Pokemod_Repair(id, ItemArmor[i]*PlayersItem[id][i]);

	// set it to 0.0 so some skills reset (otherwise they think he has a wait time and are not activated)
	set_bind(id);
	ResetSkills(id, true);

	poke_UnStuck(id, false);

	if(g_Died[id])
		ResetStatus(id, 1);	//for some reason people get stuck occasionally, this fixes the bug
	else
		ResetStatus(id);

	skills_handler(id, SI_SPAWN);

	set_bind(id, SPAWN_DELAY);

	SetSpeed(id);

	g_Died[id] = false;

	return PLUGIN_CONTINUE
 }
 
 /////////////////////////
 //  When someone dies  //
 /////////////////////////
 public pokemon_death()
 {
	if(!PMon()) return PLUGIN_CONTINUE

	new attacker = read_data(1);
	new id = read_data(2);
	new bool:caught = false;

	PlayerInfo[id][PL_ALIVE] = 0;
	debugMessage(id,3,"Player", "was killed");

	new pname[NAME_LENGTH];
	PokeToName(Pokemon[id], pname, NAME_LENGTH);

	if( Pokemod_Connected(attacker) ){

		KillXP(attacker, id);

		#if MOD!=CS
			if( Pokemod_FF(id, attacker) ) Pay(attacker, -300);
			else Pay(attacker, 300);
		#endif

		if( ValidSlot( Slot(attacker) ) && !Pokemod_FF(id, attacker) ){

			if( CatchChance( attacker, Pokemon[id], PlayersLevel(id, PlayersActive[id]) ) ){
				new name[32];
				get_user_name(attacker, name, 31);

				#if POKEMON_CATCH_XP==0
					AddPoke(attacker, Pokemon[id], 1);
				#else
					AddPoke(attacker, Pokemon[id], PlayersXP[id][PlayersActive[id]]);
				#endif

				if( g_isWild[id] > 0 )
					poke_printlang(LANG_PLAYER,"POKEMOD_CAUGHTWILD", name, PlayersLevel(attacker, HasPokemon(attacker, Pokemon[id])), pname);
				else
					poke_printlang(LANG_PLAYER,"POKEMOD_CAUGHT", name, PlayersLevel(attacker, HasPokemon(attacker, Pokemon[id])), pname);

				#if CATCH_TELEPORT==2
					EscapeRope(attacker, 0);
					poke_printlang(attacker,"POKEMOD_CATCHTELE", pname);
				#endif

				#if POKE_POPULATION==0
					if(g_isWild[id]<1)
					{
						ReleasePoke(id, Pokemon[id], false);
						poke_printlang(id,"POKEMOD_CATCHTAKE", name, pname);
					}
				#endif

				caught = true;
			}
		}
	}
	if(!caught && g_isWild[id]>0){
		poke_printlang(LANG_PLAYER,"POKEMOD_WILDDIE", pname);
	}

	// set their bind to nothing so skills reset/activate
	set_bind(id);

	skills_handler(id, SI_DEAD);

	ResetSkills(id, true);
	ResetStatus(id, 1);

	set_bind(id, WAIT_TIME);

 	PlayerStat[id][STAT_MAXHP] = 100;
	g_Died[id] = true;

	new i;
	for( i=0; i<=MAX_PLAYERS; i++ )
		if(PlayerEnt[i] == id)
			PlayerEnt[i] = 0;

	if( PM_disabled != PM_ON )
		return PLUGIN_CONTINUE;

	if( !PokeItem(id) ){
		#if RANDOM_ITEMS == 0
			return PLUGIN_CONTINUE;
		#else

			if( random_num(0,100) < abs(RANDOM_CHANCE) ){
				new rand_item = random(ItemsCreated);

				#if RANDOM_CHANCE < 0 && RANDOM_ITEMS == 1
				while( ItemInfo[rand_item]&II_SPECIAL )
					rand_item = random(ItemsCreated);
				#endif

				#if RANDOM_ITEMS == 2
					new price = abs( CostOfItem(rand_item) );
					if( !price )
						price = MAX_MONEY;

					#if RANDOM_CHANCE < 0
						while( price > random_num(0, MAX_MONEY) || ItemInfo[rand_item]&II_SPECIAL ){
							rand_item = random(ItemsCreated);
							price = abs( CostOfItem(rand_item) );
							if( !price )
								price = MAX_MONEY;
						}
					#else
						while( price > random_num(0, MAX_MONEY) ){
							rand_item = random(ItemsCreated);
							price = abs( CostOfItem(rand_item) );
							if( !price )
								price = MAX_MONEY;
						}
					#endif
				#endif

				PlayersItem[id][rand_item]++;
			}
			else
				return PLUGIN_CONTINUE;
		#endif
	}

	//Makes pokeball for item drops

	new ilen, targname[255], bool:make_drop = false;
	for(i = 0; i < ItemsCreated; i++){
		if( HasPokeItem(id, i) && !(ItemInfo[i]&II_NODROP) ){
			ilen += formatex(targname[ilen], (255-ilen), "%d %d ", i, PlayersItem[id][i]);
			PlayersItem[id][i] = 0;

			if( ItemEventForward > -1 )
			{
				new functionReturn
				ExecuteForward(ItemEventForward, functionReturn, id, IE_DROP, i)

				// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
				// and 0 is used by continue and invalid return.
				if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
				{
					return PLUGIN_HANDLED;
				}
			}

			make_drop = true;
		}
	}

	if( !make_drop )
		return PLUGIN_CONTINUE;

	new Float:temporigin[3], Float:angle[3];
	pev(id, pev_origin, temporigin);
	pev(id, pev_v_angle, angle);

	new mark = fm_create_entity("info_target");
	if( !pev_valid(mark) )
		return PLUGIN_CONTINUE;

	set_pev( mark, pev_classname, "pokeitem" );
	set_pev( mark, pev_targetname, targname );
	fm_entity_set_model( mark, MODELS[MDL_POKEBALL] );
	set_pev( mark, pev_solid, SOLID_BBOX );

	fm_entity_set_size(mark,Float:{-5.0,-5.0,-5.0},Float:{5.0,5.0,5.0});

	fm_entity_set_origin( mark, temporigin );
	set_pev( mark, pev_angles, angle );

	set_pev( mark, pev_movetype, MOVETYPE_TOSS );

	new Float:time = get_ppcvar_float(pm_itemtime);
	if(time>0.0)
		set_task( time, "poke_remove_entity", mark+REMOVE_TASK );

	return PLUGIN_CONTINUE;
 }

 /////////////////////////////////////////
 //  When someone aims at someone else  //
 /////////////////////////////////////////
 public pokemon_statustext(id)
 {
	if(!Pokemod_Alive(id) || !PMon() )
		return PLUGIN_CONTINUE;

	new abody, aid, dist;
	get_user_aiming(id, aid, abody, dist);

	if( Pokemod_Alive(aid) ){
		//pokemon that use aiming
		if( !Pokemod_FF(id, aid) ){
			skills_handler(id, SI_LOOK);
		}
	}
	return PLUGIN_CONTINUE;
 }

 /////////////////////////////////////
 //  Used if no rounds for the mod  //
 /////////////////////////////////////
 public team_ready(id)
 {
	poke_round_end();
	set_task(0.1,"round_start");
	set_task(0.1,"pokemon_spawn",id);
 }
 /////////////////////////
 //  Every Round Start  //
 /////////////////////////
 public round_start()
 {
	debugMessage(0,2,"Round", "Started");

	#if MOD==CS
		#if ALLOW_WILD==1
			WildMessage();
		#endif

		new poke_ent;
		while( (poke_ent = fm_find_ent_by_class(-1,"pokeitem") ) ){
			fm_remove_entity(poke_ent);
			remove_task(poke_ent+REMOVE_TASK);
		}
		while( (poke_ent = fm_find_ent_by_class(-1,"pokemod_ent") ) ){
			fm_remove_entity(poke_ent);
			remove_task(poke_ent+REMOVE_TASK);
		}
		while( (poke_ent = fm_find_ent_by_class(-1,"poke_attack") ) ){
			fm_remove_entity(poke_ent);
			remove_task(poke_ent+REMOVE_TASK);
		}
	#endif

	if(!PMon()){
		poke_round_end();
		return PLUGIN_CONTINUE;
	}

	if(get_ppcvar_num(pm_pokeballs)==1){
		set_task(5.0, "SpawnPokeballs", 0);
	}

	remove_task(BUYTIME_TASK);
	CanBuy = true;
	new Float:buytime = get_ppcvar_float(pm_buytime);
	if(buytime>0.0){
		set_task(buytime*60,"EndBuying",BUYTIME_TASK);
	}

	new Float:freezetime = get_pcvar_float(mp_freezetime);
	if(freezetime>0.0){
		remove_task(FREEZE_TASK);
		FreezeTime = true;
		set_task(freezetime,"EndFreezeTime",FREEZE_TASK);
	}

	return PLUGIN_CONTINUE;
 }
 
 /////////////////////////
 //  Every Round Start  //
 /////////////////////////
 public poke_round_end()
 {
	debugMessage(0,2,"Round", "Ended");

	new poke_ent;
	while( ( poke_ent = fm_find_ent_by_class(-1,"pokemon") ) ){
		fm_remove_entity(poke_ent);
		remove_task(poke_ent+REMOVE_TASK);
	}
	#if MOD==CS
		while( ( poke_ent = fm_find_ent_by_class(-1,"pokemod_ent") ) ){
			fm_remove_entity(poke_ent);
			remove_task(poke_ent+REMOVE_TASK);
		}
		while( (poke_ent = fm_find_ent_by_class(-1,"poke_attack") ) ){
			fm_remove_entity(poke_ent);
			remove_task(poke_ent+REMOVE_TASK);
		}
	#endif

	//Remove old npcs if pm is off
	if(!PMon() || get_ppcvar_num(pm_pokecenter)<=0){
		for(new i=0;i<MAX_OAKS;i++)
			if(ValidPC(i)) fm_remove_entity(PCent[i]);
	}
	else{
		#if MOD==NS
		if(ns_is_combat()){
		#endif
			if(!ValidPC(1)) npc_pokecenter(1); //only recreate for normal teams
			if(!ValidPC(2)) npc_pokecenter(2);
		#if MOD==NS
		}
		#endif
	}

	#if SAVE_ROUNDS > 0
	if(SaveXPon()){
		#if SAVE_ROUNDS > 1
		if( ++save_round_count >= SAVE_ROUNDS ){
			save_round_count = 0;
		#endif
			save_all();
		#if SAVE_ROUNDS > 1
		}
		#endif
	}
	#endif
	#if SAVE_ROUNDS < 0
	if(SaveXPon()){
		new players[MAX_PLAYERS], pnum, id;
		poke_get_players( players, pnum );

		if( pnum <= abs( SAVE_ROUNDS ) )
			save_all();
		else{
			new loop_preventer;
			while( RandomSaves[0] < abs( SAVE_ROUNDS ) && loop_preventer++ < pnum ){
				id = players[ random( pnum ) ];
				if( !RandomSaves[id] ){
					RandomSaves[id] = 1;
					RandomSaves[0]++;
					SaveXP(id, 1);
				}
			}
			if( loop_preventer >= pnum ){
				for( loop_preventer = 0; loop_preventer < pnum; loop_preventer++ )
					RandomSaves[ players[loop_preventer] ] = 0;
			}
			RandomSaves[0] = 0;
		}
	}
	#endif
 }
 
 ////////////////////////////////////
 //  When Someone Changes Weapons  //
 ////////////////////////////////////
 public WeaponChange(id)
 {
	if( Pokemod_Alive(id) )
		SetSpeed(id);
 }
 
 ////////////////////////////////////////////////////////
 //  This is a loop that is being called every second  //
 //         some pokemon skills use this               //
 ////////////////////////////////////////////////////////
 public pokemon_loop()
 {
	if( PMon() ){

		//creating things here so we dont create them each time in the for loop
		static players[MAX_PLAYERS], pnum, id, i;
		poke_get_players(players, pnum, PLAYER_ALIVE);

		static aid, ick, aTeam;

		if( ++gtime >= 20 )
			gtime = 0;

		for( i = 0; i < pnum; i++ )
		{
			id = players[i];

			// update their team every second
			Pokemod_Team(id, true);

			if( BindUsed[id]>0.0 && BindUsed[id]-get_gametime() <= 0.0 ){
				BindUsed[id] = 0.0;
			}

			if( !is_user_bot(id) )
				PokeDisplay(id,id);

			if( PM_disabled != PM_ON )
				continue;

			if( entity_in_water(id) )
				InWater(id);

			if( g_Status[id][STATUS_FROZEN] ){
				AddStatus(id, STATUS_FROZEN, -1);
				Freeze(id);
			}

			skills_handler(id, SI_LOOP);

			if( !is_user_bot(id) && HasSilphScope(id) != MAX_ITEMS ){

				for(ick = 0; ick<pnum; ick++)
				{
					aid = players[ick];

					if( id == aid )
						continue;

					if( PokeType[Pokemon[aid]] & TYPE_GHOST ){ //theyre a ghost so lets play the sprite

						if(fm_is_ent_visible(id, aid)){

							aTeam = Pokemod_Team(aid);

							//Make a line
							MakeLine_Ent(id,id,aid,SPRITE_INDEX[SPR_ZBEAM6],1,1,10,10,0,TeamColors[aTeam][0],TeamColors[aTeam][1],TeamColors[aTeam][2],200,0);
						}
					}
				}
			}
		}
		poke_get_players(players, pnum, PLAYER_DEAD|PLAYER_REAL);
		for( i = 0; i < pnum; i++ )
		{
			id = players[i];
			aid = pev(id, pev_iuser2);
			if( id != aid )
				PokeDisplay(id,aid);
		}
	}
 }

 ////////////////////////////////////
 //  When a client says something  //
 ////////////////////////////////////
 public HandleSay(id)
 {
	static said[192], cow;
	read_args(said,191);
	remove_quotes(said);

	//remove the / if it is there
	if( equali(said,"/",1 ) ) cow = 1;
	else cow = 0;

	if((containi(said[cow], "say") != -1 ) || (containi(said[cow], "type") != -1 ))
		return PLUGIN_CONTINUE;
	if( !PMon() ){
		if(containi(said[cow], "poke") != -1) {
			poke_printlang(id,"POKEMOD_DISABLED")
			return CHAT_SPAM
		}
	}
	else {
		new first_message[21], dummy[2]
		strbreak(said[cow], first_message, 20, dummy, 1)

		if((equali(first_message,"help",4 ) && containi(first_message, "poke") != -1 )
		|| (equali(first_message,"poke",4 ) && containi(first_message, "help") != -1 )
		) {
			pokehelp(id)
			return CHAT_SPAM
		}
 #if POKERANK_SYSTEM==1
		else if( equali(first_message,"pokerank",8)
		|| equali(first_message,"pokemaster",10)
		|| equali(first_message,"elite4",6)
		|| equali(first_message,"elitefour",9)
		){
			PlayersRanks(id, true)
			return CHAT_SPAM
		}
 #endif
 #if ALLOW_WILD==1
		else if(equali(first_message,"trainer")){
			if(g_isWild[id]>0){
				g_isWild[id] = -1
				UnAssignWild(id)
			}
			else
				poke_printlang(id, "POKEMOD_TRAINERALRDY")
			return CHAT_SPAM
		}
		else if(equali(first_message,"wild")){
			if(!g_isWild[id]){
				g_isWild[id] = 1
				AssignWild(id, true)
			}
			else{
				if(g_isWild[id]==1)
					poke_printlang(id, "POKEMOD_WILDALRDY")
				else
					poke_printlang(id, "POKEMOD_WILDDIE")
			}
			return CHAT_SPAM
		}
 #endif
		else if(equali(first_message,"pokecommand",11)
		|| (containi(said[cow],"what")!=-1 && containi(said[cow],"bind")!=-1 && (containi(said[cow],"skill")!=-1 || containi(said[cow],"power")!=-1 || containi(said[cow],"poke")!=-1))){
			pokecommands(id)
			return CHAT_SPAM
		}
		else if(equali(first_message, "pokedex")) {
			pokedex(id, said[cow], true)
			return CHAT_SPAM
		}
		else if(equali(first_message, "pokeitem")) {
			new Left[NAME_LENGTH], Right[151]
			strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150);
			strbreak(Right, Left, NAME_LENGTH-1, Right, 1);
			itemhelp(id, NameToItem(Left,false), true);
			return CHAT_SPAM
		}
		else if(equali(first_message, "pokemenu")) {
			new center = NearOakCheck(id)
			if(center >= 0){
				new parm[2]
				parm[0] = id
				parm[1] = center
				set_task(0.1,"PokeMenu",0,parm,2)
			}
			else{
				poke_printlang(id, "POKEMOD_OAKCLOSEDEAD")
			}
			return CHAT_SPAM
		}
		else if(equali(first_message, "pokehud")) {
			new Left[25], Right[151]
			strbreak(said[cow], Left, 24, Right, 150)
			strbreak(Right, Left, 24, Right, 150)
			if(equali(Left, "up"))
				PlayersSet[id][SET_HUD_Y]--
			else if(equali(Left, "down"))
				PlayersSet[id][SET_HUD_Y]++
			return CHAT_SPAM
		}
		else if(equali(first_message, "mypoke", 6)) {
			MyPoke(id)
			return CHAT_SPAM
		}
		else if(	( (containi(said[cow], "how") != -1) || (containi(said[cow], "?") != -1) )
		&&			( (containi(said[cow], "poke") != -1) || (containi(said[cow], "power") != -1) || (containi(said[cow], "skill") != -1) )
		){
			poke_printlang(id, "POKEMOD_HELPWITH")
			return CHAT_SPAM
		}
		else if(containi(said[cow], "poke") != -1
		&& ((containi(said[cow], "maker") != -1)||(containi(said[cow], "creator") != -1)||(containi(said[cow], "team") != -1))) {
			poke_print(0,print_chat, "PokeMod Team: %s", POKETEAM)
			return CHAT_SPAM
		}
		else if(equali(first_message, "theme") || equali(first_message, "poketheme")) {
			client_cmd(id,"play %s",SOUNDS[SND_THEME]);
			return CHAT_SPAM
		}
		else if(equali(first_message, "pokemart") || equali(first_message, "buy")) {
			if(!get_ppcvar_num(pm_pokemart_open)){
				poke_printlang(id, "POKEMOD_MARTCLOSE")
				return CHAT_SPAM
			}

			new Left[NAME_LENGTH], Right[151], cat, item, amount;
			strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150);
			strbreak(Right, Left, NAME_LENGTH-1, Right, 150);

			if( equali(Left, EMPTY) )
				ShopMenu( id );

			item = NameToItem(Left, false);
			cat = NameToCat(Left);

			if( cat == MAX_ITYPE && item == MAX_ITEMS ){
				poke_printlang(id, "POKEMOD_NOITEM")
				return CHAT_SPAM;
			}
			if( cat != MAX_ITYPE ){
				ShopMenu(id, cat);
				return CHAT_SPAM;
			}

			strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
			amount = str_to_num(Left)
			if(amount < 1)
				amount = 1

			#if MAX_AMOUNT_TO_BUY > 0
			if( amount > MAX_AMOUNT_TO_BUY ){
				poke_printlang(id, "POKEMOD_MAXBUY",MAX_AMOUNT_TO_BUY)
				return CHAT_SPAM
			}
			#endif

			buy_item(id, item, amount)
			return CHAT_SPAM
		}
		else if(equali(first_message, "sell")) {
			new Left[NAME_LENGTH], Right[151], item, amount
			strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150)
			strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
			item = NameToItem(Left,false)
			if(!item){
				poke_printlang(id, "POKEMOD_NOITEM")
				return CHAT_SPAM
			}
			strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
			amount = str_to_num(Left)
			if(amount < 1)
				amount = 1

			#if MAX_AMOUNT_TO_BUY > 0
			if( amount > MAX_AMOUNT_TO_BUY ){
				poke_printlang(id, "POKEMOD_MAXSELL",MAX_AMOUNT_TO_BUY)
				return CHAT_SPAM
			}
			#endif

			SellItem(id, item, amount)
			return CHAT_SPAM
		}
		else if(containi(first_message, "player") != -1 && ((containi(first_message, "skill") != -1)||(containi(first_message, "poke") != -1)||(containi(first_message, "power") != -1))) {
			PlayersPokemon(id, true)
			return CHAT_SPAM
		}
		else if(equali(first_message, "cancel")) {
			if(task_exists(id+RELEASE_TASK)){
				remove_task(id+RELEASE_TASK)
				remove_task(id+RELEASE2_TASK)
				poke_printlang(id, "POKEMOD_RELCANC")
			}
			else
				poke_printlang(id, "POKEMOD_NOTREL")
			return CHAT_SPAM
		}
		else if(equali(first_message, "finalrelease")) {
			if(task_exists(id+RELEASE_TASK)){
				remove_task(id+RELEASE2_TASK)
				change_task( id+RELEASE_TASK, 0.1 )
			}
			else{
				new Left[NAME_LENGTH], Right[151], pid
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150)
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
				if( (pid = NameToPoke(Left, true, id)) ){
					new releaseparm[2];
					releaseparm[0] = id;
					releaseparm[1] = pid;
					FinalRelease(releaseparm);
				}
				else
					poke_printlang(id, "POKEMOD_NOTREL")
			}
			return CHAT_SPAM
		}
		else if(containi(first_message, "player") != -1 && containi(first_message, "item") != -1) {
			PlayersItems(id, true)
			return CHAT_SPAM
		}
		else if(equali(first_message, "use")) {
			if( Pokemod_Alive(id) ){
				if( PokeItem(id) )
					ItemUseMenu(id);
				new aid, body;
				get_user_aiming(id, aid, body);
				if( !EntIsPC(aid) )
					return CHAT_SPAM
			}
			ShopMenu(id);
		}
		else if(equali(first_message,"save") || equali(first_message,"pokesave")){
			if(g_isWild[id]>0){
				poke_printlang(id, "POKEMOD_WILDSAVE")
				return CHAT_SPAM
			}
			else{
				if(SaveXPon()){
					if( SaveXP(id, 0) )
						poke_printlang(id, "POKEMOD_SAVED")
					return CHAT_SPAM
				}
				else{
					poke_printlang(id, "POKEMOD_NOSAVE")
					return PLUGIN_HANDLED
				}
			}
		}
		else if( g_isWild[id] < 1 ){
			if(equali(first_message, "switch")) {

				new Left[NAME_LENGTH], Right[151]
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150) //isolate the command

				strbreak(Right, Left, NAME_LENGTH-1, Right, 150) //isolate first thing
				new temp = NameToPoke(Left, _, id)
				if(!temp){
					poke_printlang(id, "POKEMOD_NOPOKE1")
					return CHAT_SPAM
				}

				strbreak(Right, Left, NAME_LENGTH-1, Right, 150) //isolate second thing
				new temp2 = NameToPoke(Left, _, id)
				if(!temp2){
					poke_printlang(id, "POKEMOD_NOPOKE2")
					return CHAT_SPAM
				}
				if(temp==temp2){
					poke_printlang(id, "POKEMOD_SWITCHSAME")
					return CHAT_SPAM
				}

				new slot = Slot(id, temp, CHECK_ALL)
				new slot2 = Slot(id, temp2, CHECK_ALL)

				if( !ValidSlot( slot ) || !ValidSlot( slot2 ) ){
					poke_printlang(id, "POKEMOD_SWITCHHAVE")
					return CHAT_SPAM
				}

				if( (slot>=MAX_ACTIVE_POKES || slot2>=MAX_ACTIVE_POKES) && NearOakCheck(id)==-1 ){
					poke_printlang(id, "POKEMOD_SWITCHPC")
					return CHAT_SPAM
				}

				//save the thing in slot
				new PlaceHolder = PlayersXP[id][slot];

				//overwrite thing in slot with slot2
				PlayersPoke[id][slot] = temp2;
				PlayersXP[id][slot] = PlayersXP[id][slot2];

				//overwrite thing in slot2 with saved thing
				PlayersPoke[id][slot2] = temp;
				PlayersXP[id][slot2] = PlaceHolder;

				RefreshLevels(id);

				if( Pokemon[id] == temp ) PlayersActive[id] = slot2;
				else if( Pokemon[id] == temp2 ) PlayersActive[id] = slot;

				return CHAT_SPAM;
			}
			if(equali(first_message, "send")) {

				new Left[NAME_LENGTH], Right[151]
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150) //isolate the command

				strbreak(Right, Left, NAME_LENGTH-1, Right, 150) //isolate first thing
				new temp = NameToPoke(Left, _, id)
				if(!temp){
					poke_printlang(id, "POKEMOD_NOPOKE")
					return CHAT_SPAM
				}

				new slot = Slot(id, temp, CHECK_ALL)
				new slot2 = Slot(id, 0, CHECK_PC)

				if( PlayersActive[id] == slot ){
					poke_printlang(id, "POKEMOD_PCNOACTIVE")
					return CHAT_SPAM
				}
				if( !ValidSlot(slot) ){
					poke_printlang(id, "POKEMOD_PCSENDHAVE")
					return CHAT_SPAM
				}
				if( !ValidSlot(slot2) ){
					poke_printlang(id, "POKEMOD_PCSENDROOM")
					return CHAT_SPAM
				}

				if((slot>=MAX_ACTIVE_POKES || slot2>=MAX_ACTIVE_POKES)&& NearOakCheck(id)==-1){
					poke_printlang(id, "POKEMOD_SENDCLOSE")
					return CHAT_SPAM
				}

				//overwrite thing in slot2 with slot
				PlayersPoke[id][slot2] = PlayersPoke[id][slot]
				PlayersXP[id][slot2] = PlayersXP[id][slot]

				//overwrite thing in slot2 with saved thing
				PlayersPoke[id][slot] = 0
				PlayersXP[id][slot] = 0

				RefreshLevels(id);

				poke_printlang(id, "POKEMOD_PCSENT", PokeToName(PlayersPoke[id][slot2]));

				if( !SwitchPoke(id, Pokemon[id]) )
					SwitchPoke(id, PlayersPoke[id][PlayersActive[id]])

				return CHAT_SPAM
			}
			if(equali(first_message, "retrieve")) {

				new Left[NAME_LENGTH], Right[151]
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150) //isolate the command

				strbreak(Right, Left, NAME_LENGTH-1, Right, 150) //isolate first thing
				new temp = NameToPoke(Left, _, id)
				if(!temp){
					poke_printlang(id, "POKEMOD_NOPOKE")
					return CHAT_SPAM
				}

				new slot = Slot(id, 0, CHECK_ALL)
				new slot2 = Slot(id, temp, CHECK_PC)

				if( !ValidSlot(slot) ){
					poke_printlang(id, "POKEMOD_PCRETROOM")
					return CHAT_SPAM
				}
				if( !ValidSlot(slot2) ){
					poke_printlang(id, "POKEMOD_PCRETHAVE")
					return CHAT_SPAM
				}

				if( (slot>=MAX_ACTIVE_POKES || slot2>=MAX_ACTIVE_POKES) && NearOakCheck(id)==-1 ){
					poke_printlang(id, "POKEMOD_RETCLOSE")
					return CHAT_SPAM
				}

				//overwrite thing in slot with slot2
				PlayersPoke[id][slot] = PlayersPoke[id][slot2]
				PlayersXP[id][slot] = PlayersXP[id][slot2]

				//overwrite thing in slot2 with saved thing
				PlayersPoke[id][slot2] = 0
				PlayersXP[id][slot2] = 0

				RefreshLevels(id);

				poke_printlang(id, "POKEMOD_PCRET", PokeToName(PlayersPoke[id][slot]));

				return CHAT_SPAM
			}
			if(equali(first_message, "release")) {
				new Left[NAME_LENGTH], Right[151], pid
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150)
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
				pid = NameToPoke(Left, _, id)

				if( pid )
					ReleasePoke(id, pid)
				else{
					#if RELEASE_MENU == 1
						SetActionMenu(id, AC_RELEASE )
					#else
						poke_printlang(id, "POKEMOD_NOPOKE")
					#endif
				}

				return CHAT_SPAM
			}
			else if(equali(first_message, "go")) {
				new Left[NAME_LENGTH], Right[151], pid
				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150)
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
				pid = NameToPoke(Left, _, id)
				if(!SwitchPoke(id, pid, false) && equali(said[cow], "go")){
					SetActionMenu( id, AC_GO )
				}
				return CHAT_SPAM
			}
			else if(equali(first_message, "give")) {
				new Left[NAME_LENGTH], Right[151], Name[32], pid, players[MAX_PLAYERS], pnum

				strbreak(said[cow], Left, NAME_LENGTH-1, Right, 150)//get rid of the command
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)//get the name

				if( get_info(id, Left, Name,31, false, players, pnum, false) ){
					if( pnum == 1 ){
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)//get the item name
						pid = NameToItem(Left)

						if( pid != MAX_ITEMS ){
							strbreak(Right, Left, NAME_LENGTH-1, Right, 150)//get how many
							TradeItem(id, pid, players[0], str_to_num(Left))
						}
						else{
							pid = NameToPoke(Left)
							if( !TradePoke(id, pid, players[0]) )
								poke_printlang(id, "POKEMOD_NOPOKEITEM")
						}
						return CHAT_SPAM
					}
				}
				poke_printlang(id, "POKEMOD_NOPLAYER")
				return CHAT_SPAM
			}
		}
	}
	return PLUGIN_CONTINUE
 }

 ///////////////////////////////
 //  Blocks FullUpdate Abuse  //
 ///////////////////////////////
 public fullupdate(id)
	return PLUGIN_HANDLED_MAIN

 //////////////////////////////////////
 //  When something touches another  //
 //////////////////////////////////////
 public touches(ptr, ptd)
 {
	//allrighty... lets break it down!

	if( PM_disabled == PM_OFF || !pev_valid(ptr) )
		return PLUGIN_CONTINUE;

	static szClass[32];
	pev(ptr, pev_classname, szClass, 31);

	if(equali(szClass, "poke_rock"))
		rock_touch(ptr,ptd);
	else if(equali(szClass, "psy_beam"))
		psybeam_touch(ptr,ptd);
	else if(equali(szClass, "poke_attack"))
		attack_touch(ptr,ptd);
	else if(equali(szClass, "player"))
		player_touch(ptr,ptd);

	return PLUGIN_CONTINUE;
 }
 ///////////////////////////////////////
 //  When a player touches something  //
 ///////////////////////////////////////
 player_touch(ptr, ptd)
 {
	if( !PMon() || !Pokemod_Alive(ptr) || !pev_valid(ptd) )
		return PLUGIN_CONTINUE;

	#if TOUCH_SKILL_TEAM == 0
	if( !Pokemod_FF(ptr, ptd) ){
	#endif

	PlayerDamageInfo[ptr][PD_ATTACKER] = ptd;
	PlayerDamageInfo[ptr][PD_DAMAGE] = PlayerDamageInfo[ptr][PD_WEAPON] = 0;
	skills_handler(ptr, SI_TOUCH);
	PlayerDamageInfo[ptr][PD_ATTACKER] = 0;

	#if TOUCH_SKILL_TEAM == 0
	}
	#endif

	//only get classnames if it wasn't a player
	static szClassD[32];
	pev(ptd, pev_classname, szClassD, 31);

	if(equal(szClassD, "npc_pokecenter")){

		if(g_Status[ptr][STATUS_OAK_STOP])
			return PLUGIN_CONTINUE;

		debugMessage(ptr,8,"Player", "touched a pokecenter");

		//need to check if its team specific (flag 'c')
		new oak_props[MAX_OAKPROP];
		pev(ptd, pev_globalname, oak_props, MAX_OAKPROP-1);
		if(containi(oak_props, "c")!=-1 && (Pokemod_Team(ptd)!=0 && Pokemod_Team(ptr)!=Pokemod_Team(ptd))){
			if(!is_user_bot(ptr))
				poke_printlang(ptr,"POKEMOD_OTHERTEAM");
			return PLUGIN_CONTINUE;
		}

		if(is_user_bot(ptr))
			BotCenter(ptr);
		else{
			new parm[2];
			parm[0] = ptr;
			parm[1] = ptd;
			PokeMenu( parm );
		}
		return PLUGIN_CONTINUE;
	}
	if(equal(szClassD, "pokemon")){
		debugMessage(ptr,6,"Player", "touched a pokeball pokemon");

		new poke, TargName[NAME_LENGTH];
		pev(ptd, pev_targetname, TargName, NAME_LENGTH-1);

		Pokemod_Team(ptr, true);
		if( Pokemod_FF(ptr, ptd) || FFAon() ){
			poke_printlang(ptr,"POKEMOD_OTHERTEAM");
			UnSolid(ptd);
			return PLUGIN_CONTINUE;
		}
		if( !ValidSlot( Slot(ptr) ) ){
			poke_printlang(ptr,"POKEMOD_NOROOM");
			UnSolid(ptd);
			return PLUGIN_CONTINUE;
		}
		poke = NameToPoke(TargName);
		if( ValidSlot( Slot(ptr,poke) ) ){
			poke_printlang(ptr,"POKEMOD_DUPLICATE", TargName);
			UnSolid(ptd);
			return PLUGIN_CONTINUE;
		}

		if(AddPoke(ptr, poke)){
			#if CATCH_TELEPORT > 0
				EscapeRope(ptr, 0);
				poke_printlang(ptr,"POKEMOD_CATCHTELE", TargName);
			#endif

			poke_sound(ptr,CHAN_VOICE,SND_BODYDROP);
			fm_remove_entity(ptd);
			remove_task(ptd+REMOVE_TASK);

			#if POKEBALL_RESPAWN == 1
			SpawnPokeballs();
			#endif

			return PLUGIN_CONTINUE;
		}
	}
	if(equal(szClassD, "pokeitem")){
		debugMessage(ptr,6,"Player", "touched a pokeitem");

		new bool:given=false, item, amount, Left[9], Right[151], amountgiven, ilen, newtargetname[151];

		pev(ptd, pev_targetname, Right, 150);

		while( ValidData(Right) ){
			strbreak(Right, Left, 8, Right, 150);
			item = str_to_num(Left);
			strbreak(Right, Left, 8, Right, 150);
			amount = str_to_num(Left);
			if( ValidItem(item) && amount ){
				amountgiven = GiveItem(ptr, item, amount);
				if( amountgiven == amount )
					given = true;
				else
					ilen += formatex(newtargetname[ilen], (151-ilen), "%d %d ", item, amount - amountgiven);
			}
		}

		if( given ){
			poke_sound(ptd,CHAN_VOICE,SND_BODYDROP);
			#if AUTO_ITEMS > 0
				AutoItems(ptr);
			#endif
		}
		else{
			poke_printlang(ptr,"POKEMOD_NOPICKUP");
		}

		if( !ValidData(newtargetname) ){
			fm_remove_entity(ptd);
			remove_task(ptd+REMOVE_TASK);
		}
		else{
			set_pev(ptd, pev_targetname, newtargetname);
			UnSolid(ptd);
		}

		return PLUGIN_CONTINUE;
	}
	if(equal(szClassD,"grenade")){
		new model[40];
		pev(ptd, pev_model, model, 39 );
		if( equali(model, MODELS[MDL_POKEBALL] ) ){
			debugMessage(ptr,5,"Player", "touched a poke-grenade")

			if(ptd==PlayerEnt[ptr]){
				fm_remove_entity(ptd);
				set_task(1.0,"delay_grenade",ptr);
				PlayerEnt[ptr] = 0;
				set_bind(ptr);
				poke_sound(ptr,CHAN_VOICE,SND_BODYDROP);
			}
			else {
				for( new i=1; i < MAX_PLAYERS; i++ ){
					if(PlayerEnt[i]==ptd){
						sound_cast(i);
						set_pev(ptd, pev_dmgtime, 0.1);
						set_pev(ptd, pev_nextthink, 0.1);
						set_pev(ptd, pev_owner, i);
						PlayerEnt[i] = 0;
						break;
					}
				}
			}
		}
		return PLUGIN_CONTINUE;
	}
	return PLUGIN_CONTINUE;
 }
 public delay_grenade(id)
	give_grenade(id);
 
 //////////////////////////////////////////
 //  When a poke rock touches something  //
 //////////////////////////////////////////
 attack_touch(ptr, ptd)
 {
	if( !pev_valid(ptr) )
		return;

	new aid = pev(ptr, pev_owner);
	new damage = pev(ptr, pev_iuser1);
	new skill = pev(ptr, pev_iuser2);
	new szClass[32]
	SkillToName( skill, szClass, 31 );

	debugMessage(0,3,"Skill", "%s touched something", szClass);

	if( damage && aid != ptd && Pokemod_Alive(ptd) )
		skilldamage(ptd, aid, damage, skill);

	if(equali(szClass, "Spike Cannon")){
		set_pev(ptr,pev_velocity,Float:{0.0,0.0,0.0});
		poke_sound(ptr, CHAN_WEAPON, SND_HIT);
		set_pev(ptr, pev_iuser1, 0);
		set_task(0.3,"poke_remove_entity",ptr+REMOVE_TASK);
	}
	else if(equali(szClass, "Pin Missile")){
		new Float:temporigin[3];
		if( Pokemod_Alive(ptd) && ptd != aid ){
			pev(ptr, pev_velocity, temporigin);
			temporigin[2] = floatmax(temporigin[2], 10.0);
			set_pev(ptd, pev_velocity, temporigin);
		}

		if( pev_valid(ptr) ){
			pev(ptr, pev_origin, temporigin);
			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
			write_byte(TE_SPRITE)
			write_coord(floatround(temporigin[0])) // start position
			write_coord(floatround(temporigin[1]))
			write_coord(floatround(temporigin[2]))
			write_short(SPRITE_INDEX[SPR_BIGSPIT]) // sprite index
			write_byte(10) // scale in 0.1's
			write_byte(100) // brightness
			message_end()

			poke_sound(ptr, CHAN_WEAPON, SND_HIT);
		}
		poke_remove_entity(ptr+REMOVE_TASK);
	}
	else if(equali(szClass, "Bone Club")){
		set_pev(ptr, pev_velocity, Float:{0.0,0.0,0.0});
		set_pev(ptr, pev_iuser1, 0);
		set_pev(ptr, pev_sequence, 0);
		if( ptd == aid ){
			poke_sound(ptd, CHAN_WEAPON, SND_BODYDROP);
			poke_remove_entity(ptr+REMOVE_TASK);
		}
		else if(damage)
			poke_sound(ptr, CHAN_WEAPON, SND_HIT);
	}
	else if(equali(szClass, "Bonemerang")){

		entity_set_follow(ptr, aid, 1.0*BONE_VEC);

		if( ptd == aid ){
			poke_sound(ptd, CHAN_WEAPON, SND_BODYDROP);
			poke_remove_entity(ptr+REMOVE_TASK);
		}
		else
			poke_sound(ptr, CHAN_WEAPON, SND_HIT);
	}
	else{
		poke_remove_entity(ptr+REMOVE_TASK);
	}
 }
 
 //////////////////////////////////////////
 //  When a poke rock touches something  //
 //////////////////////////////////////////
 rock_touch(ptr, ptd)
 {
	new skill = pev(ptr, pev_iuser2);
	new szClass[32]
	SkillToName( skill, szClass, 31 );

	debugMessage(0,3,"Skill", "%s touched something", szClass);

	new aid = pev(ptr, pev_owner)
	if( aid == ptd )	return PLUGIN_CONTINUE

	new maxdamage = ROCK_DAMAGE
	AddLvl(aid, maxdamage)

	if( !ValidPlayer(ptd) ){
		if( PokeEntDamage(ptd, szClass, maxdamage) ){
			rock_touch(ptr, 0);
			return PLUGIN_CONTINUE;
		}
	}

	new spritetoshow;
	new damradius = 200;
	new Float:fl_vExplodeAt[3];
	pev(ptr, pev_origin, fl_vExplodeAt);
	AddLvl(aid, damradius);

	if( SkillType[skill]&TYPE_FIRE && !entity_in_water(ptr) ){
		spritetoshow = SPRITE_INDEX[SPR_EEXPLO];
		skillAoEdamage(aid, fl_vExplodeAt, damradius, maxdamage, skill, _, _, STATUS_BURN, 2);
	}
	else {
		spritetoshow = SPRITE_INDEX[SPR_XPLODE];
		fl_vExplodeAt[2] += 60;
		skillAoEdamage(aid, fl_vExplodeAt, damradius, maxdamage, skill);
	}

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(17);
	write_coord(floatround(fl_vExplodeAt[0]));
	write_coord(floatround(fl_vExplodeAt[1]));
	write_coord(floatround(fl_vExplodeAt[2]));
	write_short(spritetoshow);
	write_byte(20);
	write_byte(200);
	message_end();

	poke_sound(ptr, CHAN_WEAPON, SND_EXM2);

	fm_remove_entity(ptr);

	return PLUGIN_CONTINUE;
 }
 
 /////////////////////////////////////////
 //  When a psy beam touches something  //
 /////////////////////////////////////////
 psybeam_touch(ptr, ptd)
 {
	new szClass[NAME_LENGTH];
	new damradius = 200;
	new maxdamage = PSYBEAM_DAMAGE;
	new aid = pev(ptr, pev_owner);
	new skill = pev(ptr, pev_iuser2);
	SkillToName(skill, szClass);

	debugMessage(0,3,"Skill", "%s touched something", szClass)

	AddLvl(aid, maxdamage);

	if( !ValidPlayer(ptd) )
		PokeEntDamage(ptd, szClass, maxdamage);

	new Float:fl_vExplodeAt[3];
	pev(ptr, pev_origin, fl_vExplodeAt);

	skillAoEdamage(aid, fl_vExplodeAt, damradius, maxdamage, skill)

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(17);
	write_coord(floatround(fl_vExplodeAt[0]));
	write_coord(floatround(fl_vExplodeAt[1]));
	write_coord(floatround(fl_vExplodeAt[2] + 60));
	write_short(SPRITE_INDEX[SPR_XPLODE]);
	write_byte(20);
	write_byte(200);
	message_end();

	poke_sound(ptr, CHAN_WEAPON, SND_EXM2);

	if(Pokemod_Alive(aid)){
		self_view(aid);
		bind_used(aid, skill);
	}
	PlayerEnt[aid] = 0;
	if( pev_valid(ptr) )
		fm_remove_entity(ptr);

	return PLUGIN_CONTINUE;
 }
 
 /////////////////////////////////////
 //  When a surf touches something  //// Note that this is not being registered, i just pretend it is. (im actually using a function to see if they are near it)
 /////////////////////////////////////
 surf_touch(ptr, ptd)
 {
	if( ptr <= 0 || !pev_valid(ptr) ) return PLUGIN_CONTINUE

	new aid = pev(ptr, pev_owner);
	new skill = pev(ptr, pev_iuser2);

	debugMessage(0,3,"Skill", "%s touched something", SkillToName(skill))

	if( aid == ptd )
		return PLUGIN_CONTINUE

	if( Pokemod_Alive(ptd) ){
		if( !Pokemod_FF(ptd, ptr) || FFon() ){
			new damage = pev(ptr, pev_iuser1);
			new Float:velocity[3];

			pev(ptr, pev_velocity, velocity)
			velocity[2] = 10.0
			set_pev(ptd, pev_velocity, velocity)

			//this will take care of the InWater
			skilldamage(ptd, aid, damage, skill)
		}
		else
			InWater(ptd)
	}
	return PLUGIN_CONTINUE
 }
 UnSolid(ent, Float:time=5.0)
 {
	if( pev_valid(ent) && !task_exists(ent+SOLID_TASK) ){
		set_pev(ent, pev_solid, SOLID_NOT);
		set_task(time, "Solidify", ent+SOLID_TASK);
	}
 }
 public Solidify(ent)
 {
	ent -= SOLID_TASK
	if( pev_valid(ent) ) set_pev(ent, pev_solid, SOLID_BBOX);
 }
 public log_block(type, msg[])
	return(log_block_state?FMRES_SUPERCEDE:FMRES_IGNORED);

 public forward_AddToFullPack(es_handle, e, entity, host, hostflags, player, pSet)
 {
	if( !player || entity==host )
		return FMRES_IGNORED

	if( Pokemod_Alive(entity) && Pokemod_Alive(host)
	&& PokeType[Pokemon[entity]] & TYPE_GHOST ){
		if( HasSilphScope(host) != MAX_ITEMS )
		{
			set_es(es_handle, ES_RenderFx, kRenderFxGlowShell);
			set_es(es_handle, ES_RenderColor, TeamColors[Pokemod_Team(entity)] );
			set_es(es_handle, ES_RenderMode, kRenderNormal);
			set_es(es_handle, ES_RenderAmt, 0);
		}
	}

	return FMRES_IGNORED
 }
 
 