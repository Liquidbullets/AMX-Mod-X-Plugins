
 //////////////////////////////////////
 //  When someone presses +pokeitem  //
 //////////////////////////////////////
 public pokeitem_down(id)
 {
	debugMessage(id,2,"Player", "pressed +pokeitem")

	g_isItemed[id] = true
	new aid, body
	get_user_aiming(id, aid, body)
	if(!Pokemod_Alive(id) || EntIsPC(aid)){
		ShopMenu(id, OpenCat(id))
		return PLUGIN_HANDLED
	}

	ItemUseMenu(id)
	return PLUGIN_HANDLED
 }

 ///////////////////////////////////////
 //  When someone releases +pokeitem  //
 ///////////////////////////////////////
 public pokeitem_up(id)
 {
	g_isItemed[id] = false
	return PLUGIN_HANDLED
 }

 ////////////////////////////////////////////////////
 //  Buy an item from oak or through say commands  //
 ////////////////////////////////////////////////////
 buy_item(id, item, num=1)
 {
	if(!PMon()){
		poke_printlang(id,"POKEMOD_DISABLED")
		return PLUGIN_HANDLED
	}
	if( !BuyCheck(id) ){
		#if MOD==CS
		poke_printlang(id,"POKEMOD_NOOAKBUY")
		#else
		poke_printlang(id,"POKEMOD_NOOAK")
		#endif
		return PLUGIN_HANDLED
	}
	if( !ValidItem(item) ){
		poke_printlang(id,"POKEMOD_NOITEM")
		return PLUGIN_HANDLED
	}
	if( ItemInfo[item] & II_SPECIAL ){
		poke_printlang(id,"POKEMOD_NOBUYITEM")
		return PLUGIN_HANDLED
	}
	#if MAX_ITEM_TOTAL > 0
	if( TotalItems(id)+num > MAX_ITEM_TOTAL){
		num = MAX_ITEM_TOTAL - TotalItems(id);
		if( !num )
			poke_printlang(id,"POKEMOD_MAXITEMS")
		else
			poke_printlang(id,"POKEMOD_MOREITEMS",num)
		return PLUGIN_HANDLED
	}
	#endif
	#if MAX_ITEM_AMOUNT > 0
	if( PlayersItem[id][item]+num > MAX_ITEM_AMOUNT){
		num = MAX_ITEM_AMOUNT - PlayersItem[id][item];
		if( !num )
			poke_printlang(id,"POKEMOD_MAXITEM")
		else
			poke_printlang(id,"POKEMOD_MOREITEM",num)
		return PLUGIN_HANDLED
	}
	#endif
	if( ItemInfo[item]&II_MAX && PlayersItem[id][item]+num > ItemMax[item] ){
		num = ItemMax[item] - PlayersItem[id][item];
		if( !num )
			poke_printlang(id,"POKEMOD_MAXITEM")
		else
			poke_printlang(id,"POKEMOD_MOREITEM",num)
		return PLUGIN_HANDLED
	}

	new cost = CostOfItem(item)
	if( cost<0 && !is_user_admin(id) ){//only admins can buy it
		poke_print(id, print_center,LANG,id,"POKEMOD_ADMINBUY")
		return PLUGIN_HANDLED
	}
	if( !cost ){
		poke_print(id, print_center,LANG,id,"POKEMOD_ITEMDIS")
		return PLUGIN_HANDLED
	}

	cost = abs(cost)*num

	if(PokePayForward > -1)
	{
		new functionReturn
		ExecuteForward(PokePayForward, functionReturn, id, -1*cost)
		// something else paid
		if(functionReturn==PLUGIN_HANDLED)
			cost = 0;
	}

	if( cost ){
		new money = Pokemod_Money(id)
		if(money < cost){//costs too much
			poke_print(id, print_center,LANG,id,"POKEMOD_NOFUNDS")
			return PLUGIN_HANDLED
		}
		Pokemod_Pay(id, -1*(cost))
	}

	PlayersItem[id][item] += num

	if(ItemInfo[item] & II_MAXHP){
		CheckMaxHP(id)
		Pokemod_Heal(id, ItemMaxHP[num]*num)
	}
	if( ItemInfo[item] & II_ARMOR ){
		CheckMaxArmor(id)
		Pokemod_Repair(id, ItemArmor[item]*num)
	}
	SetSpeed(id)

	if(is_user_bot(id))
		BotSay(id, BOT_BOUGHT_ITEM, num, item)
	else{
		poke_printlang(id,"POKEMOD_ITEMPACK", num, ItemToName(item));
		poke_print(id, print_chat,"%s - %s", ItemToName(item), ItemDesc[item]);
	}

	if( ItemEventForward > -1 )
	{
		new functionReturn
		ExecuteForward(ItemEventForward, functionReturn, id, IE_BUY, item)

		// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
		// and 0 is used by continue and invalid return.
		if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
		{
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED
 }

 //////////////////////////////
 //  Buy a pokemon from oak  //
 //////////////////////////////
 public buy_poke(id, poke)
 {
	if(!PMon()){
		poke_printlang(id,"POKEMOD_DISABLED")
		return PLUGIN_HANDLED
	}
	if( !BuyCheck(id) ){
		#if MOD==CS
		poke_printlang(id,"POKEMOD_NOOAKBUY")
		#else
		poke_printlang(id,"POKEMOD_NOOAK")
		#endif
		return PLUGIN_HANDLED
	}

	new pname[NAME_LENGTH]
	PokeToName(poke, pname, NAME_LENGTH)

	switch(AvailableSlot(id, poke))
	{
		case AS_NOGOOD:{
			poke_printlang(id,"POKEMOD_NOGOOD",pname)
			return PLUGIN_HANDLED
		}
		case AS_NOROOM:{
			poke_printlang(id,"POKEMOD_NOROOM")
			return PLUGIN_HANDLED
		}
		case AS_DUPLICATE:{
			poke_printlang(id,"POKEMOD_DUPLICATE",pname)
			return PLUGIN_HANDLED
		}
	}

	new cost = CostOfPoke(poke)

	if(cost<0 && !is_user_admin(id)){//only admins can buy it
		poke_print(id, print_center,LANG,id,"POKEMOD_ADMINBUY")
		return PLUGIN_HANDLED
	}
	if( !cost ){
		poke_print(id, print_center,LANG,id,"POKEMOD_POKEDIS")
		return PLUGIN_HANDLED
	}

	if(PokePayForward > -1)
	{
		new functionReturn
		ExecuteForward(PokePayForward, functionReturn, id, -1*cost)
		// something else paid
		if(functionReturn==PLUGIN_HANDLED)
			cost = 0;
	}

	if( cost ){
		new money = Pokemod_Money(id)
		if(money < abs(cost)){//costs too much
			poke_print(id, print_center,LANG,id,"POKEMOD_NOFUNDS")
			return PLUGIN_HANDLED
		}

		Pokemod_Pay(id, -1*abs(cost))
	}

	poke_printlang(id,"POKEMOD_BUYPOKE", pname)
	AddPoke(id, poke, 1)

	return PLUGIN_HANDLED
 }

 ///////////////////
 //  Use an item  //
 ///////////////////
 public UseItem(id, item)
 {
	if(!Pokemod_Alive(id) || !PMon()) return PLUGIN_HANDLED

	//check turns into 1 if item is used, and 2 if soundcure should be played
	new check = 0

	new iname[NAME_LENGTH]
	ItemToName(item, iname, NAME_LENGTH)
	debugMessage(id,2,"Player", "used %s",iname)

	if( ItemEventForward > -1 )
	{
		new functionReturn
		ExecuteForward(ItemEventForward, functionReturn, id, IE_USE, item)

		// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
		// and 0 is used by continue and invalid return.
		if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
		{
			return PLUGIN_HANDLED;
		}
	}

	if( ItemInfo[item] & II_DISABLE )
		return PLUGIN_HANDLED;

	if( ItemInfo[item] & II_HEAL ){
		if( Pokemod_Health(id) < PlayerStat[id][STAT_MAXHP] ){
			check = 2
			if( ItemHeal[item] == -1 )
				Pokemod_Heal(id, PlayerStat[id][STAT_MAXHP])
			else
				Pokemod_Heal(id, ItemHeal[item])
		}
	}
	if( ItemInfo[item] & II_REMEDY ){
		if( HasStatus(id) ){
			check = 2
			ResetStatus(id)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOSTATUS")
	}
	if( ItemInfo[item] & II_POISON ){
		if(g_Status[id][STATUS_POISON]){
			check = 2
			RemoveStatus(id, STATUS_POISON)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOPOISON")
	}
	if( ItemInfo[item] & II_ASLEEP ){
		if(g_Status[id][STATUS_ASLEEP]){
			check = 2
			unsleep(id)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOASLEEP")
	}
	if( ItemInfo[item] & II_BURN ){
		if(g_Status[id][STATUS_BURN]){
			check = 2
			RemoveStatus(id, STATUS_BURN)
			stopFireSound(id)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOBURNT")
	}
	if( ItemInfo[item] & II_FROZEN ){
		if(g_Status[id][STATUS_FROZEN]){
			check = 2
			RemoveStatus(id, STATUS_FROZEN)
			SetSpeed(id)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOFROZEN")
	}
	if( ItemInfo[item] & II_PARALYZE 
	||	ItemInfo[item] & II_SLOW	){
		if(g_Status[id][STATUS_PARALYZE] || g_Status[id][STATUS_SLOW]){
			check = 2
			RemoveStatus(id, STATUS_PARALYZE)
			RemoveStatus(id, STATUS_SLOW)
		}
		else if(!check) poke_printlang(id,"POKEMOD_NOPARA")
	}
	if( ItemInfo[item] & II_ESCAPE ){
		check = 1
		EscapeRope(id, 1)
		if(is_user_bot(id)){
			start_heal(id)
		}
	}
	if( ItemInfo[item] & II_FOSSIL ) {

		if( NearOakCheck(id) > 0 ){
			new pname[NAME_LENGTH]
			PokeToName(ItemFossil[item], pname, NAME_LENGTH)
			switch( AvailableSlot(id, ItemFossil[item]) )
			{
				case AS_OK:{
					new i = Slot(id)
					PlayersPoke[id][i] = ItemFossil[item]
					PlayersXP[id][i] = 1
					PlayersLevels[id][i] = 1;
					check = 1

					poke_print(id, print_chat, "%L: %L",id,"POKEMOD_OAKNAME",id,"POKEMOD_OAKFOSSIL", iname, pname)
				}
				case AS_NOGOOD:
					poke_print(id, print_chat, "%L: %L",id,"POKEMOD_OAKNAME",id,"POKEMOD_NOGOOD")
				case AS_NOROOM:
					poke_print(id, print_chat, "%L: %L",id,"POKEMOD_OAKNAME",id,"POKEMOD_NOROOM")
				case AS_DUPLICATE:
					poke_print(id, print_chat, "%L: %L",id,"POKEMOD_OAKNAME",id,"POKEMOD_DUPLICATE", pname)
			}
		}
		else
			poke_printlang(id,"POKEMOD_ITEMTOOAK", iname)
		if(!check) check = -1
	}
	if( ItemInfo[item] & II_BIKEV ) {
		if( NearOakCheck(id) > 0 ){
			poke_print(id,print_chat,"%L: %L",id,"POKEMOD_OAKNAME",id,"POKEMOD_OAKTRADE", iname, ItemToName(ItemItem[item]))
			PlayersItem[id][ItemItem[item]]++
			check = 1
		}
		else
			poke_printlang(id,"POKEMOD_ITEMTOOAK", iname)
		if(!check) check = -1
	}
	if( ItemInfo[item] & II_BIKE ) {
		if(g_Status[id][STATUS_BIKE]){
			RemoveStatus(id, STATUS_BIKE)
			poke_printlang(id,"POKEMOD_BIKESTOP", iname)
		}
		else{
			SetStatus(id, STATUS_BIKE)
			poke_printlang(id,"POKEMOD_BIKESTART", iname)
		}
		SetSpeed(id)
		if(!check) check = -1
	}
	#if RARE_CANDY_MODE == 2 || RARE_CANDY_MODE == 3
	if( ItemInfo[item] & II_LEVEL ){
		PlayerGive[id] = item
		SetActionMenu( id, AC_GIVE_ITEM )
		check = -1
	}
	#endif
	if( ItemInfo[item] & II_GIVE ){
		PlayerGive[id] = item
		SetActionMenu( id, AC_GIVE_ITEM )
		check = -1
	}

	if( !check && ItemInfo[item] & II_CATCH )
		poke_printlang(id,"POKEMOD_BALLAUTO")

	if( !check &&
		(	 ItemInfo[item] & II_PDAMAGE
		||	 ItemInfo[item] & II_DAMAGE
		||	 ItemInfo[item] & II_ARMOR
		||	 ItemInfo[item] & II_SPEED
		||	 ItemInfo[item] & II_MAXHP
		))
		poke_printlang(id,"POKEMOD_STATAUTO")

	if(check > 0){
		PlayersItem[id][item]--
		poke_printlang(id,"POKEMOD_USEITEM", iname)

		if( check == 2 )
			sound_cure(id)
	}
	else if( check > -1 )
		sound_deny(id)

	return PLUGIN_HANDLED
 }

 ////////////////////////////////////
 //  Returns if they have an item  //
 ////////////////////////////////////
 PokeItem(id)
 {
	if(!PMon()) return false

	for(new i = 0; i < ItemsCreated; i++)
		if(PlayersItem[id][i] > 0)
			return true
	return false
 }

 //////////////////////////////////////
 //  Determines if id has this item  //
 //////////////////////////////////////
 HasPokeItem(id, item)
 {
	if(!PMon() || !ValidItem(item) ) return 0;
	return max( PlayersItem[id][item], 0 );
 }

 //////////////////////////////
 //  Teleport user to spawn  //
 //////////////////////////////
 public EscapeRope(id, num)
 {
	if(num > 0)
		if(!PassTest(id, 0))
			return PLUGIN_HANDLED

	new entSpawn
	new Float:spawnAngle[3] 
	new Float:spawnOrigin[3]

	if( pev_valid(id) ){
		new teamnum = Pokemod_Team(id)
		if(teamnum!=0)
			entSpawn = find_free_spawn(teamnum, spawnOrigin, spawnAngle)
	}

	if( !entSpawn ){
		// Didn't find a free spawn spot. Quit... 
		if(Pokemod_Alive(id))
			poke_printlang(id,"POKEMOD_NOWAYOUT")
		return PLUGIN_CONTINUE 
	}

	new origina[3]
	FVecIVec(spawnOrigin, origina)

	if( num>0 ){
		get_user_origin(id, origin[id])
		MakeLine(0,origin[id],origina,SPRITE_INDEX[SPR_ROPE],0,0,20,7,0,139,59,19,255,0)
	}

	set_pev(id, pev_angles, spawnAngle)
	set_user_origin(id,origina)

	return PLUGIN_HANDLED
 }

 ///////////////////
 //  Gives items  //
 ///////////////////
 GiveItem(id, item, amount)
 {
	if( !ValidItem(item) )
		return 0;

	#if MAX_ITEM_TOTAL > 0
	if( TotalItems(id)+amount > MAX_ITEM_TOTAL){
		amount = MAX_ITEM_TOTAL - TotalItems(id);
		if( !amount )
			return 0
	}
	#endif
	#if MAX_ITEM_AMOUNT > 0
	if( PlayersItem[id][item]+amount > MAX_ITEM_AMOUNT){
		amount = MAX_ITEM_AMOUNT - PlayersItem[id][item];
		if( !amount )
			return 0
	}
	#endif
	if( ItemInfo[item]&II_MAX && PlayersItem[id][item]+amount > ItemMax[item] ){
		amount = ItemMax[item] - PlayersItem[id][item];
		if( !amount )
			return 0
	}

	PlayersItem[id][item] += amount

	if(ItemInfo[item] & II_MAXHP)
		CheckMaxHP(id)
	if(ItemInfo[item] & II_ARMOR)
		CheckMaxArmor(id)
	if(ItemInfo[item] & II_SPEED)
		SetSpeed(id)

	if( ItemEventForward > -1 )
	{
		new functionReturn
		ExecuteForward(ItemEventForward, functionReturn, id, IE_PICKUP, item)

		// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
		// and 0 is used by continue and invalid return.
		if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
		{
			return PLUGIN_HANDLED;
		}
	}

	poke_printlang(id,"POKEMOD_ITEMPICKUP", amount, ItemToName(item));
	poke_print(id, print_chat,"%s - %s", ItemToName(item), ItemDesc[item]);

	return amount;
 }

 ////////////////////
 //  Sell an item  //
 ////////////////////
 SellItem(id, item, num=1)
 {
	if( !ValidItem(item) ){
		poke_printlang(id,"POKEMOD_NOITEM")
		return
	}
	new itemname[NAME_LENGTH]
	ItemToName(item, itemname, NAME_LENGTH)
	if( num > HasPokeItem(id, item) ){
		poke_printlang(id,"POKEMOD_NOITEMNUM", num, itemname)
		return
	}
	new sold_for = floatround(abs(CostOfItem(item, true))*SELLFACTOR)*num

	Pokemod_Pay(id, sold_for)
	PlayersItem[id][item] -= num

	if(ItemInfo[item] & II_MAXHP){
		CheckMaxHP(id)
		if( Pokemod_Health(id) > PlayerStat[id][STAT_MAXHP] )
			Pokemod_Heal(id, 0)
	}
	if(ItemInfo[item] & II_ARMOR){
		CheckMaxArmor(id)
		if( Pokemod_Armor(id) > PlayerStat[id][STAT_MAXAP] )
			Pokemod_Repair(id, 0)
	}
	if(ItemInfo[item] & II_SPEED)
		SetSpeed(id)

	if( ItemEventForward > -1 )
	{
		new functionReturn
		ExecuteForward(ItemEventForward, functionReturn, id, IE_SELL, item)

		// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
		// and 0 is used by continue and invalid return.
		if(functionReturn==PLUGIN_HANDLED)	//if something was handled, stop
		{
			return;
		}
	}

	poke_printlang(id,"POKEMOD_SELLITEM", num, itemname, sold_for)
 }

 /////////////////////////////////
 //  Removes items from player  //
 /////////////////////////////////
 public NoItems(id)
 {
	if(g_Status[id][STATUS_BIKE]){
		RemoveStatus(id, STATUS_BIKE);
		SetSpeed(id);
	}
	for( new apple = 0; apple < ItemsCreated; apple++ )
		PlayersItem[id][apple] = 0;

	return PLUGIN_HANDLED;
 }

 CostOfItem(item, bool:is_item=true)
 {
	if(is_item)
		return ItemPrice[item]
	return ItemPrice[MAX_ITEMS+item]
 }
 CostOfPoke(poke)
	return CostOfItem(poke, false)

 public ReadItems(read_data)
 {
	new PokeFile[128];
	Poke_FileName( F_Items, PokeFile);

	new Data[256], i, charnum, size = file_size(PokeFile, 1);
	new Left[DESC_LENGTH], Right[151];
	new item, icat, is_poke;

	if( read_data ) debugMessage(_,2,"Config", "Begining to read item file");

	ItemCatCreated = 0;
	if( !read_data ) ItemsCreated = 0;

	for(i=0; i<size; i++){
		read_file(PokeFile,i,Data,255,charnum)

		if( !ValidData( Data ) )
			continue;

		if( equal(Data,DASH,1) ){
			if( !read_data )
				continue;
			else
				copy(Data, 255, Data[1]);
		}

		strbreak(Data, Left, NAME_LENGTH-1, Right, 150)
		remove_quotes(Left)

		if( equal(Data, LEFT_CBRACKET, 1) ){
			if(icat == MAX_ITYPE){
				debugMessage(_,_, "Config", "Max item categories reached. Cannot create category %s",Left)
				continue
			}

			icat = ItemCatCreated++
			replace(Left, NAME_LENGTH-1, LEFT_CBRACKET, EMPTY)
			replace(Left, NAME_LENGTH-1, RIGHT_CBRACKET, EMPTY)
			copy(ItemCatName[icat], NAME_LENGTH-1, Left)

			continue;
		}

		if( read_data ){
			is_poke = NameToPoke(Left, true)
			if( is_poke ){
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
				ItemPrice[ MAX_ITEMS + is_poke ] = str_to_num(Left)
				continue
			}
		}

		if(item == MAX_ITEMS){
			debugMessage(_,_, "Config", "Max items reached. Cannot create item %s",Left)
			continue
		}

		//okay it isn't a pokemon, lets save the item name
		copy( ItemName[item], NAME_LENGTH-1, Left)
		replace_all( ItemName[item], NAME_LENGTH-1, UNDERSCORE, SPACE)

		//now lets get the price
		strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
		ItemPrice[ item ] = str_to_num(Left)

		if( !read_data ){
			item++;
			ItemsCreated++;
			continue;
		}

		ItemCat[item] = icat;
		ItemInfo[item] = 0;
		formatex(ItemDesc[item], 100, LANG,0,"POKEMOD_NODESC");

		while( ValidData(Right) ){
			if( contain(Right, "\"") == 0 ){
				strbreak(Right, Left, DESC_LENGTH-1, Right, 150)
				remove_quotes(Left);
				copy(ItemDesc[item], 100, Left);
				break;
			}
			else{

				strbreak(Right, Left, DESC_LENGTH-1, Right, 150)
				remove_quotes(Left)
				ItemInfo[item] |= read_flags(Left)

				switch( Left[0] )
				{
					case 'a':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemHeal[item] = str_to_num(Left)
					}
					case 'b':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemCatch[item] = str_to_num(Left)
					}
					case 'l':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemPDamage[item] = str_to_num(Left)
					}
					case 'm':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemDamage[item] = str_to_num(Left)
					}
					case 'n':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemArmor[item] = str_to_num(Left)
					}
					case 'o':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemSpeed[item] = str_to_num(Left)
					}
					case 'p':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemMaxHP[item] = str_to_num(Left)
					}
					case 'r':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemFossil[item] = NameToPoke(Left)
					}
					case 't':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemItem[item] = NameToItem(Left,false)
					}
					case 'u':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemSpeed[item] = str_to_num(Left)
					}
					case 'y':{
						strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
						remove_quotes(Left)
						ItemMax[item] = str_to_num(Left)
					}
				}
			}
		}
		item++;
	}

	if( read_data ){
		if( RegisterItemForward > -1 ){
			new functionReturn;
			ExecuteForward(RegisterItemForward, functionReturn);
		}
		debugMessage(_,2,"Config", "%d Items loaded.", ItemsCreated);
	}
 }
 public TradeItem(id, item, nid, num)
 {
	if( !ValidItem(item) )
		return false;

	new iname[NAME_LENGTH];
	ItemToName(item, iname, NAME_LENGTH);

	if( num < 1 )
		num = 1;

	if(PlayersItem[id][item]>=num){
		new name[32], Nname[32];
		get_user_name(id, name, 31);
		get_user_name(nid, Nname, 31);

		#if MAX_ITEM_TOTAL > 0
		if( TotalItems(id)+num > MAX_ITEM_TOTAL){
			num = MAX_ITEM_TOTAL - TotalItems(id);
			if( !num ){
				poke_printlang(id,"POKEMOD_NOMOREITEMS", name);
				return false;
			}
		}
		#endif
		#if MAX_ITEM_AMOUNT > 0
		if( PlayersItem[id][item]+num > MAX_ITEM_AMOUNT){
			num = MAX_ITEM_AMOUNT - PlayersItem[id][item];
			if( !num ){
				poke_printlang(id,"POKEMOD_NOMOREITEM", name);
				return false;
			}
		}
		#endif
		if( ItemInfo[item]&II_MAX && PlayersItem[id][item]+num > ItemMax[item] ){
			num = ItemMax[item] - PlayersItem[id][item];
			if( !num ){
				poke_printlang(id,"POKEMOD_NOMOREITEM", name);
				return false;
			}
		}

		PlayersItem[nid][item] += num;
		PlayersItem[id][item] -= num;

		poke_printlang(nid,"POKEMOD_RECIEVEITEM", name, num, iname);
		poke_printlang(id,"POKEMOD_GAVEITEM", num, iname, Nname);
		return true;
	}
	else
		poke_printlang(id,"POKEMOD_NOITEMNUM", num, iname);
	return false;
 }
 HasSilphScope( id )
 {
	for( new i=0; i<ItemsCreated; i++)
		if( ItemInfo[i]&II_SCOPE && HasPokeItem( id, i ) )
			return i;
	return MAX_ITEMS;
 }
 TotalItems( id )
 {
	new total;
	for( new i=0; i<ItemsCreated; i++)
		total += PlayersItem[id][i];
	return total;
 }
 public Console_Pokemart(id)
 {
	new arg[32];
	read_argv(1,arg,31);
	new cat = NameToCat(arg);
	if( cat < 0 || cat >= ItemCatCreated ){
		if( ValidData(arg) )
			console_print(id, "%s %L", PREFIX, id,"POKEMOD_NOSHOP");
		cat = -1;
	}
	ShopMenu(id, OpenThisCat(id, cat) );
	return PLUGIN_HANDLED;
 }
 #if AUTO_ITEMS > 0
 AutoItems(id)
 {
	if( PokeItem(id) ){
		#if AUTO_ITEMS == 1
		if(is_user_bot(id)){
		#endif

		new hp = Health(id)

		for( new item = 0 ; item < ItemsCreated ; item++ ){

			if( !HasPokeItem(id,item) || ItemInfo[item] & II_DISABLE )
				continue
			if( ItemInfo[item] & II_HEAL ){
				if( ItemHeal[item] == -1 ){
					if( PlayerStat[id][STAT_MAXHP]-hp > PlayerStat[id][STAT_MAXHP]/2 ){
						UseItem(id, item)
						break
					}
				}
				else if( PlayerStat[id][STAT_MAXHP]-hp >= ItemHeal[item] ){
					hp += ItemHeal[item]
					UseItem(id, item)
				}
			}
		}

		#if AUTO_ITEMS == 1
		}
		#endif
	}
 }
 #endif

 OpenCat(id)
 {
	for( new i=0; i<MAX_ITYPE+3; i++ ){
		if( i == MAX_ITYPE+1 && !PokeItem(id) )
			continue;
		if( Open[id][i] )
			return i;
	}
	return -1;
 }
 OpenThisCat(id, cat)
 {
	for( new i=0; i<MAX_ITYPE+3; i++ )
		Open[id][i] = ( cat == i );
	return cat;
 }
