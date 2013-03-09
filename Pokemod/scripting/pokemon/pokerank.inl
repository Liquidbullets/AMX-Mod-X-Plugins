
 
 #if POKERANK_SYSTEM==0
  #endinput
 #endif

 ///////////////////////////////////////////////////////////////////////
 //  When someone wants see what the pokeranks are (through console)  //
 ///////////////////////////////////////////////////////////////////////
 public Console_PlayersRanks(id)
 {
	PlayersRanks(id, false)
	return PLUGIN_HANDLED
 }
 
 /////////////////////////////////////////////////////
 //  When someone wants see what the pokeranks are  //
 /////////////////////////////////////////////////////
 public PlayersRanks(id, bool:motd)
 {
	new text[1501], len = 1500, n, temp[256], iLen
	new players[MAX_PLAYERS], pnum, pid, name[32]
	poke_get_players(players, pnum)

	new bool:its_a_motd = true
	#if MOD==NS
		its_a_motd = false
	#endif

	if(motd){
		if(its_a_motd)
			n += copy(text[n],len-n,"<center><b>Player's PokeRanks</b></center><br>")
		else
			n += copy(text[n],len-n,"Player's PokeRanks<br>")
		n += formatex(text[n],len-n,"Pokemon Master:  %s<br>",RankName[0])
		n += formatex(text[n],len-n,"Elite Four:  %s   %s   %s   %s<br>",RankName[1],RankName[2],RankName[3],RankName[4])
		copy(temp, 255, EMPTY)
	}
	else{
		console_print(id, ":::Player's PokeRanks:::")
		console_print(id, ":Pokemon Master:  %s",RankName[0])
		console_print(id, ":Elite Four:  %s   %s   %s   %s",RankName[1],RankName[2],RankName[3],RankName[4])
	}

	new x, teamName[71]
	for(new team = 1; team < 4; team++ ){
		if(motd){
			GetTeamName(team, its_a_motd, teamName, 70)
			n += copy(text[n],len-n,teamName)
		}
		else
			PrintTeamName(team, id)
		for( x = 0; x < pnum; x++){
			pid = players[x]
			if ( Pokemod_Team(pid) != team ) continue
			get_user_name(pid, name, 31)
			if(motd){
				if(its_a_motd){
					replace_all(name, 31, "<", "(")	//things that screw up the motd
					replace_all(name, 31, ">", ")")
					iLen += formatex(temp[iLen], (255-iLen), "<br><b>%s</b> - %s(%d)", name,PokeRankNames[getPokeRank(pid)],PlayersCatch[pid])
				}
				else
					iLen += formatex(temp[iLen], (255-iLen), "<br>%s - %s(%d)", name,PokeRankNames[getPokeRank(pid)],PlayersCatch[pid])
			}
			else
				console_print(id, "::%s - %s(%d)",name,PokeRankNames[getPokeRank(pid)],PlayersCatch[pid])

			if(motd)
				n += copy(text[n],len-n,temp)

			copy(temp, 255, EMPTY)
			iLen = 0
		}
	}

	if(motd){
		if(!its_a_motd){
			replace_all(text, 1500, "<li>", "\n-")
			replace_all(text, 1500, "<br>", "\n")
			set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
			show_hudmessage(id,text)
		}
		else{
			show_motd( id, text, "Player's PokeRanks" )
		}
	}
	return PLUGIN_HANDLED
 }
 getPokeRank(id)
 {
	new i=0
	while(PlayersCatch[id]>=PokeRanks[i]){
		if(i+1==MAX_RANKS)
			return MAX_RANKS-1
		i++
	}
	return i
 }
 
 LoadRanks()
 {
	new Right[151];

	#if SAVE==SAVE_NVAULT
	new save_key[51], time;
	#endif

	#if SAVE==SAVE_VAULT
	new save_key[51];
	#endif

	#if SAVE==SAVE_MYSQL
	new sql[512], error[128];
	new Result:retvalxp;
	#endif

	for(new i=0; i<5; i++){

	#if SAVE==SAVE_NVAULT
		formatex(save_key,50,"PokeMod_Rank%d", i)
		nvault_lookup(nvault_db, save_key, Right, 150, time)
	#endif
	#if SAVE==SAVE_VAULT
		formatex(save_key,50,"PokeMod_Rank%d", i)
		get_vaultdata(save_key, Right, 150)
	#endif
	#if SAVE==SAVE_MYSQL
		mySQLConnect()
		if ( gMySQL <= SQL_FAILED ) return false
		
		formatex(sql, 511, "SELECT `XPS` FROM `pm_save` WHERE `SAVE_KEY` = '_Rank%d'",i)
		retvalxp = dbi_query(gMySQL,sql)

		if(retvalxp <= RESULT_FAILED){
			dbi_error(gMySQL,error,127)
			debugMessage(0,0,"Rank", "No Ranks Loaded: '%s'",error)

			//Closes the connection if set not to be persistent
			close_mysql()
			return true
		}
		else if (!dbi_nextrow(retvalxp)) {
			dbi_error(gMySQL,error,127)
			debugMessage(0,0,"Rank", "No Saved Ranks Loaded: '%s'", error)

			if (retvalxp >= RESULT_OK) dbi_free_result(retvalxp)

			//Closes the connection if set not to be persistent
			close_mysql()

			return true
		}
		dbi_field(retvalxp, 1, Right, 150)
	#endif

		HandleRanks(i,Right)

	}
	return true
 }
 
 HandleRanks(rank, Right[151])
 {
	debugMessage(0,6,"Rank", "%d loading '%s'", rank, Right);

	new temp[25]
	strbreak(Right, RankName[rank], 31, Right, 150)
	strbreak(Right, RankKey[rank], 35, Right, 150)
	strbreak(Right, temp, 24, Right, 150)
	RankCatch[rank] = str_to_num(temp)

	#if SAVE==SAVE_MYSQL
		replace_all(RankKey[rank],35,"`","\\`");
		replace_all(RankKey[rank],35,"'","\\'");
	#endif

	return true
 }
 
 SpecialRank(id)
 {
	new key[35]
	if(getSaveKey(id,key)){
		for(new i=0; i<5; i++){
			if(equali(key,RankKey[i]))
				return i
		}
	}
	return -1
 }
 UpdateSpecialRank(id)
 {
	new oldrank = SpecialRank(id)

	//don't need to check if they are higher than anyone if they are the top
	if(oldrank > 0){
		new newrank = oldrank
		while(newrank > 0 && PlayersCatch[id] > RankCatch[newrank-1]) // With this newrank can equal to 0.
		{
			newrank--
		}

		if(newrank==oldrank){
			get_user_name(id, RankName[oldrank], 31)
			RankCatch[oldrank] = PlayersCatch[id]
			return
		}

		for(new k = oldrank; k > newrank; k--){
			if(k==0)
				break

			copy(RankName[k],	31,	RankName[k-1])
			copy(RankKey[k],	34,	RankKey[k-1])
			RankCatch[k] = RankCatch[k-1]
		}

		get_user_name(id, RankName[newrank], 31)
		getSaveKey(id, RankKey[newrank])
		RankCatch[newrank] = PlayersCatch[id]

		if(newrank==0)
			poke_print(0, print_chat, "Congratulations %s! You are the Pokemon Master!",RankName[0])
		else
			poke_print(id, print_chat, "You are now a higher member in the Elite Four!")

		SaveRanks();
	}
 }
 CheckUpdateRank(id)
 {
	if(PlayersCatch[id]>RankCatch[4]){
		get_user_name(id, RankName[4], 31)
		getSaveKey(id, RankKey[4])
		RankCatch[4] = PlayersCatch[id]
		poke_print(0, print_chat, "Congratulations %s! You have become a member of the Elite Four!",RankName[4])
		UpdateSpecialRank(id)
	}
 }
 
 SaveRanks()
 {
	new ranks[5][101], temp_rankname[32], i;
	for( i=0; i<5; i++){
		copy(temp_rankname, 31, RankName[i]);
		replace_all(temp_rankname,31," ","_");

		#if SAVE==SAVE_MYSQL
			replace_all(temp_rankname,100,"`","\\`");
			replace_all(temp_rankname,100,"'","\\'");
		#endif

		formatex(ranks[i],100,"%s %s %d", temp_rankname,RankKey[i],RankCatch[i]);
	}

	#if SAVE==SAVE_MYSQL
		mySQLConnect()
		if ( gMySQL <= SQL_FAILED ) return

		debugMessage(0, 8,"Rank", "Trying to save Rank data")

		new error[128],sql[512]
		new Result:retval, Result:retvalins

		for( i=0; i<5; i++){

			//Thanks to HC for help with writing more efficient queries
			//Check if this user has an entry already, if not make one
			formatex(sql, 511, "SELECT * FROM `pm_save` WHERE `SAVE_KEY` = '_Rank%d'",i)
			retval = dbi_query(gMySQL,sql)

			if (retval <= RESULT_FAILED) {
				dbi_error(gMySQL,error,127)
				debugMessage(0,0,"Rank", "Error Querying MySQL DB for Rank %d: '%s' - '%s'", i, error, sql)
				return
			}
			else if (!dbi_nextrow(retval)) {
				formatex(sql, 511, "INSERT INTO `pm_save` (SAVE_KEY) VALUES ('_Rank%d')", i)
				retvalins = dbi_query(gMySQL,sql)
				if (retvalins <= RESULT_FAILED) {
					dbi_error(gMySQL,error,127)
					debugMessage(0,0,"Rank", "Error Writing MySQL XP for Ranks: '%s' - '%s'", error, sql)
					return
				}
			}

			if (retval >= RESULT_OK) dbi_free_result(retval)

			//Update users entry with current data
			formatex(sql, 511, "UPDATE `pm_save` SET `XPS`='%s' WHERE (SAVE_KEY='_Rank%d')",ranks[i],i)
			debugMessage(0, 8,"Rank", "Trying to save rank(%d) data(%s)",i, ranks[i])
			retval = dbi_query(gMySQL,sql)
			if (retval <= RESULT_FAILED) {
				dbi_error(gMySQL,error,127)
				debugMessage(0,0,"Rank", "Error Writing MySQL Rank: '%s' - '%s'", error, sql)
				return
			}
		}

		//Closes the connection if set not to be persistent
		close_mysql()
	#endif

	#if SAVE==SAVE_VAULT
		new save_key[51];
		debugMessage(0, 8,"Rank", "Trying to save ranks data")

		for( i=0; i<5; i++){
			debugMessage(0, 8,"Rank", "Trying to save rank(%d) data(%s)",i, ranks[i])
			formatex(save_key, 50, "PokeMod_Rank%d", i);
			set_vaultdata(save_key, ranks[i]);
		}
	#endif

	#if SAVE==SAVE_NVAULT
		new save_key[51];
		debugMessage(0, 8,"Rank", "Trying to save ranks data")

		for( i=0; i<5; i++){
			debugMessage(0, 8,"Rank", "Trying to save rank(%d) data(%s)",i, ranks[i])
			formatex(save_key, 50, "PokeMod_Rank%d", i);
			nvault_set(nvault_db, save_key, ranks[i])
		}
	#endif
 }
 
 