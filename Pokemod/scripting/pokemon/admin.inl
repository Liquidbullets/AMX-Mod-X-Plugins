
 //////////////////
 //  Admin Init  //
 //////////////////
 pokeadmin_init()
 {
	//Admin Console Commands
	register_pokeadmincmd("givepoke","admin_givepoke",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon name> <level> - gives players a pokemon");
	register_pokeadmincmd("setpoke","admin_setpoke",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon name> - temporarily gives players pokemon power");
	register_pokeadmincmd("takepoke","admin_takepoke",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon | @ALL | @# | @C> - takes a pokemon away from players");
	register_pokeadmincmd("morph","admin_morph",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon | @ALL | @# | @C> <new pokemon> - morphes a pokemon into a new one");

	register_pokeadmincmd("givexp","admin_givexp",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon | @ALL | @# | @C> <amount> - gives xp to players pokemon");
	register_pokeadmincmd("setxp","admin_setxp",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon | @ALL | @# | @C> <amount> - sets xp on players pokemon");

	#if POKERANK_SYSTEM==1
	register_pokeadmincmd("addrank","admin_rank",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <amount> - gives rank points to player");
	register_pokeadmincmd("setrank","admin_rank",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <amount> - sets rank points on player");
	#endif

	register_pokeadmincmd("setlevel","admin_setlvl",ADMIN_IMMUNITY);
	register_pokeadmincmd("setlvl","admin_setlvl",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <pokemon | @ALL | @# | @C> <level> - sets level on players pokemon");

	register_pokeadmincmd("giveitem","admin_giveitem",ADMIN_IMMUNITY,"<nick | @TEAM | @ALL> <item name> <amount> - gives players an item");

	register_pokeadmincmd("debugger","admin_debugger",ADMIN_RCON,"<nick | 0> - changes who recieves the debug messages");

	#if PM_ADMIN_COMMANDS==1
	register_pokeadmincmd("help","admin_help",ADMIN_ALL," - shows admin command help");
	#endif
	#if PM_ADMIN_COMMANDS==1 && PM_CVAR_PREFIX==1
	register_pokeadmincmd("cvar","admin_cvar",ADMIN_CVAR," - saves changes to pokemod cvars");
	#endif

	register_pokeadmincmd("enable","admin_enable",ADMIN_CFG,"<pokemon | @ALL> - enables the use of a pokemon");
	register_pokeadmincmd("tempenable","admin_enable",ADMIN_CFG,"<pokemon | @ALL> - temporarily enables the use of a pokemon");
	register_pokeadmincmd("disable","admin_disable",ADMIN_CFG,"<pokemon | @ALL> - disables the use of a pokemon");
	register_pokeadmincmd("tempdisable","admin_disable",ADMIN_CFG,"<pokemon | @ALL> - temporarily disables the use of a pokemon");

	register_pokeadmincmd("pokemart","admin_pokemart",ADMIN_CFG,"<pokemon name | item name> <price> - changes items in Pokemart");
	register_pokeadmincmd("safari","admin_safari",ADMIN_CFG,"<pokemon name> <level> - changes pokemon in Safari");
	register_pokeadmincmd("reset","admin_reset",ADMIN_RCON," - resets pokemod files");
	register_pokeadmincmd("loadconfig","admin_loadconfig",ADMIN_CFG," - loads pokemod config files");
	register_pokeadmincmd("erasexp","admin_erasexp",ADMIN_RCON," - erases all xp");
 }

 ///////////////////////////////////////
 //  Finds the target of the command  //
 ///////////////////////////////////////
 bool:get_info(id, arg1[], name[], namelen, bool:console=true, player_return[MAX_PLAYERS], &pnum_return, bool:admin=true)
 {
	if( (pnum_return = RCON_POKEADMINS) == 1 ){
		if( id && !(get_user_flags(id)&ADMIN_RCON) && admin ){
			player_return[0] = id;
			pnum_return = 1;
			get_user_name(id, name, namelen);
			return true;
		}
	}

	new placeholder[33]
	new players[MAX_PLAYERS], pnum, pid, found = 0;
	poke_get_players(players, pnum)
	remove_quotes(arg1)
	replace_all(arg1,31," ", EMPTY)

	for(new i=0; i<pnum; i++){
		pid = players[i]
		get_user_name(pid, placeholder, 32)
		replace_all(placeholder,32," ", EMPTY)
		if(containi(placeholder, arg1) != -1 || equali(placeholder, arg1)){
			if( found ){
				if(console)
					PokeAdminNotice(id,LANG, id,"POKEMOD_MULTUSERS")
				else
					poke_printlang( id,"POKEMOD_MULTUSERS")
				return false;
			}
			found = pid
			get_user_name(pid, name, namelen)
		}
	}
	if( found ){
		player_return[0] = found;
		pnum_return = 1;
		return true;
	}
	else {

		//check for all first, because in some mods, @A is a team
		if(equali(arg1,"@ALL") || equali(arg1,STAR)){
			copy(name,namelen,"everyone");
			poke_get_players(player_return, pnum_return);
		}

		else if( equali(arg1, AT, 1) ){

			#if MOD==CS
			if(equali(arg1[1],"T",1)){
				copy(name,namelen,"all Terrorists");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 1);
			}
			#endif
			#if MOD==DOD
			else if(equali(arg1[1],"AL",2)){
				copy(name,namelen,"all Allieds");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 1);
			}
			#endif
			#if MOD==NS
			else if(equali(arg1[1],"M",1)){
				copy(name,namelen,"all Marines");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 1);
			}
			#endif

			#if MOD==CS
			else if(equali(arg1[1],"C",1)){
				copy(name,namelen,"all Counter-Terrorists");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 2);
			}
			#endif
			#if MOD==DOD
			else if(equali(arg1[1],"AX",2)){
				copy(name,namelen,"all Axis");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 2);
			}
			#endif
			#if MOD==NS
			else if(equali(arg1[1],"A",1)){
				copy(name,namelen,"all Aliens");
				poke_get_players(players, pnum_return, PLAYER_INTEAM, 2);
			}
			#endif

			else if(equali(arg1[1],"BOT",3)){
				copy(name,namelen,"all Bots");
				poke_get_players(player_return, pnum_return, PLAYER_BOT);
			}

		}

		if( pnum_return < 1 ){
			if(console)
				PokeAdminNotice(id,LANG,id,"POKEMOD_NOUSERTEAM");
			else
				poke_printlang(id,"POKEMOD_NOUSERTEAM");
			return false;
		}
	}
	return true;
 }
 bool:GetPlayersPokes(id, target_id=0, pname[NAME_LENGTH], poke[MAX_POKES], pokenum[1], endname[NAME_LENGTH])
 {
	if( equal(pname, AT, 1) || equal(pname, STAR, 1) ){
		if(equali(pname,"@ALL") || equali(pname,STAR)){
			copy(endname,NAME_LENGTH-1,"pokemon")
			pokenum[0] = MAX_POKES
			for(new i=0; i<pokenum[0]; i++)
				poke[i] = PlayersPoke[target_id][i]
		}
		else if(isdigit(pname[1]) && ValidSlot(str_to_num(pname[1])) ){
			new apple = str_to_num(pname[1])
			pokenum[0] = 1
			poke[0] = PlayersPoke[target_id][apple-1]

			if( apple > 99 ) apple %= 100;
			if( apple > 13 ) apple %= 10;
			switch(apple)
			{
				case 1: formatex(endname, NAME_LENGTH-1, "%dst pokemon", apple);
				case 2: formatex(endname, NAME_LENGTH-1, "%dnd pokemon", apple);
				case 3: formatex(endname, NAME_LENGTH-1, "%drd pokemon", apple);
				default: formatex(endname, NAME_LENGTH-1, "%dth pokemon", apple);
			}
		}
		else if(pname[1]=='C'){
			pokenum[0] = 1
			poke[0] = PlayersPoke[target_id][PlayersActive[target_id]]
			formatex(endname,NAME_LENGTH-1,LANG,id,"POKEMOD_CURPOKE")
		}
		else{
			PokeAdminNotice(id,LANG,id,"POKEMOD_NOPOKESELECT")
			return false
		}
	}
	else{
		new apple = NameToPoke(pname);
		if( apple ){
			PokeToName(apple, endname, NAME_LENGTH);
			pokenum[0] = 1;
			poke[0] = apple;
		}
	}

	if( !pokenum[0] )
	{
		PokeAdminNotice(id,LANG,id,"POKEMOD_NOPOKE")
		return false
	}

	return true
 }
 
 ////////////////////
 //  Sets pokemon  //
 ////////////////////
 public admin_setpoke(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 3) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)

	if(isdigit(pname[0]))
		return PokeAdminNotice(id,"Usage: pm_setpoke <nick | @TEAM | @ALL> <pokemon name>")

	new name[32], poke = NameToPoke(pname)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	if(!ValidPoke(poke))
		return PokeAdminNotice(id,LANG,id,"POKEMOD_NOPOKE")

	#if RCON_POKEADMINS == 1
	if( PokeInfo[poke] & PI_SPECIAL && !(get_user_flags(id)&ADMIN_RCON) )
		return PokeAdminNotice(id,"You cannot set special pokemon.")
	#endif

	PokeToName(poke, pname, NAME_LENGTH)	//make sure it is capitalized correctly

	for(new i=0; i<pnum; i++){
		tid = players[i];
		Pokemon[tid] = poke;
		ResetSkills(tid, false);
	}

	if( pnum == 1 )
		PokeAdminNotice(id,"%s is now a %s.",name,pname)
	else
		PokeAdminNotice(id,"%s are now %ss.",name,pname)

	new command[255]
	formatex(command, 254, "changed %s into %s.",name, pname)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 ////////////////////
 //  Give pokemon  //
 ////////////////////
 public admin_givepoke(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], amount[25]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)
	read_argv(3,amount,24)

	if( isdigit(pname[0]) || ( amount[0] && !isdigit(amount[0]) ) )
		return PokeAdminNotice(id,"Usage: pm_givepoke <nick | @TEAM | @ALL> <pokemon name> <level>")

	new name[32], poke, level = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	poke = NameToPoke(pname)
	if(!ValidPoke(poke))
		return PokeAdminNotice(id,LANG,id,"POKEMOD_NOPOKE")

	#if RCON_POKEADMINS == 1
	if( PokeInfo[poke] & PI_SPECIAL && !(get_user_flags(id)&ADMIN_RCON) )
		return PokeAdminNotice(id,"You cannot give special pokemon.")
	#endif

	if(equali(amount,EMPTY))
		level = 1
	else if( TotalLevels < level || level < 1 )
		return PokeAdminNotice(id,"Invalid level.")

	new xp = PokeLevels[level-1]+1
	PokeToName(poke, pname, NAME_LENGTH)	//make sure it is capitalized correctly

	new bool:givegood = true;
	for(new i=0; i<pnum; i++){
		tid = players[i]

		if( pnum == 1 )
			switch( AvailableSlot(tid, poke, CHECK_ALL) )
			{
				case AS_NOGOOD:		return PokeAdminNotice(id,"You cannot give pokemon to wild players.");
				case AS_NOROOM:		return PokeAdminNotice(id,"%s has too many pokemon.",name);
				case AS_DUPLICATE:	return PokeAdminNotice(id,"%s already has a %s.",name,pname);
				case AS_OK:			givegood = AddPoke(tid, poke, xp, id, CHECK_ALL);
			}
		else if( AvailableSlot(tid, poke, CHECK_ALL) == AS_OK )
			AddPoke(tid, poke, xp, id, CHECK_ALL);
	}

	if( givegood )
		PokeAdminNotice(id,"%s was given a level %d %s.",name,level,pname)
	else
		PokeAdminNotice(id,"Could not give %s a level %d %s.",name,level,pname)

	new command[255]
	if( givegood )
		formatex(command, 254, "gave %s a level %d %s.",name, level, pname)
	else
		formatex(command, 254, "could not give %s a level %d %s.",name, level, pname)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 ////////////////////
 //  Take pokemon  //
 ////////////////////
 public admin_takepoke(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 3) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], endname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)

	if(isdigit(pname[0]))
		return PokeAdminNotice(id,"Usage: pm_takepoke <nick | @TEAM | @ALL> <pokemon | @ALL | @#>")

	new name[32], poke[MAX_POKES], pokenum[1]
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	new bool:valid_poke = GetPlayersPokes(id, _, pname, poke, pokenum, endname)
	if(!valid_poke)
		return PLUGIN_HANDLED

	new i, j;

	for( i=0; i<pnum; i++){
		tid = players[i]
		GetPlayersPokes(id, tid, pname, poke, pokenum, endname)

		for( j=0; j<pokenum[0]; j++){
			if( pnum == 1 ){
				if( HasPokemon(tid, poke[j], CHECK_ALL)==-1 && pokenum[0]==1)
					return PokeAdminNotice(id,"%s does not have %s.",name,endname)
				else
					ReleasePoke(tid, poke[j], false)
			}
			else if(HasPokemon(tid, poke[j], CHECK_ALL))
				ReleasePoke(tid, poke[j], false)
		}
	}

	PokeAdminNotice(id,"%s released %s.",name,endname)

	new command[255]
	formatex(command, 254, "made %s release %s.",name, endname)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 /////////////////
 //  Show help  //
 /////////////////
 public admin_help(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess( id );

	new arg1[8], flags = get_user_flags(id)
	new start = read_argv(1, arg1, 7) ? str_to_num(arg1) : 1
	new lHelpAmount = 10
	
	if (id == 0 && read_argc() == 3)
		lHelpAmount = read_argv(2, arg1, 7) ? str_to_num(arg1) : 10

	if (--start < 0)
		start = 0

	new clcmdsnum = get_concmdsnum(flags, id)

	if (start >= clcmdsnum)
		start = clcmdsnum - 1

	new help_title[31]
	formatex(help_title, 30, LANG, id, "HELP_COMS")
	replace(help_title, 30, "AMX Mod X", "PokeMod")
	console_print(id, "\n----- %s -----", help_title)

	new info[128], cmd[32], eflags
	new end = start + lHelpAmount // HELPAMOUNT

	if (end > clcmdsnum)
		end = clcmdsnum

	new actualfound
	for (new i = 0; i < clcmdsnum; i++)
	{
		get_concmd(i, cmd, 31, eflags, info, 127, flags, id)
		#if PM_ADMIN_COMMANDS==1
		if( !equal( cmd,"pm_",3 ) )
			continue
		#endif
		actualfound++
		if(actualfound > start && actualfound < end)
			console_print(id, "%3d: %s %s", actualfound, cmd, info)
	}

	if (end > actualfound)
		end = actualfound
	
	console_print(id, "----- %L -----", id, "HELP_ENTRIES", start + 1, end, actualfound)

	if (end < actualfound)
		formatex(help_title, 30, LANG, id, "HELP_USE_MORE", end + 1)
	else
		formatex(help_title, 30, LANG, id, "HELP_USE_BEGIN")

	#if PM_ADMIN_COMMANDS==1
	replace(help_title, 30, "amx_help", "pm_help")
	#endif

	console_print(id, "----- %s -----", help_title)

	return PLUGIN_HANDLED
 }
 
 ///////////////////////
 //  Give xp pokemon  //
 ///////////////////////
 public admin_givexp(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], amount[25], poke_num, endname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)
	read_argv(3,amount,24)

	if(!(isdigit(amount[0]) || (equal(amount,DASH,1) && isdigit(amount[1]))))
		return PokeAdminNotice(id,"Usage: pm_givexp <nick | @TEAM | @ALL> <pokemon | @ALL | @#> <amount>")

	new name[32], poke[MAX_POKES], pokenum[1], xp = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	new bool:valid_poke = GetPlayersPokes(id, _, pname, poke, pokenum, endname)
	if(!valid_poke)
		return PLUGIN_HANDLED

	if(!xp)
		return PokeAdminNotice(id,"Invalid experience amount.")

	new i, j;
	for( i=0; i<pnum; i++){
		tid = players[i]
		GetPlayersPokes(id, tid, pname, poke, pokenum, endname)

		for( j=0; j<pokenum[0]; j++){
			poke_num = HasPokemon(tid, poke[j])
			if(poke_num>=0){
				if( pnum == 1 && poke_num < 0 && pokenum[0] == 1 )
					return PokeAdminNotice(id,"%s does not have %s.",name,endname)

				GiveXP(tid, poke_num, xp)
				Check_Evolve(tid, poke[j])
			}
		}
	}

	PokeAdminNotice(id,"%s's %s was given %dexp.",name,endname,xp)

	new command[255]
	formatex(command, 254, "gave %s's %s %dexp.",name, endname, xp)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 ///////////////////////
 //  Sets pokemon xp  //
 ///////////////////////
 public admin_setxp(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], amount[25], poke_num, endname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)
	read_argv(3,amount,24)

	if(!(isdigit(amount[0]) || (equal(amount[0],"-",1) && isdigit(amount[1]))))
		return PokeAdminNotice(id,"Usage: pm_setxp <nick | @TEAM | @ALL> <pokemon | @ALL | @#> <amount>")

	new name[32], poke[MAX_POKES], pokenum[1], xp = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	new bool:valid_poke = GetPlayersPokes(id, _, pname, poke, pokenum, endname)
	if(!valid_poke)
		return PLUGIN_HANDLED

	if(xp<1)
		return PokeAdminNotice(id,"Invalid experience amount.")

	new i, j;
	for( i=0; i<pnum; i++){
		tid = players[i]
		GetPlayersPokes(id, tid, pname, poke, pokenum, endname)

		for( j=0; j<pokenum[0]; j++){
			poke_num = HasPokemon(tid, poke[j])
			if( poke_num >= 0 ){
				PlayersXP[tid][poke_num] = xp
				PlayersLevel(tid, poke_num, true);
				Check_Evolve(tid, poke[j])
			}
			else if( pnum == 1 && pokenum[0] == 1 )
				return PokeAdminNotice(id,"%s does not have %s.",name,endname)
		}
	}

	PokeAdminNotice(id,"%s's %s was set to %dexp.",name,endname,xp)

	new command[255]
	formatex(command, 254, "set %s's %s's exp to %d.",name, endname, xp)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 ///////////////////////
 //  Sets pokemon lv  //
 ///////////////////////
 public admin_setlvl(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], amount[25], poke_num, endname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)
	read_argv(3,amount,24)

	if(!(isdigit(amount[0]) || (equal(amount,DASH,1) && isdigit(amount[1]))))
		return PokeAdminNotice(id,"Usage: pm_setlvl <nick | @TEAM | @ALL> <pokemon | @ALL | @#> <level>")

	new name[32], poke[MAX_POKES], pokenum[1], lvl = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	new bool:valid_poke = GetPlayersPokes(id, _, pname, poke, pokenum, endname)
	if(!valid_poke)
		return PLUGIN_HANDLED

	if( TotalLevels < lvl || lvl < 1 )
		return PokeAdminNotice(id,"Invalid level.")

	new i, j;
	for( i=0; i<pnum; i++){
		tid = players[i]
		GetPlayersPokes(id, tid, pname, poke, pokenum, endname)

		for( j=0; j<pokenum[0]; j++){
			poke_num = HasPokemon(tid, poke[j], CHECK_ALL)
			if( poke_num >= 0 ){
				PlayersXP[tid][poke_num] = PokeLevels[lvl-1]+1
				PlayersLevels[tid][poke_num] = lvl;
				Check_Evolve(tid, poke[j])
			}
			else if( pnum == 1 && pokenum[0] == 1 )
				return PokeAdminNotice(id,"%s does not have %s.",name,endname)
		}
	}

	PokeAdminNotice(id,"%s's %s was set to level %d.",name,endname,lvl)

	new command[255]
	formatex(command, 254, "set %s's %s's level to %d.",name, endname, lvl)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 //////////////////////////////////
 //  Resets PokeMod to defaults  //
 //////////////////////////////////
 public admin_reset(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	new arg1[32], command[255], con_print[255]
	read_argv(1,arg1,31)

	formatex(command, 254, "reset %s.",arg1)
	formatex(con_print, 254, "%s reset.",arg1)

	if(equali(arg1,"config"))
		ResetConfig()
	else if(equali(arg1,"pokemon")){
		ResetPokemon()
		ReadPokemon(0)
		ReadPokemon(1)
	}
	else if(equali(arg1,"skills")){
		ResetSkillInfo()
		ReadSkills()
		ReadPokemon(0)
		ReadPokemon(1)
	}
	else if(equali(arg1,"items")){
		ResetPokemart()
		ReadItems(1)
	}
	else if(equali(arg1,"safari")){
		ResetSafari()
	}
	else if(equali(arg1,"maps"))
		ResetMapsConfig()
	else if(equali(arg1,"xp")){
		ResetXPs()
		ReadXPs()
	}
	else if(equali(arg1,"all")){
		ResetConfig()
		ResetPokemart()
		ResetSafari()
		ResetMapsConfig()
		ResetSkillInfo()
		ResetPokemon()
		ResetXPs()

		StartPokeMod(0);

		copy(command, 254, "reset all PokeMod files.")
		copy(con_print, 254, "All files reset.")
	}
	else{
		PokeAdminNotice(id, "Please specify what to reset.")
		return PokeAdminNotice(id, "Available Resets: config ; pokemon ; skills ; items ; safari ; maps ; xp ; all")
	}

	PokeAdminNotice(id,con_print)

	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 /////////////////////////
 //  Loads saved cvars  //
 /////////////////////////
 public admin_loadconfig(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	StartPokeMod(0);
	PokeAdminNotice(id,"Config Files Loaded.");

	AdminPrint( id, "loaded the PokeMod config files.");

	return PLUGIN_HANDLED;
 }
 
 ///////////////////////
 //  Erases saved XP  //
 ///////////////////////
 public admin_erasexp(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	PokeAdminNotice(id,"Please wait while everything is erased.")

	cleanXP(true)
	new players[MAX_PLAYERS], pnum, tid, i, k;
	get_players(players, pnum);
	for( i = 0; i < pnum; i++ ){
		tid = players[i];
		Pokemon[tid] = 0;
		for( k = 0; k<MAX_POKES; k++){
			PlayersPoke[tid][k] = 0;
			PlayersXP[tid][k] = 0;
			PlayersLevels[tid][k] = 1;
		}
		client_connect(tid);
	}

	PokeAdminNotice(id,"Everything erased successfully.")

	new command[255]
	copy(command, 254, "erased all PokeMod experience and levels.")
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 /////////////////////////////
 //  Prints admin commands  //
 /////////////////////////////
 public AdminPrint( id, text[])
 {
	new name[32];
	get_user_name(id, name, 31);
	show_activity( id, name, text );

	new authid[35];
	get_user_authid(id, authid, 34);
	if(!IsValidSteamID(authid)) {
		get_user_ip(id,authid,34,1);
	}
	log_amx("%s: \"%s<%d><%s><>\" %s", PREFIX, name, get_user_userid(id), authid, text);
 }

 //////////////////////////////////////////////
 //  If user has access to PokeMod commands  //
 //////////////////////////////////////////////
 poke_access(id, level)
 {
	if( !id )
		return true

	new user_flags = get_user_flags(id);
	if( !level || (user_flags & level) ){
		new accessflags[11]
		get_ppcvar_string(pm_admin, accessflags, 10)
		return ( get_user_flags(id) & read_flags(accessflags) )
	}
	return false;
 }
 
 //////////////////////////////////////////////////
 //  displays/changes information about pokemod  //
 //////////////////////////////////////////////////
 public Console_Pokemod(id,level)
 {
	if( PM_disabled == PM_ON && !PMon() && !poke_access(id,level) && 0<id<=MAX_PLAYERS)
		return PokeAdminNoticeOff(id)

	new arg1[32], arg2[32];
	read_argv(1,arg1,31);
	read_argv(2,arg2,31);

	if(	equali(arg1,EMPTY)
	||	(0<id<=MAX_PLAYERS && (PM_disabled != PM_ON || !poke_access(id,level)) )
	){
		//display info about pokemod
		switch(PM_disabled)
		{
			case PM_ON:{
				if( PMon() ){
					if(Safari())
						console_print(id, "[PokeMod is set to SAFARI");
					else{
						console_print(id, "[PokeMod is set to ON");
						if(SaveXPon()){
							#if SAVE == SAVE_VAULT
							console_print(id, "[Saving is set to ON (Vault)");
							#endif
							#if SAVE == SAVE_NVAULT
							console_print(id, "[Saving is set to ON (NVault)");
							#endif
							#if SAVE == SAVE_MYSQL
							console_print(id, "[Saving is set to ON (Sql)");
							#endif
						}
						else
							console_print(id, "[Saving is set to OFF");
					}
				}
				else
					console_print(id, "[PokeMod is set to OFF");
			}
			case PM_OFF:{
				new missing = poke_missing_files();
				if( missing > 0 ){
					if( missing == 1 )
						console_print(id, "[PokeMod is Missing a File" );
					else
						console_print(id, "[PokeMod is Missing %d Files", missing );
				}
				console_print(id, "[PokeMod is set to OFF (no precache)");
			}
			case PM_XP:{
				console_print(id, "[PokeMod has its Skills Disabled (no precache)");
			}
		}

		console_print(id, "[Version : %s",VERSION);
		console_print(id, "[Language File : %L",id,"POKEMOD_LANGVERSION");
		console_print(id, "[PokeMod Team : %s",POKETEAM);
		if(get_ppcvar_num(pm_debug)){
			new debug_key[51];
			get_ppcvar_string(pm_debug_key, debug_key, 50);
			console_print(id, "[Debug is '%d' : '%s'",get_ppcvar_num(pm_debug),debug_key);
		}
		return PLUGIN_HANDLED;
	}

	new PokeFile[128];
	Poke_FileName( F_ModConfig, PokeFile);

	if(equali(arg1,"on")){
		if(PMon())
			return PokeAdminNotice(id, "is already ON");

		PokeAdminNotice(id, "is now ON");
		set_ppcvar_num(pm_pokemod, 1);
		set_ppcvar_line(pm_pokemod, "1");
	}
	else if(equali(arg1,"off")){
		if(!PMon())
			return PokeAdminNotice(id, "is already OFF");

		PokeAdminNotice(id, "is now OFF");
		set_ppcvar_num(pm_pokemod, 0);
		set_ppcvar_line(pm_pokemod, "0");
	}
	else if(equali(arg1,"safari")){
		if(equali(arg2,"on")){
			if(!Safari()){
				save_all();
				PokeAdminNotice(id, "Safari is now ON");
				set_ppcvar_num(pm_safari_mode, 1);
				set_ppcvar_line(pm_safari_mode, "1");
			}
			else
				PokeAdminNotice(id, "Safari mode is already ON.");
		}
		else if(equali(arg2,"off")){
			if(Safari()){
				PokeAdminNotice(id, "Safari is now OFF");
				set_ppcvar_num(pm_safari_mode, 0);
				set_ppcvar_line(pm_safari_mode, "0");
				load_all();
			}
			else
				PokeAdminNotice(id, "Safari mode is already OFF.");
		}
	}
	else if(equali(arg1,"debug")){
		if(equal(arg2,EMPTY)){
			new debug_key[51];
			get_ppcvar_string(pm_debug_key, debug_key, 50);
			PokeAdminNotice(id, "Debug is '%d' : '%s'",get_ppcvar_num(pm_debug),debug_key);
		}
		else{
			new debug_value = str_to_num(arg2);
			if(!debug_value && !equali(arg2,"0") && !equali(arg2,"off")){
				set_ppcvar_string(pm_debug_key, arg2);
				set_ppcvar_line(pm_debug_key, arg2);
				PokeAdminNotice(id, "Debug Key is now '%s'",arg2);
			}
			else{
				set_ppcvar_num(pm_debug, debug_value);
				if(debug_value>0){
					set_ppcvar_line(pm_debug, arg2);
					PokeAdminNotice(id, "Debug is now Level:%d",debug_value);
				}
				else{
					set_ppcvar_string(pm_debug_key, EMPTY);
					set_ppcvar_line(pm_debug_key, EMPTY);
					set_ppcvar_line(pm_debug, "0");
					PokeAdminNotice(id, "Debug is now OFF");
				}
			}
		}
	}

	return PLUGIN_HANDLED;
 }
 
 /////////////////////////////////////////////////
 //  adds/changes pokemart values (of pokemon)  //
 /////////////////////////////////////////////////
 public admin_pokemart(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 3)	//thank you SHmod
		return PokeAdminArgs(id);

	new arg1[32], arg2[32];
	read_argv(1,arg1,31);
	read_argv(2,arg2,31);

	new PokeFile[128];
	Poke_FileName( F_Items, PokeFile);
	new item, price = str_to_num(arg2), iname[NAME_LENGTH], is_item=0, line=-1 ;
	new size = file_size(PokeFile, 1);

	if(!item){
		item = NameToItem(arg1);
		if(item==MAX_ITEMS){
			item = NameToPoke(arg1);
			if(item){
				is_item = 0;
				PokeToName(item, iname, NAME_LENGTH);
			}
			else
				return PokeAdminNotice(id,"You did not supply a valid pokemon or item.");
		}
		else{
			is_item = 1;
			ItemToName(item, iname, NAME_LENGTH);
		}
	}
	if(!price && !equal(arg2,"0"))
		return PokeAdminNotice(id,"%s is currently priced at: %d",iname,is_item?CostOfItem(item):CostOfPoke(item));

	new Left[NAME_LENGTH], Right[151], LeftN;

	new is_in_file;

	new pokedexline[255], i, charnum;

	for(i=0; i<=size; i++){
		read_file(PokeFile,i,pokedexline,255,charnum);
		strbreak(pokedexline, Left, NAME_LENGTH-1, Right, 150);

		if(is_item){
			LeftN = NameToItem(Left, false);
			if(LeftN == MAX_ITEMS)
				continue;
		}
		else{
			LeftN = NameToPoke(Left);
			if(!LeftN)
				continue;
		}

		if(LeftN==item){
			is_in_file = i;
			break;
		}
	}
	if(is_in_file)
		line = is_in_file;
	else
		line = -1;
	if( is_item )
		ItemPrice[item] = price;
	else
		ItemPrice[MAX_ITEMS+item] = price;


	new line_to_write[128];
	replace_all(iname, 31, " ", "_");
	strbreak(Right, Left, NAME_LENGTH-1, Right, 150);		//need to get the old price out
	formatex(line_to_write, 127, "%s %d %s",iname, price, Right);
	write_file(PokeFile, line_to_write, line);

	new command[255], bool:AdminOnly = false;
	if(price<0)
		AdminOnly = true;
	PokeAdminNotice(id,"You changed the price of %s to $%d%s.", iname, abs(price), AdminOnly?"(Admin Only)":EMPTY);

	if(price)
		formatex(command, 254, "changed Pokemart price of: %s to $%d%s.",iname, abs(price), AdminOnly?"(Admin Only)":EMPTY);
	else
		formatex(command, 254, "disabled Pokemart selling of %s.",iname);
	AdminPrint( id, command );

	return PLUGIN_HANDLED;
 }
 
 ///////////////////////////////////////////////
 //  adds/changes safari values (of pokemon)  //
 ///////////////////////////////////////////////
 public admin_safari(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 3)	//thank you SHmod
		return PokeAdminArgs(id)

	new arg1[32], arg2[32]
	read_argv(1,arg1,31)
	read_argv(2,arg2,31)

	new poke, level = str_to_num(arg2), pname[NAME_LENGTH],line=-1

	poke = NameToPoke(arg1)
	if( !ValidPoke(poke) )
		return PokeAdminNotice(id,"You did not supply a valid pokemon.")

	if(!level && !equali(arg2,"0"))
		return PokeAdminNotice(id,"%s's current safari level is: %d",pname,SafariLevel(poke))

	PokeToName(poke, pname, NAME_LENGTH)	//correct capitalization

	new PokeFile[128];
	Poke_FileName( F_Safari, PokeFile);
	new is_in_file
	new size = file_size(PokeFile, 1)

	new pokedexline[255], i, charnum
	new Left[NAME_LENGTH], Right[151], LeftN

	for(i=0; i<=size; i++){
		read_file(PokeFile,i,pokedexline,255,charnum)
		strbreak(pokedexline, Left, NAME_LENGTH-1, Right, 150)
		LeftN = str_to_num(Left)
		if(!LeftN){
			LeftN = NameToPoke(Left)
			if(!LeftN)
				continue
		}
		if(LeftN==poke){
			is_in_file = i
			break
		}
	}
	if(is_in_file)
		line = is_in_file
	else
		line = -1

	new line_to_write[128]
	formatex(line_to_write, 127, "%s %d",pname, level)

	new oldlevel = SafariLevels[poke]
	SafariLevels[poke] = level

	write_file(PokeFile, line_to_write, line)

	new command[255], bool:AdminOnly = false
	if(level<0)
		AdminOnly = true

	if(level!=-1000)
		PokeAdminNotice(id,"You changed the safari level of %s to %d%s.", pname, abs(level), AdminOnly?"(Admin Only)":EMPTY)
	else{
		for(new q=0; q<MAX_PLAYERS; q++){
			if(Pokemon[q]==poke)
				RefreshPoke(q)
		}
		PokeAdminNotice(id,"You completely disabled %s.", pname)
	}

	if(level && level!=-1000)
		formatex(command, 254, "changed Safari level of %s to %d%s.",pname, abs(level), AdminOnly?"(Admin Only)":EMPTY)
	else{
		if(level==-1000)
			formatex(command, 254, "disabled the use of %s.",pname)
		else{
			if(oldlevel==-1000)
				formatex(command, 254, "enabled the use of %s.",pname)
			else
				formatex(command, 254, "disabled the use of %s in Safari mode.",pname)
		}
	}
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 
 
 ////////////////////
 //  Give pokemon  //
 ////////////////////
 public admin_giveitem(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], iname[NAME_LENGTH], amount[25]
	read_argv(1,arg1,31)
	read_argv(2,iname,NAME_LENGTH-1)
	read_argv(3,amount,24)

	if(	isdigit(iname[0])
	|| (amount[0] && !( isdigit(amount[0] ) || ( equal(amount,DASH,1) && isdigit(amount[1]) ) ) )
	)
		return PokeAdminNotice(id,"Usage: pm_givepoke <nick | @TEAM | @ALL> <item name> <amount>")

	new name[32], item, num = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	item = NameToItem(iname, false)
	if( item==MAX_ITEMS )
		return PokeAdminNotice(id,"Invalid item name.")

	if(equali(amount,EMPTY))
		num = 1
	else if(!num)
		return PokeAdminNotice(id,"Invalid amount.")

	ItemToName(item, iname, NAME_LENGTH)

	for(new i=0; i<pnum; i++){
		tid = players[i]

		PlayersItem[tid][item] += num
		if(PlayersItem[tid][item]<0)
			PlayersItem[tid][item] = 0

		if( PlayersItem[tid][item] && ItemEventForward > -1 )
		{
			new functionReturn
			ExecuteForward(ItemEventForward, functionReturn, tid, IE_PICKUP, item)

			// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
			// and 0 is used by continue and invalid return.
			if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
			{
				return PLUGIN_HANDLED;
			}
		}
	}

	PokeAdminNotice(id,"%s was %s %d %s.",name,num>0?"given":"robbed of",abs(num),iname)

	new command[255]
	if(num>0)
		formatex(command, 254, "gave %s %d %s.",name, num, iname)
	else
		formatex(command, 254, "stole %d %s from %s.", abs(num), iname, name)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 
 ///////////////////////
 //  Morph a pokemon  //
 ///////////////////////
 public admin_morph(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new arg1[32], pname[NAME_LENGTH], newpname[NAME_LENGTH], poke_num, endname[NAME_LENGTH]
	read_argv(1,arg1,31)
	read_argv(2,pname,NAME_LENGTH-1)
	read_argv(3,newpname,NAME_LENGTH-1)

	if(isdigit(newpname[0]))
		return PokeAdminNotice(id,"Usage: pm_morph <nick | @TEAM | @ALL> <pokemon | @ALL | @#> <new pokemon>")

	new name[32], poke[MAX_POKES], pokenum[1]
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	new bool:valid_poke = GetPlayersPokes(id, _, pname, poke, pokenum, endname)
	if(!valid_poke)
		return PLUGIN_HANDLED

	new newpoke = NameToPoke(newpname)
	if(!newpoke)
		return PokeAdminNotice(id,"Invalid new pokemon.")

	PokeToName(newpoke, newpname, NAME_LENGTH)

	for(new i=0; i<pnum; i++){
		tid = players[i]
		GetPlayersPokes(id, tid, pname, poke, pokenum, endname)

		for(new j=0; j<pokenum[0]; j++){
			poke_num = HasPokemon(tid, poke[j])
			if(poke_num>=0){
				PlayersPoke[tid][poke_num] = newpoke
				if( poke_num == PlayersActive[id] )
					Pokemon[id] = newpoke;
			}
			else if( pnum == 1 && pokenum[0] == 1 )
				return PokeAdminNotice(id,"%s does not have %s.",name,endname)
		}
	}

	PokeAdminNotice(id,"%s's %s was morphed into %s.",name,endname,newpname)

	new command[255]
	formatex(command, 254, "morphed %s's %s into %s.",name, endname, newpname)
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 ////////////////////////
 //  Enable a pokemon  //
 ////////////////////////
 public admin_enable(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	new cmd[20], pname[NAME_LENGTH], poke, bool:temp = false
	read_argv(0,cmd,19)
	read_argv(1,pname,NAME_LENGTH-1)

	if(containi(cmd,"temp")!=-1)
		temp = true

	new totalpokes[TOTAL_POKES], pnum, i
	new command[255], arg_count = read_argc();
	formatex(command, 254, "%senabled the use of ",temp?"temporarily ":EMPTY)

	poke = NameToPoke(pname)
	if( !ValidPoke(poke) ){
		if(equali(pname,"@ALL")){
			add(command, 254, "all pokemon.");
			for(i=0;i<=PokesCreated;i++) totalpokes[i] = i+1;
			pnum = PokesCreated+1;
		}
		else
			return PokeAdminNotice(id,"Invalid pokemon to enable.")
	}
	else{
		for( i=1; i < arg_count; i++ ){
			read_argv(i,pname,NAME_LENGTH-1)
			poke = NameToPoke(pname)
			PokeToName(poke, pname, NAME_LENGTH)
			if( i == 1 ){
				add(command, 254, pname);
			}
			else if( i == arg_count-1 ){
				if( arg_count > 3 )
					add(command, 254, ",");
				add(command, 254, " and ");
				add(command, 254, pname);
			}
			else {
				add(command, 254, ", ");
				add(command, 254, pname);
			}
			totalpokes[ pnum++ ] = poke
		}
		add(command, 254, ".");
	}


	new PokeFile[128];
	Poke_FileName( F_Safari, PokeFile);
	new is_in_file, line
	new size
	new pokedexline[255], k, charnum
	new Left[NAME_LENGTH], Right[151], LeftN

	for(i=0; i<pnum; i++)
	{
		poke = totalpokes[i]
		if(!temp){
			size = file_size(PokeFile, 1)

			for(k=0; k<=size; k++){
				read_file(PokeFile,k,pokedexline,255,charnum)
				strbreak(pokedexline, Left, NAME_LENGTH-1, Right, 150)
				LeftN = str_to_num(Left)
				if(!LeftN){
					LeftN = NameToPoke(Left)
					if(!LeftN)
						continue
				}
				if(LeftN==poke){
					is_in_file = k
					break
				}
			}
			if(is_in_file)
				line = is_in_file
			else
				line = -1

			formatex(pokedexline, 254, "%s 1",PokeToName(poke))
			write_file(PokeFile, pokedexline, line)
		}

		SafariLevels[poke] = 0
	}

	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 /////////////////////////
 //  Disable a pokemon  //
 /////////////////////////
 public admin_disable(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	new cmd[20], pname[NAME_LENGTH], poke, bool:temp = false
	read_argv(0,cmd,19)
	read_argv(1,pname,NAME_LENGTH-1)

	if(containi(cmd,"temp")!=-1)
		temp = true

	new totalpokes[TOTAL_POKES], pnum, i
	new command[255], arg_count = read_argc();
	formatex(command, 254, "%sdisabled the use of ",temp?"temporarily ":EMPTY)

	poke = NameToPoke(pname)
	if( !ValidPoke(poke) ){
		if(equali(pname,"@ALL")){
			add(command, 254, "all pokemon.")
			for(i=0;i<=PokesCreated;i++) totalpokes[i] = i+1
			pnum = PokesCreated+1
		}
		else
			return PokeAdminNotice(id,"Invalid pokemon to disable.")
	}
	else{
		for( i=1; i < arg_count; i++ ){
			read_argv(i,pname,NAME_LENGTH-1)
			poke = NameToPoke(pname)
			PokeToName(poke, pname, NAME_LENGTH)
			if( i == 1 ){
				add(command, 254, pname);
			}
			else if( i == arg_count-1 ){
				if( arg_count > 3 )
					add(command, 254, ",");
				add(command, 254, " and ");
				add(command, 254, pname);
			}
			else {
				add(command, 254, ", ");
				add(command, 254, pname);
			}
			totalpokes[ pnum++ ] = poke
		}
		add(command, 254, ".");
	}


	new PokeFile[128];
	Poke_FileName( F_Safari, PokeFile);
	new is_in_file, line
	new size
	new pokedexline[255], k, charnum
	new Left[NAME_LENGTH], Right[151], LeftN

	for(i=0; i<pnum; i++)
	{
		poke = totalpokes[i]
		if(!temp){
			size = file_size(PokeFile, 1)

			for(k=0; k<=size; k++){
				read_file(PokeFile,k,pokedexline,255,charnum)
				strbreak(pokedexline, Left, NAME_LENGTH-1, Right, 150)
				LeftN = str_to_num(Left)
				if(!LeftN){
					LeftN = NameToPoke(Left)
					if(!LeftN)
						continue
				}
				if(LeftN==poke){
					is_in_file = k
					break
				}
			}
			if(is_in_file)
				line = is_in_file
			else
				line = -1

			formatex(pokedexline, 254, "%s -1000",PokeToName(poke))
			write_file(PokeFile, pokedexline, line)
		}

		SafariLevels[poke] = -1000
	}

	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 
 
 ///////////////////
 //  Change rank  //
 ///////////////////
 #if POKERANK_SYSTEM==1
 public admin_rank(id,level)
 {
	if(!poke_access(id,level))
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 4) {	//thank you SHmod
		PokeAdminArgs(id)
		return PokeAdminName(id)
	}

	new cmd[20], arg1[32], amount[25], bool:set = false
	read_argv(0,cmd,19)
	read_argv(1,arg1,31)
	read_argv(2,amount,24)

	if(containi(cmd,"set")!=-1)
		set = true

	if(!(isdigit(amount[0]) || (equal(amount,DASH,1) && isdigit(amount[1])))){
		if(set)
			return PokeAdminNotice(id,"Usage: pm_setrank <nick | @TEAM | @ALL> <amount>")
		else
			return PokeAdminNotice(id,"Usage: pm_addrank <nick | @TEAM | @ALL> <amount>")
	}

	new name[32], num = str_to_num(amount)
	new players[MAX_PLAYERS], pnum, tid
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	if(!num)
		return PokeAdminNotice(id,"Invalid amount.")

	for(new i=0; i<pnum; i++){
		tid = players[i]

		if(set)
			PlayersCatch[tid] = num
		else
			PlayersCatch[tid] += num

		if(SpecialRank(tid)!=-1)	//update their thing, and check if they went higher
			UpdateSpecialRank(tid)
		else
			CheckUpdateRank(tid) //only check if they went higher than the last person in Elite Four
	}

	if(set)
		PokeAdminNotice(id,"%s PokeRank points was set to %d.",name,num)
	else
		PokeAdminNotice(id,"%s was %s %d PokeRank points.",name,num>0?"given":"robbed of",num)

	new command[255]
	if(set)
		formatex(command, 254, "set %s PokeRank points to %d.",name, num)
	else {
		if(num>0)
			formatex(command, 254, "gave %s %d PokeRank points.",name, num)
		else
			formatex(command, 254, "stole %d PokeRank points from %s.", abs(num), name)
	}
	AdminPrint( id, command)

	return PLUGIN_HANDLED
 }
 #endif

 ////////////////////////////
 //  Change pokemod cvars  //
 ////////////////////////////
 public admin_cvar(id)
 {
	if(!(get_user_flags(id)&ADMIN_CVAR ) && id )
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 3)	//thank you SHmod
		return PokeAdminNotice(id,"Too many arguments supplied.");

	new arg1[32], cvar_num;
	read_argv(1,arg1,31);
	cvar_num = get_pokecvar(arg1, true);

	if(cvar_num==MAX_CVARS)
		return PokeAdminNotice(id,"Invalid PokeMod Cvar.");

	copy(arg1, 31, get_pokecvar_name(cvar_num) );	//refresh the name

	if( id && !(get_user_flags(id)&ADMIN_RCON) ){
		new bool:deny = false;
		if(	cvar_num == pm_admin )
			deny = true;
	#if SAVE==SAVE_MYSQL
		if(	cvar_num == pm_mysql_host
		||	cvar_num == pm_mysql_user
		||	cvar_num == pm_mysql_pass
		||	cvar_num == pm_mysql_db
		||	cvar_num == pm_mysql_persistent
		)
			deny = true;
	#endif
		if( deny )
			return PokeAdminNotice(id,"You have no access to change that cvar.");
	}

	new arg2[64];
	read_argv(2,arg2,63);

	if( !ValidData(arg2) ){
		get_ppcvar_string(cvar_num,arg2,63);
		return PokeAdminNotice(id,"'%s' is set to '%s'.",arg1,arg2)
	}

	set_ppcvar_string(cvar_num, arg2);
	set_ppcvar_line(cvar_num, arg2);

	PokeAdminNotice(id,"'%s' was set to '%s'.",arg1,arg2);

	new command[255];
	formatex(command, 254, "set '%s' to '%s'.",arg1, arg2);
	AdminPrint( id, command );

	return PLUGIN_HANDLED;
 }

 ///////////////////////////////////////////
 //  Changes who sees the debug messages  //
 ///////////////////////////////////////////
 public admin_debugger(id)
 {
	if(!(get_user_flags(id)&ADMIN_RCON ) && id )
		return PokeAdminNoticeAccess(id)

	if(!PMon())
		return PokeAdminNoticeOff(id)

	if (read_argc() > 2)	//thank you SHmod
		return PokeAdminArgs(id)

	new arg1[32];
	read_argv(1,arg1,31);

	if( equali(arg1, "0") ){
		debugger = 0;
		PokeAdminNotice(id,"The server is now the debugger.");
		AdminPrint( id, "changed the debugger to the server." );
		return PLUGIN_HANDLED
	}

	new name[32]
	new players[MAX_PLAYERS], pnum
	if( !get_info(id,arg1,name,31,true,players,pnum) )
		return PLUGIN_HANDLED

	if( pnum > 1 )
		return PokeAdminNotice(id,"You can only set one player to be the debugger.");

	if( !Pokemod_Connected( players[0] ) )
		return PokeAdminNotice(id,"You must set a player to be the debugger.");

	debugger = players[0];
	PokeAdminNotice(id,"%s is now the debugger.",name);

	new command[255];
	formatex(command, 254, "changed the debugger to %s.",name);
	AdminPrint( id, command );

	return PLUGIN_HANDLED;
 }
 PokeAdminArgs(id)
	return PokeAdminNotice(id,LANG,id,"POKEMOD_ADMINARGS")
 PokeAdminName(id)
	return PokeAdminNotice(id,LANG,id,"POKEMOD_ADMINNAME")
 PokeAdminNotice(id, const error_lulz[], {Float,Sql,Result,_}:...)
 {
	new output[256];
	vformat(output, 255, error_lulz, 3);
	console_print(id, "%s %s", PREFIX, output );
	return PLUGIN_HANDLED;
 }
 PokeAdminNoticeOff(id)
	return PokeAdminNotice( id, LANG, id,"POKEMOD_NOACTIVE");
 PokeAdminNoticeAccess(id)
	return PokeAdminNotice( id, LANG, id,"POKEMOD_NOACCESS");

 
 