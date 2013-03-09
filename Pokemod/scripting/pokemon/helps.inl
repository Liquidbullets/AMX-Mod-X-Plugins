
 ///////////////////////////////////////////
 //  Tells players to update their binds  //
 ///////////////////////////////////////////
 public updatecommands(id)
	poke_print(id, print_chat, "Please rebind your keys to +pokeskill and +pokeitem.")

 //////////////////////////////////////////////////
 //  When someone wants to look at the pokehelp  //
 //////////////////////////////////////////////////
 public pokehelp(id)
 {
	new poke_help_message[2555], iLen

	new bool:its_a_motd = true
	#if MOD==NS
		its_a_motd = false
	#endif

	iLen += formatex(poke_help_message[iLen], (2555-iLen),	"%sPokeMod%s",its_a_motd?"<center><b><font color=red size=+2>":EMPTY,its_a_motd?"</font></b></center>":EMPTY)
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += formatex(poke_help_message[iLen], (2555-iLen),	"%sThe World of Pokemon with a Twist!%s",its_a_motd?"<center>":EMPTY,its_a_motd?"</center>":EMPTY)
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>To see a list of commands, say /pokecommands")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>Touch Prof. Oak for the poke menu(main menu)")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>There are 2 ways to \"catch\" pokemon.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- 1. Buy a pokeball from Prof. Oak and kill someone.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- 2. Go to the opposing teams base and pick up a pokeball.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>-- To see a list of pokemon, say \"/pokedex #\" or \"/pokedex name\"")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>To change pokemon, say /go <pokemon name>, or to see a menu say /go")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>To use pokemon skills and items, bind keys to +pokeskill and +pokeitem")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- To bind a key, follow these steps:")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- 1. Press the ` or ~ key to bring up the console")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- 2. Type bind (key) (what to bind)")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>-- For this example you will want to bind +pokeskill or +pokeitem")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- 3. Press enter and voila, now just press (key) and it will do what you binded")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>- Examples: bind v +pokeskill, bind f +pokeitem")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>When you kill someone, you get experience based on their level.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>As your pokemon levels up, its skill becomes more invoking.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>When you get enough experience, your pokemon may evolve into a new pokemon.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>Your evolved pokemon will have its predecessor's skill and a new one.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>* Note that not all pokemon evolve with just experience.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>Your max hp is based on the average level of your pokemon.")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<li>So go train them all!")
	iLen += copy(poke_help_message[iLen], (2555-iLen),		"<br>")
	iLen += formatex(poke_help_message[iLen], (2555-iLen),	"<br>Check the %sPokeMod Site%s for updates, suggestions, and other things.",its_a_motd?"<a href=\"http://www.deadrocks.com/forums/\">":EMPTY,its_a_motd?"</a>":EMPTY)

	if(!its_a_motd){
		replace_all(poke_help_message, 2555, "<li>", "\n-")
		replace_all(poke_help_message, 2555, "<br>", "\n")
		set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
		show_hudmessage(id,poke_help_message)
	}
	else
		show_motd( id, poke_help_message, "PokeHelp" )
	return PLUGIN_HANDLED
 }

 //////////////////////////////////////////////////
 //  When someone wants to look at the pokehelp  //
 //////////////////////////////////////////////////
 public pokecommands(id)
 {
	new poke_help_message[2555], iLen

	new bool:its_a_motd = true
	#if MOD==NS
		its_a_motd = false
	#endif

	if( its_a_motd )
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<center><b><font color=red size=+2>PokeMod Commands</font></b></center>")

	iLen += copy(poke_help_message[iLen], (2555-iLen), "<br>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<center>Available Commands:</center>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<br><table>")

	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>Say:<table>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokecommands</td><td>gets you here</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokehelp</td><td>shows basic information about PokeMod</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/save</td><td>saves your PokeMod data (unless saving is off or safari is on)</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokedex poke</td><td>shows information about the pokemon specified (ex. '/pokedex 1' or '/pokedex bulbasaur')</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokemenu</td><td>shows PokeMod menu if you're dead or close to a Prof. Oak</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokehud up/down</td><td>moves the PokeMod HUD up or down (ex. '/pokehud up' or '/pokehud down'</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/playerspokes</td><td>shows everyones pokemon</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokemart item amount</td><td>buys the amount of the item</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokeitem item</td><td>view information about the item</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/go</td><td>shows menu to select your active pokemon</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/mypokes</td><td>shows the pokemon in your party</td></tr>")
	if(!Safari()){
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/switch poke1 poke2</td><td>switches pokemon with another</td></tr>")
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/release</td><td>shows menu to release pokemon into the wild</td></tr>")
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/give player pokemon</td><td>gives your pokemon to player (ex. '/give Emp pikachu')</td></tr>")
#if ALLOW_WILD==1
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/wild</td><td>become a random wild pokemon</td></tr>")
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/trainer</td><td>become a trainer</td></tr>")
#endif
#if POKERANK_SYSTEM==1
		iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>/pokerank</td><td>view players pokeranks</td></tr>")
#endif
	}
	iLen += copy(poke_help_message[iLen], (2555-iLen), "</table></td></tr>")

	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>Console:<table>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>pokemod</td><td>shows PokeMod information about server</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>+pokeskill</td><td>uses your current skill</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "<tr><td>+pokeitem</td><td>shows menu of your current items</td></tr>")
	iLen += copy(poke_help_message[iLen], (2555-iLen), "</table></td></tr></table>")

	if( !its_a_motd ){
		replace_all(poke_help_message, 2555, "<tr><td>", "<br>");
		replace_all(poke_help_message, 2555, "</td><td>", " - ");
		replace_all(poke_help_message, 2555, "</td></tr>", EMPTY);
		replace_all(poke_help_message, 2555, "<table>", EMPTY);
		replace_all(poke_help_message, 2555, "</table>", EMPTY);
	}

	if(!its_a_motd){
		replace_all(poke_help_message, 2555, "<li>", "\n-")
		replace_all(poke_help_message, 2555, "<br>", "\n")
		set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
		show_hudmessage(id,poke_help_message)
	}
	else
		show_motd( id, poke_help_message, "PokeCommands" )
	return PLUGIN_HANDLED
 }

 ///////////////////////////////////////////////////////////////////////
 //  When someone wants see what other people have (through console)  //
 ///////////////////////////////////////////////////////////////////////
 public Console_PlayersPokemon(id)
 {
	PlayersPokemon(id, false)
	return PLUGIN_HANDLED
 }
 ///////////////////////////////////////////////////////////////////////
 //  When someone wants see what other people have (through console)  ////Items//
 ///////////////////////////////////////////////////////////////////////
 public Console_PlayersItems(id)
 {
	PlayersItems(id, false)
	return PLUGIN_HANDLED
 }
 
 /////////////////////////////////////////////////////
 //  When someone wants see what other people have  //
 /////////////////////////////////////////////////////
 public PlayersPokemon(id, bool:motd)
 {
	new text[1501], len = 1500, n, temp[256], iLen
	new players[MAX_PLAYERS], pnum, pid, name[32]
	poke_get_players(players, pnum)

	new bool:its_a_motd = true
	#if MOD==NS
		its_a_motd = false
	#endif

	if(motd)
		if(its_a_motd)
			n += copy(text[n],len-n,"<center><b>Player's Pokemon</b></center><br>")
		else
			n += copy(text[n],len-n,"Player's Pokemon<br>")
	else
		console_print(id, ":::Player's Pokemon:::")

	new x, j, teamName[71], show_pics = POKEDEX_IMAGES
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
					iLen += formatex(temp[iLen], (255-iLen), "<br><b>%s</b> -", name)

					if( show_pics )
						iLen += formatex(temp[iLen], (255-iLen), " %s", PokeImageHtml( Pokemon[pid] ))
				}
				else
					iLen += formatex(temp[iLen], (255-iLen), "<br>%s -", name)
			}
			else
				console_print(id, "::%s",name)
			if(Safari() || g_isWild[pid]>0){
				iLen += formatex(temp[iLen], (255-iLen), " %s(%d)", PokeToName(Pokemon[pid]), PlayersLevel(pid,j))
			}
			else{
				for( j=0; j<MAX_ACTIVE_POKES; j++){
					if(PlayersPoke[pid][j] || j==0){
						if(PlayersActive[pid]==j){
							if(motd && its_a_motd)
								iLen += copy(temp[iLen], (255-iLen), " <b>")
						}
						iLen += formatex(temp[iLen], (255-iLen), " %s(%d)", PokeToName(PlayersPoke[pid][j]), PlayersLevel(pid,j))
						if(PlayersActive[pid]==j){
							if(motd && its_a_motd)
								iLen += copy(temp[iLen], (255-iLen), "</b>")
						}
					}
				}
			}
			if(motd)
				n += copy(text[n],len-n,temp)
			else
				console_print(id, ":::%s",temp)

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
		else
			show_motd( id, text, "Player's Pokemon" )
	}
	return PLUGIN_HANDLED
 }
 /////////////////////////////////////////////////////
 //  When someone wants see what other people have  ////Items//
 /////////////////////////////////////////////////////
 public PlayersItems(id, bool:motd)
 {
	new text[1501], len = 1500, n, temp[256], iLen
	new players[MAX_PLAYERS], pnum, pid, name[32]
	poke_get_players(players, pnum)

	new bool:its_a_motd = true
	#if MOD==NS
		its_a_motd = false
	#endif

	if(motd)
		if(its_a_motd)
			n += copy(text[n],len-n,"<center><b>Player's Items</b></center><br>")
		else
			n += copy(text[n],len-n,"Player's Items<br>")
	else
		console_print(id, ":::Player's Items:::")

	new x, j, teamName[71]
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
					iLen += formatex(temp[iLen], (255-iLen), "<br><b>%s</b> -", name)
				}
				else
					iLen += formatex(temp[iLen], (255-iLen), "<br>%s -", name)
			}
			else
				console_print(id, "::%s",name)

			for( j=0; j<ItemsCreated; j++){
				if(HasPokeItem(pid,j)){
					iLen += formatex(temp[iLen], (255-iLen), " %s(%d)", ItemToName(j), PlayersItem[pid][j])
				}
				else if(j==0 && !PokeItem(pid)){
					iLen += copy(temp[iLen], (255-iLen), " None")
					break
				}
			}

			if(motd)
				n += copy(text[n],len-n,temp)
			else
				console_print(id, ":::%s",temp)

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
		else
			show_motd( id, text, "Player's Items" )
	}
	return PLUGIN_HANDLED
 }

 ///////////////////////////////////////////////////////////////
 //  When someone wants to look at pokedex (through console)  //
 ///////////////////////////////////////////////////////////////
 public Console_Pokedex(id)
 {
	new arg[32]
	read_argv(1,arg,31)
	pokedex(id, arg, false)
	return PLUGIN_HANDLED
 }
 public Console_Pokeitem(id)
 {
	new arg[32]
	read_argv(1,arg,31)
	itemhelp(id, NameToItem(arg,false), false)
	return PLUGIN_HANDLED
 }
 
 /////////////////////////////////////////////
 //  When someone wants to look at pokedex  //
 /////////////////////////////////////////////
 public pokedex(id, said[], bool:motd)
 {
	if( equali(said, "pokedex", 7) )
		copy( said, 191, said[7]);
	if( equali(said, " ", 1) )
		copy( said, 191, said[1]);

	new start;
	if( is_str_num(said) )
		start = str_to_num(said);
	else{
		if( !(start = NameToPoke(said)) ){
			if( id )
				HelpMenu(id);
			else
				console_print(id, "%s Please specify a pokemon.",PREFIX);
			return PLUGIN_HANDLED;
		}
	}

	new text[1501], len = 1500, n, i, line_format[255];
	new ename[NAME_LENGTH];

	new bool:its_a_motd = true;
	#if MOD==NS
		its_a_motd = false;
	#endif

	if(motd){
		if(its_a_motd)
			n += copy(text[n],len-n,"<center>")
		n += copy(text[n],len-n,"Say \"/pokedex #\" or \"/pokedex name\" to see other pokemon.<br><br>")
		n += copy(text[n],len-n,"Pokemon(evolve) - skill - skill description<br><br>")
		if(its_a_motd)
			n += copy(text[n],len-n,"<table border=\"1\"><tr><td><img src=\"http://img65.imageshack.us/img65/3504/battlechart7nl.gif\" width=\"389\" height=\"266\"></td></tr></table><br>")
	}

	new evolve, show_pics = POKEDEX_IMAGES;
	for(i = start; i < start+POKEDEX_PERPAGE; i++){

		if( i > PokesCreated )
			break;

		if( ValidPoke(i) ){
			if( PokeInfo[i] & PI_EVOLVE ){
				if( (evolve = OtherEvolve(i)) ){
					if( evolve == TRADE_EVOLVE )
						copy(ename,25,"(Trading)")
					else
						formatex(ename,25,"(%s)",ItemToName(-1*evolve))
				}
				else if( (evolve = LevelEvolve(i)) )
					formatex(ename,25,"(Level %d)",evolve)
			}
			else
				copy(ename, 25, EMPTY)

			if(motd && its_a_motd && show_pics)
				formatex(line_format, 255, "%d. %s %s%s - %s - %s<br>",i,PokeImageHtml(i),PokeToName(i),ename,SkillToName(PokeSkill[i]),SkillDescription(PokeSkill[i],id))
			else
				formatex(line_format, 255, "%d. %s%s - %s - %s%s",i,PokeToName(i),ename,SkillToName(PokeSkill[i]),SkillDescription(PokeSkill[i],id),motd?"<br>":EMPTY)

			if(motd)
				n += copy(text[n],len-n,line_format)
			else
				console_print(id, line_format)
		}
		else if(motd) n += copy(text[n],len-n,"<br>")
	}

	if(motd && 0<id<=MAX_PLAYERS){
		if(!its_a_motd){
			replace_all(text, 1500, "<li>", "\n-")
			replace_all(text, 1500, "<br>", "\n")
			set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
			show_hudmessage(id,text)
		}
		else
			show_motd( id, text, "Pokedex" )
	}
	return PLUGIN_HANDLED
 }
 public itemhelp(id, itemstart, bool:motd)
 {
	if( itemstart >= MAX_ITEMS || itemstart < 0 ){
		if( id )
			HelpMenu(id);
		else
			console_print(id, "%s Please specify a pokeitem.",PREFIX);
		return PLUGIN_HANDLED;
	}

	new text[1501], len = 1500, n, i, line_format[255];
	new bool:its_a_motd = true;
	#if MOD==NS
		its_a_motd = false;
	#endif

	if(motd){
		if(its_a_motd)
			n += copy(text[n],len-n,"<center>")
		n += copy(text[n],len-n,"PokeItems:<br><br>")
	}

	for(i = itemstart; i < itemstart+10; i++){

		if( i > ItemsCreated )
			break;

		formatex(line_format, 255, "%s - %s%s",ItemToName(i),ItemDesc[i],motd?"<br>":EMPTY)

		if(motd)
			n += copy(text[n],len-n,line_format)
		else
			console_print(id, line_format)
	}

	if(motd && 0<id<=MAX_PLAYERS){
		if(!its_a_motd){
			replace_all(text, 1500, "<li>", "\n-")
			replace_all(text, 1500, "<br>", "\n")
			set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
			show_hudmessage(id,text)
		}
		else
			show_motd( id, text, "PokeItems" )
	}
	return PLUGIN_HANDLED
 }
 
 
 PrintTeamName(team, id)
 {
	switch(team)
	{
		case 2:{
			#if MOD==CS
				console_print(id, ":Counter-Terrorists:")
			#endif
			#if MOD==DOD
				console_print(id, ":Axis:")
			#endif
			#if MOD==NS
				console_print(id, ":Aliens:")
			#endif
		}
		case 1:{
			#if MOD==CS
				console_print(id, ":Terrorists:")
			#endif
			#if MOD==DOD
				console_print(id, ":Allies:")
			#endif
			#if MOD==NS
				console_print(id, ":Marines:")
			#endif
		}
		default:
			console_print(id, ":Spectators:")
	}
 }
 GetTeamName(team, bool:its_a_motd, teamName[], num)
 {
	switch(team)
	{
		case 2:{
			#if MOD==CS
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"blue\">Counter-Terrorists</font> :</b>")
				else
					copy(teamName,num,"<br>Counter-Terrorists :")
			#endif
			#if MOD==DOD
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"red\">Axis</font> :</b>")
				else
					copy(teamName,num,"<br>Axis :")
			#endif
			#if MOD==NS
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"red\">Aliens</font> :</b>")
				else
					copy(teamName,num,"<br>Aliens :")
			#endif
		}
		case 1:{
			#if MOD==CS
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"red\">Terrorists</font> :</b>")
				else
					copy(teamName,num,"<br>Terrorists :")
			#endif
			#if MOD==DOD
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"blue\">Allies</font> :</b>")
				else
					copy(teamName,num,"<br>Allies :")
			#endif
			#if MOD==NS
				if(its_a_motd)
					copy(teamName,num,"<br><b><font color=\"blue\">Marines</font> :</b>")
				else
					copy(teamName,num,"<br>Marines :")
			#endif
		}
		default:{
			if(its_a_motd)
				copy(teamName,num,"<br><b><font color=\"grey\">Spectators</font> :</b>")
			else
				copy(teamName,num,"<br>Spectators :")
		}
	}
 }
 
 ShowPokeInfo(id, pid)
 {
	poke_print(id, print_chat, "[Pokedex] %s - %s - %s",PokeToName(pid),SkillToName(PokeSkill[pid]),SkillDescription(PokeSkill[pid],id));
 }

 
 ////////////////////////////////////////////
 //  When someone wants see their pokemon  //
 ////////////////////////////////////////////
 MyPoke(id)
 {
	new text[1501], len = 1500, n, i, pid;
	new pname[NAME_LENGTH];

	new bool:its_a_motd = true;
	#if MOD==NS
		its_a_motd = false;
	#endif

	new show_pics = POKEDEX_IMAGES;

	if(its_a_motd)
		n += copy(text[n],len-n,"<center><b>")
	n += copy(text[n],len-n,"Pokemon<br>")
	if(its_a_motd)
		n += copy(text[n],len-n,"<table border=\"1\"><tr>")
	if( Pokemon[id] ){

		pid = Pokemon[id];
		PokeToName( pid, pname, NAME_LENGTH );

		if(its_a_motd)
			n += copy(text[n],len-n,"<td><center>")

		if( show_pics )
			n += formatex(text[n],len-n,"%s ",PokeImageHtml(pid))
		n += formatex(text[n],len-n,"%s<br>",pname)

		n += copy(text[n],len-n, "Type(s):")

		if( PokeType[pid] & TYPE_GRASS )
			n += copy(text[n],len-n, " Grass")
		if( PokeType[pid] & TYPE_FIRE )
			n += copy(text[n],len-n, " Fire")
		if( PokeType[pid] & TYPE_WATER )
			n += copy(text[n],len-n, " Water")
		if( PokeType[pid] & TYPE_ROCK )
			n += copy(text[n],len-n, " Rock")
		if( PokeType[pid] & TYPE_ELECTRIC )
			n += copy(text[n],len-n, " Electric")
		if( PokeType[pid] & TYPE_PSYCHIC )
			n += copy(text[n],len-n, " Psychic")
		if( PokeType[pid] & TYPE_BUG )
			n += copy(text[n],len-n, " Bug")
		if( PokeType[pid] & TYPE_POISON )
			n += copy(text[n],len-n, " Poison")
		if( PokeType[pid] & TYPE_GHOST )
			n += copy(text[n],len-n, " Ghost")
		if( PokeType[pid] & TYPE_NORMAL )
			n += copy(text[n],len-n, " Normal")
		if( PokeType[pid] & TYPE_ICE )
			n += copy(text[n],len-n, " Ice")
		if( PokeType[pid] & TYPE_FIGHTING )
			n += copy(text[n],len-n, " Fighting")
		if( PokeType[pid] & TYPE_GROUND )
			n += copy(text[n],len-n, " Ground")
		if( PokeType[pid] & TYPE_FLYING )
			n += copy(text[n],len-n, " Flying")
		if( PokeType[pid] & TYPE_DRAGON )
			n += copy(text[n],len-n, " Dragon")

		n += copy(text[n],len-n,"<br>")


		new poke_skills[MAX_SKILLS], skill_count;
		for( i = 0; i < MAX_SKILLS; i++ )
			poke_skills[i] = -1;

		new tempskill, tempcheck, bool:skill_found;
		for( new i = -1; i<MAX_ACTIVE_POKES; i++ ){
			if( i != -1 ){
				pid = PlayersPoke[id][i];

				//if weve already added the skill in, we don't need to add it again
				if( i == PlayersActive[id] )
					continue;
			}
			while( pid && skill_count < MAX_SKILLS ){
				if( !PokeDisabled(pid) && (tempskill = PokeSkill[pid]) != -1 ){
					if( i==-1 || PokeInfo[pid] & PI_GLOBAL ){
						skill_found = false;
						for( tempcheck=0; tempcheck < skill_count; tempcheck++ )
							if( poke_skills[tempcheck] == tempskill )
								skill_found = true;
						if( !skill_found ){
							poke_skills[skill_count] = tempskill;
							skill_count++;
						}
					}
				}

				pid = AnyEvolveTo(pid);
			}
		}
		for( i = 0; i < skill_count; i++ )
			n += formatex(text[n],len-n, "%s - %s<br>",SkillToName(poke_skills[i]),SkillDescription(poke_skills[i],id))

		if(its_a_motd)
			n += copy(text[n],len-n,"</td>")
	}
	if(its_a_motd)
		n += copy(text[n],len-n, "<td>")
	else
		n += copy(text[n],len-n, "<br>")

	for(i = 0; i < MAX_ACTIVE_POKES; i++){
		pid = PlayersPoke[id][i];
		if( !pid ) break;
		if( show_pics )
			n += formatex(text[n],len-n, "%d. %s %s - Level %d - %s<br>",i+1,PokeImageHtml(pid),PokeToName(pid),PlayersLevel(id,i),SkillToName(PokeSkill[pid]))
		else
			n += formatex(text[n],len-n, "%d. %s - Level %d - %s<br>",i+1,PokeToName(pid),PlayersLevel(id,i),SkillToName(PokeSkill[pid]))
	}

	if(its_a_motd)
		n += copy(text[n],len-n, "</td></tr></table>")

	if( 0 < id <= MAX_PLAYERS ){
		if(!its_a_motd){
			replace_all(text, 1500, "<li>", "\n-")
			replace_all(text, 1500, "<br>", "\n")
			set_hudmessage(230,100,10,0.80,0.28, 0, 1.0, 20.0, 0.9, 0.9, -1)
			show_hudmessage(id,text)
		}
		else
			show_motd( id, text, "Pokemon" )
	}
	return PLUGIN_HANDLED
 }

