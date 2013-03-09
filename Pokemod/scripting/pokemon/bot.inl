
 /////////////////////////////
 //  Radio command for bot  //
 /////////////////////////////
 BotSay(id, BOT_SAYS:type, num, num2=0)
 {
	if( get_ppcvar_num(pm_botsays) < 1 || !Pokemod_Alive(id) || FFAon() ) return;

	new players[MAX_PLAYERS], pnum;
	poke_get_players(players,pnum,PLAYER_REAL|PLAYER_INTEAM,Pokemod_Team(id));

	if(!pnum)
		return;

	new tid, name[32];
	get_user_name(id,name,31);

	for(new i = 0; i<pnum; i++){

		tid = players[i];

		switch(type)
		{
			case BOT_BOUGHT_ITEM:{	//bought something
				poke_printlang(tid,"POKEMOD_BOTBUY", name, num, ItemToName(num2));
			}

			case BOT_SELECT_POKE:{	//chose pokemon
				poke_printlang(tid,"POKEMOD_BOTGO", name, PokeToName(num));
			}
		}
	}
 }
 
 ////////////////////////
 //  Lets bot use oak  //
 ////////////////////////
 BotCenter(id)
 {
	if( Pokemon[id] ){
		new ran = random_num(0,TOTAL_POKES);
		if( ran < TOTAL_POKES/2 )
			BotMart(id);		//try to make them buy an item
		else if( ran < (TOTAL_POKES*2)/3 )
			BotSelect(id);	//try to make them change pokemon
	}
	else{
		BotPoke(id);			//try to give them a pokemon
	}
 }
 
 //////////////////////////
 //  Lets bot buy items  //
 //////////////////////////
 BotMart(id)
 {
	if( !PokeItem(id) && BuyCheck(id) ){
		buy_item(id, random(ItemsCreated));
	}
 }
 
 /////////////////////////////
 //  Gives bot random poke  //
 /////////////////////////////
 BotPoke(id)
 {
	new ran = random_num(1,PokesCreated);
	if(Safari()){
		while(!SafariLevel(ran))
			ran = random_num(1,PokesCreated);
	}
	else if(!PlayersPoke[id][0]){
		while( !ValidPoke(ran) )
			ran = random_num(1,PokesCreated);

		PlayersPoke[id][0] = ran;
		PlayersXP[id][0] = PokeLevels[BOT_START_LEVEL];
		PlayersLevels[id][0] = BOT_START_LEVEL;
		PlayersActive[id] = 0;
	}
	else
		ran = 0;

	if( ran ){
		Pokemon[id] = ran;
		BotSay(id, BOT_SELECT_POKE, ran);
	}
 }
 
 /////////////////////////////////////////////////////
 //  Makes a bot select a random pokemon they have  //
 /////////////////////////////////////////////////////
 BotSelect(id)
 {
	if(is_user_bot(id)){

		if(Safari()){
			BotPoke(id);
			return;
		}
		else if( !random(10) ){
			BotRelease(id);
			return;
		}

		#if CHANGE_POKES != 0
		if( !ChangedPoke[id] )
			return;
		else
			ChangedPoke[id] = 0;
		#endif

		new pokecount;
		for(new m=0; m<MAX_ACTIVE_POKES; m++)
			if(PlayersPoke[id][m]) pokecount++; else break;
		if(pokecount > 1){
			new rand = random(pokecount);

			if( rand != PlayersActive[id] ){
				PlayersActive[id] = rand;
				Pokemon[id] = PlayersPoke[id][rand];

				BotSay(id, BOT_SELECT_POKE, Pokemon[id]);
			}
		}
	}
 }
 //////////////////////////////////////////////////////
 //  Makes a bot release a random pokemon they have  //
 //////////////////////////////////////////////////////
 BotRelease(id)
 {
	if( AverageLevel(id) >= (TotalLevels*2)/3 ){
		new pokecount;
		for(new m=0; m<MAX_ACTIVE_POKES; m++)
			if(PlayersPoke[id][m]) pokecount++; else break;
		if(pokecount > 1)
			ReleasePoke(id, PlayersPoke[id][ random(pokecount) ], false)
	}
 }
 
 
 
 