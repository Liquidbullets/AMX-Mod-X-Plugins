 
 /////////////////////
 //  Handles Menus  //
 /////////////////////
 public poke_menu_handler(id, menu, item)
 {
	new access, cmd[9], iName[64], callback, key;

	if( item == MENU_EXIT )
		key = POKE_MENU_EXIT;
	else {
		menu_item_getinfo(menu, item, access, cmd,8, iName,63, callback);
		key = str_to_num(cmd);
	}

	if( key == POKE_MENU_EXIT)
	{
		debugMessage(id,2,"Menu", "exited a menu=%d, item=%d, Action=%s", menu,item,ActionToName(Action[id]) )

		menu_destroy(menu);

		if(Action[id] == AC_PC){
			new parm[2];
			parm[0] = id;
			parm[1] = FIND_POKECENTER;
			set_task(0.1,"PokeMenu", 0, parm, 2);
			return PLUGIN_HANDLED;
		}
		if( Action[id] == AC_SKILL && item == MENU_EXIT )
			return PLUGIN_HANDLED;

		SetActiveSkill(id);
		SetAction( id );

		return PLUGIN_HANDLED;
	}

	debugMessage(id,2,"Menu", "used a menu=%d, item=%s(%s), Action=%d", menu,iName,cmd,ActionToName(Action[id]) )

	//switch for different menus
	switch(Action[id])
	{
		case AC_START:{
			if(!PlayersPoke[id][0] && !PlayersPoke[id][MAX_ACTIVE_POKES]){
				if(Safari()){
					set_task(0.1,"SafariMenu",id)
					return PLUGIN_CONTINUE
				}
				Pokemon[id] = key;
				PlayersPoke[id][0] = Pokemon[id];
				PlayersXP[id][0] = 1;
				PlayersLevels[id][0] = 1;
				set_bind(id);
				SetSpeed(id);
				ShowPokeInfo(id, Pokemon[id]);
			}
		}

		case AC_MAIN:{
			switch(key)
			{
				case 0: set_task(0.1,"ShowShopMenu",id)
				case 1: start_heal(id)
				case 2: set_task(0.1,"PCMenu",id)
				case 3: set_task(0.1,"HelpMenu",id)
			}
		}

		case AC_MART:{
			switch(key)
			{
				//opening/closing categories
				case MART_CAT_OFFSET..MART_CAT_OFFSET+MAX_ITYPE+3:{
					new num=key-MART_CAT_OFFSET
					if(Open[id][num]){
						Open[id][num] = false
						set_task(0.1,"ShowShopMenu",id)
					}
					else{
						Open[id][num] = true
						new parm[2];
						parm[0] = id;
						parm[1] = num;
						set_task(0.1, "ShowShopCat", 0, parm, 2);
					}
				}
				//selling items
				case MART_SELL_OFFSET..MART_SELL_OFFSET+MAX_ITEMS:{
					SellItem(id, key-MART_SELL_OFFSET)
					menu_destroy(menu)
					set_task(0.1,"ShowShopMenu",id)
					return PLUGIN_CONTINUE
				}
				//buying items/pokemon
				default:{
					if(BuyCheck(id)){
						if(key >= MART_POKE_OFFSET){
							buy_poke( id, key-MART_POKE_OFFSET );
						}
						else{
							buy_item(id, key)
							set_task(0.1,"ShowShopMenu",id)
						}
					}
				}
			}
		}

		case AC_GO:{
			if(PlayersPoke[id][key] && key<MAX_ACTIVE_POKES){
				SwitchPoke(id, PlayersPoke[id][key], false)
			}
		}

		case AC_RELEASE:{
			ReleasePoke(id, PlayersPoke[id][key])
		}

		case AC_PC_GIVE:{
			switch(AvailableSlot(id,PlayersPoke[id][key],CHECK_PC))
			{
				case AS_OK:{
					if( key != PlayersActive[id] ){
						new tempslot = Slot(id,0,CHECK_PC)
						PlayersPoke[id][tempslot] = PlayersPoke[id][key]
						PlayersXP[id][tempslot] = PlayersXP[id][key]

						PlayersPoke[id][key] = 0;
						PlayersXP[id][key] = 0;
						RefreshLevels(id);

						poke_printlang(id,"POKEMOD_PCSENT", PokeToName(PlayersPoke[id][tempslot]));
					}
					else{
						poke_printlang(id,"POKEMOD_PCNOACTIVE");
					}

					SaveXP(id)
					menu_destroy(menu)
					set_task(0.1,"ActionMenu",id)
					return PLUGIN_HANDLED
				}
				case AS_NOGOOD:{
					poke_printlang(id,"POKEMOD_PCSENDFAIL")
				}
				case AS_NOROOM:{
					poke_printlang(id,"POKEMOD_PCSENDROOM")
				}
				case AS_DUPLICATE:{
					poke_printlang(id,"POKEMOD_PCSENDDUP")
				}
			}
		}
		case AC_PC_TAKE:{
			switch(AvailableSlot(id,PlayersPoke[id][key]))
			{
				case AS_OK:{
					new tempslot = Slot(id)
					PlayersPoke[id][tempslot] = PlayersPoke[id][key]
					PlayersXP[id][tempslot] = PlayersXP[id][key]

					PlayersPoke[id][key] = 0
					PlayersXP[id][key] = 0
					RefreshLevels(id);

					poke_printlang(id,"POKEMOD_PCRET", PokeToName(PlayersPoke[id][tempslot]));

					SaveXP(id)
					menu_destroy(menu)
					set_task(0.1,"ActionMenu",id)
					return PLUGIN_HANDLED
				}
				case AS_NOGOOD:{
					poke_printlang(id,"POKEMOD_PCRETFAIL")
				}
				case AS_NOROOM:{
					poke_printlang(id,"POKEMOD_PCRETROOM")
				}
				case AS_DUPLICATE:{
					poke_printlang(id,"POKEMOD_PCRETDUP")
				}
			}
		}

		case AC_GIVE_ITEM:{
			if(PlayersPoke[id][key]){
				if(ItemInfo[PlayerGive[id]] & II_LEVEL )
					LevelUpPokemon(id, key, 1, PlayerGive[id])
				else
					Check_Stone_Evolve(id, PlayersPoke[id][key], PlayerGive[id])
			}
		}

		case AC_PC:{
			if(NearOakCheck(id) || !Pokemod_Alive(id)){
				switch(key)
				{
					case 0:{
						menu_destroy(menu)
						SetActionMenu( id, AC_RELEASE )
						return PLUGIN_HANDLED
					}
					case 2:{
						if( SaveXP(id, 0) )
							poke_printlang(id,"POKEMOD_SAVED")
					}
					case 4:{
						if(Safari()){
							menu_destroy(menu)
							set_task(0.1,"SafariMenu",id)
							return PLUGIN_CONTINUE
						}
						menu_destroy(menu)
						SetActionMenu( id, AC_PC_GIVE )
						return PLUGIN_HANDLED
					}
					case 5:{
						menu_destroy(menu)
						SetActionMenu( id, AC_PC_TAKE )
						return PLUGIN_HANDLED
					}
					case 6:{
						PlayersSet[id][SET_HUD_Y]--
						set_task(0.1,"PCMenu",id)
					}
					case 7:{
						PlayersSet[id][SET_HUD_Y]++
						set_task(0.1,"PCMenu",id)
					}
					case 8:{
						PlayersSet[id][SET_HUD_PARTY] = !PlayersSet[id][SET_HUD_PARTY]
						set_task(0.1,"PCMenu",id)
					}
				}
			}
			else
				poke_printlang(id,"POKEMOD_OAKPC")
		}

		case AC_ITEM:{
			menu_destroy(menu)
			if( HasPokeItem( id , key ) )
				UseItem( id , key )
			return PLUGIN_HANDLED
		}

		case AC_SAFARI:{
			Pokemon[id] = key
			ShowPokeInfo(id, key)
		}

		case AC_HELP:{
			SetAction( id )
			if( key < 0 ){
				itemhelp(id, (key*-1)-1, true);
			}
			else{
				switch(key)
				{
					case 0: pokehelp(id)
					default:{
						new num[25]
						formatex(num, 25, "%d", (key*POKEDEX_PERPAGE)-(POKEDEX_PERPAGE-1) )
						pokedex(id, num, true)
					}
				}
			}
			return PLUGIN_HANDLED
		}

		case AC_SKILL:{
			menu_destroy(menu);
			if( key != ActiveSkill(id) )
				CheckRepUsed(id);
			SetActiveSkill(id, key);
			set_task(0.1, "SkillMenu", id);
			return PLUGIN_HANDLED;
		}

		case AC_SELECT:{
			switch( PlayerSelect[id][0] )
			{
				case SEL_EVOLVE:{
					new pid = PlayerSelect[id][1];
					if( key ){
						new epid = PlayerSelect[id][2];
						new name[32],pnum = HasPokemon(id, pid, CHECK_ALL);
						get_user_name(id, name, 31);

						PlayersPoke[id][pnum] = epid;
						if(pnum==PlayersActive[id])
							Pokemon[id] = epid;
						poke_printlang(LANG_PLAYER,"POKEMOD_EVOLVE",name,PokeToName(pid),PokeToName(epid))
					}
					else
						poke_printlang(id,"POKEMOD_CANCELEVO",PokeToName(pid))

					PlayerSelect[id][0] = MAX_SEL;
				}
			}
		}
	}

	SetAction( id )
	menu_destroy(menu);

	return PLUGIN_HANDLED;
 }
 pokemenu_create( title[], any:... )
 {
	static output[128];
	vformat(output, 127, title, 2);
	if( !g_coloredMenus ) pokemenu_removecolors( output, 127 );
	return menu_create( output, "poke_menu_handler");
 }
 pokemenu_additem(const menu, const num, const text[], any:...)
 {
	static output[128], numstring[9];
	vformat(output, 127, text, 4);
	num_to_str(num, numstring, 8);
	if( !g_coloredMenus ) pokemenu_removecolors( output, 127 );
	return menu_additem(menu, output, numstring, 0);
 }
 pokemenu_fakeexit(const menu)
 {
	pokemenu_additem(menu, POKE_MENU_EXIT, LANG,0,"POKEMOD_EXIT");
	pokemenu_noexit(menu);
 }
 pokemenu_exit(const menu)
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
 pokemenu_noexit(const menu)
	menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
 pokemenu_removecolors( output[], len )
 {
	replace_all(output,len, "\\R","     ");
	replace_all(output,len, "\\r",COLON);
	replace_all(output,len, "\\w",EMPTY);
	replace_all(output,len, "\\y",DASH);
 }

 //////////////////
 //  Start Menu  //
 //////////////////
 public StartMenu(id)
 {
	if( !Pokemod_Connected(id) )
		return
	if( is_user_bot(id) ){
		BotPoke(id)
		return
	}

	if( Safari() ){
		SafariMenu(id)
		return
	}

	if( !Pokemod_Connected(id) )
		return
	if( PlayersPoke[id][0] || PlayersPoke[id][MAX_ACTIVE_POKES] )
		return

	new menu = pokemenu_create( "\\r%L:",id,"POKEMOD_STARTPOKE" )
	new pokesinmenu;

	for( new i=0; i<=PokesCreated; i++ ){
		if( PokeInfo[i] & PI_START ){
			pokemenu_additem( menu, i, "\\y%-20s\\R\\w%s", POKENAMES[i], SkillToName( PokeSkill[i] ) );
			pokesinmenu++;
		}
	}
	pokemenu_noexit(menu);

	if( pokesinmenu ){
		menu_display(id, menu, 0)
		debugMessage(id,6,"Menu", "was shown Start menu");

		SetAction( id, AC_START )
	}
 }

 ///////////////////
 //  Action Menu  //
 ///////////////////
 SetActionMenu(id, action)
 {
	Action[id] = action;
	ActionMenu( id );
	SetAction( id, action );
 }
 public ActionMenu(id)
 {
	if(!Pokemod_Connected(id))
		return

	new menu;

	switch(Action[id])
	{
		case AC_PC_GIVE:{
			if(Safari()){
				set_task(0.1,"SafariMenu",id)
				return
			}
			menu = pokemenu_create( "\r%L:",id,"POKEMOD_MENUSEND" )
		}
		case AC_PC_TAKE:{
			if(Safari()){
				set_task(0.1,"SafariMenu",id)
				return
			}
			else if( !PlayersPoke[id][MAX_ACTIVE_POKES] ){
				poke_printlang(id,"POKEMOD_NOPCPOKE");
				return
			}
			menu = pokemenu_create( "\r%L:",id,"POKEMOD_MENURET" )
		}
		case AC_RELEASE:{
			if(Safari()){
				set_task(0.1,"SafariMenu",id)
				return
			}
			menu = pokemenu_create( "\r%L:",id,"POKEMOD_MENUREL" )
		}
		case AC_GO:{
			if(Safari()){
				set_task(0.1,"SafariMenu",id)
				return
			}
			menu = pokemenu_create( "\r%L:",id,"POKEMOD_MENUGO" )
		}
		case AC_GIVE_ITEM:{
			menu = pokemenu_create( "\r%L:",id,"POKEMOD_MENUGIVEITEM", ItemToName(PlayerGive[id]) )
		}
	}
	
	RefreshPoke(id)

	new i, start, stop;
	start = 0;
	stop = MAX_ACTIVE_POKES;

	if( Action[id] == AC_PC_TAKE ){
		start = MAX_ACTIVE_POKES;
		stop = MAX_POKES;
	}

	for(i = start; i<stop; i++){
		if(PlayersPoke[id][i])
			pokemenu_additem( menu, i, "\\w%-20s\\R%L %d", PokeToName(PlayersPoke[id][i]), id,"POKEMOD_LEVEL", PlayersLevel(id, i))
	}
	pokemenu_exit(menu);

	menu_display(id, menu, 0)
	debugMessage(id,6,"Menu", "was shown an Action menu");
 }

 /////////////////
 //  Main Menu  //
 /////////////////
 public PokeMenu(parm[])
 {
	new id = parm[0]
	new oldcenter = parm[1]

	new center;
	//if oldcenter == FIND_POKECENTER, we need to find a new one
	if( oldcenter == FIND_POKECENTER )
		center = NearOakCheck(id);
	else
		center = oldcenter;

	//stop if not valid pokecenter or if theyre not connected
	if( center < 0 || !Pokemod_Connected(id) )
		return

	new playermenu, playerkeys
	get_user_menu(id, playermenu, playerkeys)
	if(playermenu)
		return

	if(!PlayersPoke[id][0] && !PlayersPoke[id][MAX_ACTIVE_POKES])
		StartMenu(id)
	else{
		if( NearOakCheck(id) >= 0 ){

			new oak_props[MAX_OAKPROP]
			if(pev_valid(center))
				pev(center, pev_globalname, oak_props, MAX_OAKPROP-1)
			else	//lets let them be able to do everything while dead
				copy(oak_props, MAX_OAKPROP-1, "a")

			new menu = pokemenu_create( "\\r%L:",id,"POKEMOD_MENUMAIN" )

			if(get_ppcvar_num(pm_pokemart_open) && containi(oak_props,"a")!=-1)
				pokemenu_additem( menu, 0, "\\w%L",id,"POKEMOD_POKEMART" );
			if(containi(oak_props,"b")!=-1)
				pokemenu_additem( menu, 1, "\\w%L",id,"POKEMOD_HEAL" );

			pokemenu_additem( menu, 2, "\\w%L",id,"POKEMOD_PCNAME" );
			pokemenu_additem( menu, 3, "\\r%L",id,"POKEMOD_POKEHELP" );

			pokemenu_fakeexit(menu);

			menu_display(id, menu, 0)
			debugMessage(id,6,"Menu", "was shown Main menu");

			SetAction( id, AC_MAIN )
		}
	}
 }

 /////////////////
 //  Shop Menu  //
 /////////////////
 public ShowShopMenu(id) ShopMenu(id, OpenCat(id))
 public ShowShopCat(parm[]) ShopMenu(parm[0], parm[1])
 ShopMenu(id, icat=-1)
 {
	if(!Pokemod_Connected(id))
		return

	if( BuyCheck(id) )
	{
		new cost, bool:showed_cat = false
		new i, pname[NAME_LENGTH]

		new menu = pokemenu_create( "\\r%L:", id,"POKEMOD_POKEMART" )

		new cat, catsdisplay;
		if( icat == MAX_ITYPE+1 || icat == MAX_ITYPE+2 ){
			catsdisplay = cat = icat;
		}
		else if( icat < 0 || icat >= ItemCatCreated ){
			cat = 0;
			catsdisplay = ItemCatCreated;
		}
		else{
			cat = icat;
			catsdisplay = cat+1;
		}

		while( cat < catsdisplay )
		{
			pokemenu_additem( menu, MART_CAT_OFFSET+cat, "\\r%-20s\\R(%s)", ItemCatName[cat], Open[id][cat]?DASH:PLUS)

			if(Open[id][cat]){
				showed_cat = true;
				for(i = 0; i < ItemsCreated; i++){
					if( !(ItemInfo[i] & II_SPECIAL)
					&& ItemCat[i] == cat ){
						cost = CostOfItem(i, true)
						if(cost){
							ItemToName(i, pname, NAME_LENGTH)
							if(cost<0){
								if(!is_user_admin(id))
									continue
								pokemenu_additem( menu, i, "\\y%-20s\\R%d", pname, abs(cost) )
							}
							else
								pokemenu_additem( menu, i, "\\w%-20s\\R%d", pname, abs(cost) )
						}
					}
				}
			}
			cat++;
		}
		if( !showed_cat ){
			if( PokeItem(id) && icat != MAX_ITYPE+2 ){
				formatex( pname, NAME_LENGTH-1, LANG, id,"POKEMOD_SELLITEMS");
				pokemenu_additem( menu, MART_CAT_OFFSET+MAX_ITYPE+1, "\\y%-20s\\R(%s)", pname, Open[id][MAX_ITYPE+1]?DASH:PLUS)

				if(Open[id][MAX_ITYPE+1]){
					showed_cat = true;
					for(i = 0; i < ItemsCreated; i++){
						if(HasPokeItem(id, i)){
							cost = floatround(CostOfItem(i, true)*SELLFACTOR)
							if(cost){
								ItemToName(i, pname, NAME_LENGTH)
								if(cost<0){
									if(!is_user_admin(id))
										continue
									pokemenu_additem( menu, MART_SELL_OFFSET+i, "\\y%-20s\\R%d",pname, abs(cost))
								}
								else
									pokemenu_additem( menu, MART_SELL_OFFSET+i, "\\w%-20s\\R%d",pname, abs(cost))
							}
						}
					}
				}
			}
		}

		if( !showed_cat ){
			pokemenu_additem( menu, MART_CAT_OFFSET+MAX_ITYPE+2, "\\r%-20s\\R(%s)", "Pokemon", Open[id][MAX_ITYPE+2]?DASH:PLUS)

			if( Open[id][MAX_ITYPE+2] ){
				for(i=1; i <= PokesCreated; i++){
					cost = CostOfPoke(i)
					if(cost){
						PokeToName(i, pname, NAME_LENGTH)
						if(cost<0){
							if(!is_user_admin(id))
								continue
							pokemenu_additem( menu, MART_POKE_OFFSET+i, "\\y%-20s\\R%d", pname, abs(cost))
						}
						else
							pokemenu_additem( menu, MART_POKE_OFFSET+i, "\\w%-20s\\R%d", pname, abs(cost))
					}
				}
			}
		}

		pokemenu_exit(menu);

		menu_display(id, menu, 0)
		debugMessage(id,6,"Menu", "was shown PokeMart menu");

		SetAction( id, AC_MART )
	}
 }

 //////////////////
 //  PC Menu  //
 //////////////////
 public PCMenu(id)
 {
	if(!Pokemod_Connected(id))
		return

	new menu = pokemenu_create( "\\r%L:",id,"POKEMOD_PCNAME" )
	if(!Safari())
		pokemenu_additem( menu, 0, "\\w%L",id,"POKEMOD_PCRELEASE" )
	if(SaveXPon()){
		pokemenu_additem( menu, 2, "\\w%L",id,"POKEMOD_PCSAVE" )
	}
	if(Safari())
		pokemenu_additem( menu, 4, "\\w%L",id,"POKEMOD_PCCHANGE" )
	else{
		pokemenu_additem( menu, 4, "\\w%L",id,"POKEMOD_PCSEND" )
		pokemenu_additem( menu, 5, "\\w%L",id,"POKEMOD_PCRETRIEVE" )
	}
	pokemenu_additem( menu, 6, "\\w%L",id,"POKEMOD_PCHUDUP" )
	pokemenu_additem( menu, 7, "\\w%L",id,"POKEMOD_PCHUDDOWN" )
	if(!Safari()){
		if( PlayersSet[id][SET_HUD_PARTY] )
			pokemenu_additem( menu, 8, "\\w%L",id,"POKEMOD_PCHIDEPARTY" )
		else
			pokemenu_additem( menu, 8, "\\w%L",id,"POKEMOD_PCSHOWPARTY" )
	}

	menu_display(id, menu, 0)
	debugMessage(id,6,"Menu", "was shown PC menu");

	SetAction( id, AC_PC )
 }

 /////////////////////
 //  Item Use Menu  //
 /////////////////////
 public ItemUseMenu(id)
 {
	if(!Pokemod_Connected(id))
		return

	if(Pokemod_Alive(id))
	{
		if(PokeItem(id))
		{
			new menu = pokemenu_create( "\\r%L:",id,"POKEMOD_MENUYOURITEM" )
			new iname[NAME_LENGTH], num
			new bool:shown = false

			for( num = 0; num < ItemsCreated; num++)
			{
				if(HasPokeItem(id,num)){

					shown = false
					ItemToName(num,iname, NAME_LENGTH)

					if( ItemInfo[num] & II_BIKE ){
						pokemenu_additem( menu, num, "\\w%s (%L)", iname, id, g_Status[id][STATUS_BIKE]?"POKEMOD_RIDE":"POKEMOD_WALK" )
						shown = true
					}
					if( ItemInfo[num] & II_PDAMAGE ){
						pokemenu_additem( menu, num, "\\w%s (%L:%d)", iname, id,"POKEMOD_IADDPDAMAGE", PlayersItem[id][num]*ItemPDamage[num])
						shown = true
					}
					if( ItemInfo[num] & II_SPEED ){
						pokemenu_additem( menu, num, "\\w%s (%L:%d)", iname, id,"POKEMOD_IADDSPEED", PlayersItem[id][num]*ItemSpeed[num])
						shown = true
					}
					if( ItemInfo[num] & II_MAXHP ){
						pokemenu_additem( menu, num, "\\w%s (%L:%d)", iname, id,"POKEMOD_IADDHP", PlayersItem[id][num]*ItemMaxHP[num])
						shown = true
					}
					if( ItemInfo[num] & II_ARMOR ){
						pokemenu_additem( menu, num, "\\w%s (%L:%d)", iname, id,"POKEMOD_IADDARMOR", PlayersItem[id][num]*ItemArmor[num])
						shown = true
					}
					if( ItemInfo[num] & II_DAMAGE ){
						pokemenu_additem( menu, num, "\\w%s (%L:%d)", iname, id,"POKEMOD_IADDDAMAGE", PlayersItem[id][num]*ItemDamage[num])
						shown = true
					}
					if( ItemInfo[num] & II_CATCH ){
						pokemenu_additem( menu, num, "\\w%s (%L: %d%%)(%d)", iname, id,"POKEMOD_ICATCH", ItemCatch[num], PlayersItem[id][num] )
						shown = true
					}
					if( ItemInfo[num] & II_HEAL && ItemHeal[num] != -1 ){
						pokemenu_additem( menu, num, "\\w%s (%L)(%d)", iname, id,"POKEMOD_IHEAL", ItemHeal[num], PlayersItem[id][num] )
						shown = true
					}
					if( !shown ){
						pokemenu_additem( menu, num, "\\w%s (%d)", iname, PlayersItem[id][num])
					}
				}
			}
			pokemenu_exit(menu);

			menu_display(id, menu, 0)
			debugMessage(id,6,"Menu", "was shown Item menu");

			SetAction( id, AC_ITEM )
		}
		else
			ShopMenu(id, OpenCat(id))
	}
 }

 /////////////////
 //  Help Menu  //
 /////////////////
 public HelpMenu(id)
 {
	if( !Pokemod_Connected(id) )
		return;

	if( Action[id] == AC_HELP )
		return;

	new menu = pokemenu_create( "\\r%L:", id,"POKEMOD_POKEHELP" );
	pokemenu_additem( menu, 0, "\\y%L", id,"POKEMOD_POKEBASICS" );

	new i=0;
	while( i<=PokesCreated/POKEDEX_PERPAGE ){
		if( (POKEDEX_PERPAGE*i)+1 > PokesCreated ) break;
		pokemenu_additem( menu, i+1, "\\w%L (%d-%d)", id,"POKEMOD_POKEDEX", (POKEDEX_PERPAGE*i)+1, min((POKEDEX_PERPAGE*i)+POKEDEX_PERPAGE,PokesCreated));
		i++;
	}
	i=0;
	while( i<=ItemsCreated/10 ){
		if( (10*i)+1 > ItemsCreated ) break;
		pokemenu_additem( menu, (i*-10)-1, "\\w%L (%d-%d)", id,"POKEMOD_ITEMS", (10*i)+1, min((10*i)+10,ItemsCreated));
		i++;
	}

	pokemenu_exit(menu);

	menu_display(id, menu, 0);
	debugMessage(id,6,"Menu", "was shown Help menu");

	SetAction( id, AC_HELP );
 }

 //////////////////
 //  Skill Menu  //
 //////////////////
 public SkillMenu(id)
 {
	if( !Pokemod_Connected(id) )
		return;

	new menu = pokemenu_create( "\\r%L:", id,"POKEMOD_SKILLSLEARNED" );

	new skills[MAX_SKILLS], skillnames[MAX_SKILLS][NAME_LENGTH], skill_count, tempcheck, bool:skill_found;
	new active_skill = ActiveSkill(id);
	debugMessage(id,6,"Skill", "About to show Skill menu (active skill=%s)", SkillToName(active_skill) );

	new i, pid = Pokemon[id], tempskill;
	for( i = -1; i<MAX_ACTIVE_POKES; i++ ){
		if( i != -1 ){
			pid = PlayersPoke[id][i];

			//if weve already added the skill in, we don't need to add it again
			if( pid == PlayersActive[id] )
				continue;
		}
		while( pid && skill_count < MAX_SKILLS+1 ){
			if( !PokeDisabled(pid) && (tempskill = PokeSkill[pid]) != -1 ){
				if( SkillInfo[tempskill] & SI_BIND && ( i==-1 || PokeInfo[pid] & PI_GLOBAL ) ){
					skill_found = false;
					for( tempcheck=0; tempcheck < skill_count; tempcheck++ )
						if( skills[tempcheck] == tempskill )
							skill_found = true;

					if( !skill_found ){
						if( tempskill == active_skill )
							formatex( skillnames[skill_count], NAME_LENGTH-1, "\\y%s", SkillToName(tempskill) );
						else
							copy( skillnames[skill_count], NAME_LENGTH-1, SkillToName(tempskill) );

						skills[skill_count] = tempskill;
						skill_count++;
					}
				}
			}

			pid = AnyEvolveTo(pid);
		}
	}

	for( i=0; i < skill_count; i++)
		pokemenu_additem( menu, skills[i], skillnames[i] );

	pokemenu_fakeexit( menu );

	menu_display(id, menu, 0);
	debugMessage(id,6,"Menu", "was shown Skill menu (active skill=%s)", SkillToName(active_skill) );

	SetActiveSkill(id, active_skill);

	SetAction( id, AC_SKILL );
 }

 ///////////////////
 //  Safari Menu  //
 ///////////////////
 public SafariMenu(id)
 {
	if( !Pokemod_Connected(id) )
		return;

	if( Action[id] == AC_SAFARI )
		return;

	new menu = pokemenu_create( "\\r%L:", id,"POKEMOD_POKEMON" )

	new level;
	for(new i=1; i<=PokesCreated; i++){
		if( (level=SafariLevel(i)) )
			pokemenu_additem( menu, i, "\\w%-20s\\R%L %d", PokeToName(i), id,"POKEMOD_LEVEL", level)
	}

	pokemenu_exit(menu);

	menu_display(id, menu, 0);
	debugMessage(id,6,"Menu", "was shown Safari menu");

	SetAction( id, AC_SAFARI );
 }

 ///////////////////
 //  Select Menu  //
 ///////////////////
 public SelectMenu(id)
 {
	if( !Pokemod_Connected(id) )
		return;

	if( PlayerSelect[id][0] == MAX_SEL )
		return;

	new menu;
	switch( PlayerSelect[id][0] )
	{
		case SEL_EVOLVE:{
			menu = pokemenu_create( "\\r%L:", id,"POKEMOD_EVOLVING", PokeToName(PlayerSelect[id][1]) );
			pokemenu_additem( menu, 1, "\\w%L", id,"POKEMOD_ALLOW" );
			pokemenu_additem( menu, 0, "\\w%L", id,"POKEMOD_ABORT" );
			pokemenu_noexit(menu);
		}
	}

	menu_display(id, menu, 0);
	debugMessage(id,6,"Menu", "was shown Select menu");

	SetAction( id, AC_SELECT );
 }


 
 
 