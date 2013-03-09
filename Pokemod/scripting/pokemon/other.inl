
 // in this file, there is just a whole bunch of functions that deals with pokemod a lot, but has no specific place

 /////////////////////////////////////
 //  Determines the pokemon's name  //
 /////////////////////////////////////
 PokeToName(const pid, name[]="", len=0)
 {
	static _name[NAME_LENGTH];
	if( !ValidPoke(pid) )
		copy(_name, NAME_LENGTH-1, POKENAMES[0]);
	else
		copy(_name, NAME_LENGTH-1, POKENAMES[pid]);

	if(len)	copy(name, len, _name);
	return _name;
 }
 ////////////////////////////////////////////
 //  Returns the number that a pokemon is  //
 ////////////////////////////////////////////
 NameToPoke(const pname[], bool:check_full=false, id=0)
 {
	static pid, pokename[NAME_LENGTH];
	copy( pokename, NAME_LENGTH-1, pname);

	replace_all(pokename, NAME_LENGTH-1, "_", EMPTY);
	replace_all(pokename, NAME_LENGTH-1, " ", EMPTY);
	pokename[0] = toupper(pokename[0]);

	if( check_full ){
		for( pid = 0; pid <= PokesCreated; pid++ )
			if( equali( pokename, POKENAMES[pid] ) )
				return pid;
	}
	else{
		static current_comp, largest[2], poke[2];
		largest[0] = largest[1] = poke[0] = poke[1] = 0;

		if( id ){
			for( pid = 0; pid < MAX_POKES; pid++ ){
				current_comp = NameCompare( pokename, NAME_LENGTH-1, POKENAMES[ PlayersPoke[id][pid] ] )
				if( current_comp >= largest[0] ){
					largest[0] = current_comp;
					poke[0] = PlayersPoke[id][pid];
					if( largest[0] > largest[1] ){
						largest[1] = largest[0];
						poke[1] = poke[0];
						poke[0] = 0;
					}
				}
			}
		}
		else{
			for( pid = 0; pid <= PokesCreated; pid++ ){
				current_comp = NameCompare( pokename, NAME_LENGTH-1, POKENAMES[pid] )
				if( current_comp >= largest[0] ){
					largest[0] = current_comp;
					poke[0] = pid;
					if( largest[0] > largest[1] ){
						largest[1] = largest[0];
						poke[1] = poke[0];
						poke[0] = 0;
					}
				}
			}
		}
		if( !poke[0] || largest[1] > largest[0] )
			return poke[1];
	}

	return 0;
 }

 PokeImageHtml( const poke )
 {
	new szhtml[128];
	if( ValidPoke( poke ) ){
		formatex( szhtml, 127, "<img src=\"http://www.serebii.net/pokedex-rs/icon/%03d.gif\">", poke );
	}
	return szhtml;
 }
 
 ///////////////////////////////////
 //  Determines the name of item  //
 ///////////////////////////////////
 ItemToName(item, name[]="", len=0)
 {
	static _name[NAME_LENGTH];
	if( ValidItem(item) )
		copy(_name , NAME_LENGTH-1 , ItemName[item]);
	else
		copy(_name , NAME_LENGTH-1 , "None");

	if(len)	copy(name, len, _name);
	return _name;
 }
 
 ///////////////////////////////////
 //  Determines the item by name  //
 ///////////////////////////////////
 NameToItem(const iname[], bool:check_full=true)
 {
	static i, itemname[NAME_LENGTH], tempname[NAME_LENGTH];
	copy( itemname, NAME_LENGTH-1, iname);
	replace_all(itemname, NAME_LENGTH-1, "_", EMPTY);
	replace_all(itemname, NAME_LENGTH-1, " ", EMPTY);

	static current_comp, largest[2], items[2];
	largest[0] = largest[1] = items[0] = items[1] = 0;

	for(i=0; i < ItemsCreated; i++){
		copy(tempname, NAME_LENGTH-1, ItemName[i]);
		replace_all(tempname, NAME_LENGTH-1, "_", EMPTY);
		replace_all(tempname, NAME_LENGTH-1, " ", EMPTY);

		if(check_full){
			if( equali(itemname, tempname) )
				return i;
		}
		else{
			current_comp = NameCompare( itemname, NAME_LENGTH-1, tempname );
			if( current_comp >= largest[0] ){
				largest[0] = current_comp;
				items[0] = i;
				if( largest[0] > largest[1] ){
					largest[1] = largest[0];
					items[1] = items[0];
					items[0] = 0;
				}
			}
		}
	}

	if( !check_full && (!items[0] || largest[1] > largest[0] ) )
		return items[1];
	return MAX_ITEMS;
 }
 
 ///////////////////////////////////
 //  Determines the item by name  //
 ///////////////////////////////////
 NameToSkill(const name[])
 {
	static skill, i, _name[NAME_LENGTH], tempname[NAME_LENGTH];
	copy( _name, NAME_LENGTH-1, name);
	skill = -1;

	replace_all(_name, NAME_LENGTH-1, "_", EMPTY);
	replace_all(_name, NAME_LENGTH-1, " ", EMPTY);

	for(i=0; i < SkillsCreated; i++){
		copy(tempname, NAME_LENGTH-1, SkillName[i]);
		replace_all(tempname, NAME_LENGTH-1, "_", EMPTY);
		replace_all(tempname, NAME_LENGTH-1, " ", EMPTY);

		if(equali(_name, tempname)){
			skill = i;
			break;
		}
	}

	return skill;
 }

 ////////////////////////////////////////////////////
 //  Determines the name of a skill without any _  //
 ////////////////////////////////////////////////////
 SkillToName(skill, name[]="", len=0, bool:remove_scores=true)
 {
	static _name[NAME_LENGTH];
	if(skill > -1 && skill < SkillsCreated)
		copy(_name , NAME_LENGTH-1 , SkillName[skill]);
	else
		copy(_name , NAME_LENGTH-1 , "None");

	if(remove_scores)	replace_all(_name, NAME_LENGTH-1, "_", " ");
	if(len)	copy(name, len, _name);
	return _name;
 }
 
 ///////////////////////////////////////
 //  Determines the category by name  //
 ///////////////////////////////////////
 NameToCat(catname[])
 {
	for( new i=0; i<MAX_ITYPE; i++ )
		if(equali(catname, ItemCatName[i]))
			return i;
	return MAX_ITYPE;
 }
 
 //--Things im using so i dont have to use actual natives each time--//
 Pokemod_Connected(id)
	return ValidPlayer(id) ? PlayerInfo[id][PL_CONNECTED] : 0;
 Pokemod_Connect(id,num)
 {
	PlayerInfo[id][PL_CONNECTED] = num;
	if( num ) skills_handler(id, SI_CONNECT);
	else skills_handler(id, SI_DISCONNECT);
	return num;
 }
 Pokemod_Alive(id, bool:update=false)
 {
	if( ValidPlayer(id) ){
		if( update ) PlayerInfo[id][PL_ALIVE] = is_user_alive(id);
		return PlayerInfo[id][PL_ALIVE];
	}
	else if( pev_valid(id) ){
		new Float:takedamage;
		pev(id, pev_takedamage, takedamage);
		return floatround( takedamage );
	}
	return 0;
 }
 Pokemod_FF(id, id2)
 {
	if( id == id2 ) return 2;
	else if( Pokemod_Team(id) == Pokemod_Team(id2) && !FFAon() ) return 1;
	return 0;
 }
 Pokemod_Team(id, bool:update=false)
 {
	if( id > MAX_PLAYERS )
		return pev(id, pev_team);

	if( !Pokemod_Connected(id) )
		return 0;

	if( update ){
	#if MOD == CS
		PlayerInfo[id][PL_TEAM] = _:cs_get_user_team(id);
	#else
		#if MOD == NS
			PlayerInfo[id][PL_TEAM] = pev(id, pev_team);
		#else
			PlayerInfo[id][PL_TEAM] = get_user_team(id);
		#endif
	#endif
	}

	return PlayerInfo[id][PL_TEAM];
 }
 Pokemod_Health(id)
 {
	if( ValidPlayer(id) ) return get_user_health(id);
	else if( pev_valid(id) ){
		new Float:health;
		pev(id, pev_health, health);
		return floatround( health );
	}
	return 0;
 }
 Pokemod_Heal(id, amm)
 {
	if( ValidPlayer(id) ){
		new old = Health(id);
		if( PlayerStat[id][STAT_MAXHP] <= 1 )
			CheckMaxHP(id);
		if( (PlayerStat[id][STAT_MAXHP] < old && PlayerStat[id][STAT_MAXHP] > 0) || !amm )
			return 0;
		set_user_health(id, clamp(old+amm, 1, abs( PlayerStat[id][STAT_MAXHP] ) ) );
		return 1;
	}
	else if( pev_valid(id) ){
		new Float:health, Float:max_health;
		pev(id, pev_health, health);
		pev(id, pev_max_health, max_health);
		set_pev(id, pev_health, floatmin( health+amm, max_health ) );
		return 1;
	}
	return 0;
 }
 Pokemod_Money(id)
 {
	if( !ValidPlayer(id) ) return 0;
	#if MOD==CS
		money[id] = cs_get_user_money(id);
	#endif
	return money[id];
 }
 Pokemod_Pay(id, amm, do_forward=1)
 {
	if( !ValidPlayer(id) ) return 0;

	if(do_forward && PokePayForward > -1)
	{
		new functionReturn
		ExecuteForward(PokePayForward, functionReturn, id, amm)

		// Forward will return the highest value, don't return 1 or 2 in function cause of return handled or handled_main
		// and 0 is used by continue and invalid return.
		if(functionReturn==PLUGIN_HANDLED)	//if something else did handle it, let's stop here.
		{
			return 1;
		}
	}
	#if MOD==CS
		new old = cs_get_user_money(id);
		new newmoney = clamp(old+amm, 0, MAX_MONEY);

		cs_set_user_money(id, newmoney , 1);
		money[id] = newmoney;
	#else
		money[id] = clamp(money[id]+amm, 0, MAX_MONEY);
	#endif
	return 1;
 }
 Pokemod_Armor(id)
 {
	if( ValidPlayer(id) ) return get_user_armor(id);
	else if( pev_valid(id) ){
		new Float:armorvalue;
		pev(id, pev_armorvalue, armorvalue);
		return floatround( armorvalue );
	}
	return 0;
 }
 Pokemod_Repair(id, amm)
 {
	if( !ValidPlayer(id) )
		return 0;

	new old = get_user_armor(id);
	new repair = clamp(old+amm, 0, abs(PlayerStat[id][STAT_MAXAP]) );

	for(new i=0; i < ItemsCreated; i++)
		if( ItemInfo[i] & II_ARMOR && HasPokeItem(id, i) )
			repair += ItemArmor[i]*PlayersItem[id][i];

	repair = clamp(repair, 0, abs(PlayerStat[id][STAT_MAXAP]) );

	if(repair < old)
		return 0;

	#if MOD==CS
		if( repair )
			cs_set_user_armor(id, repair, (repair >= 50) ? CS_ARMOR_VESTHELM : CS_ARMOR_KEVLAR );
		else
			cs_set_user_armor(id, repair, CS_ARMOR_NONE);
	#else
		set_user_armor(id, repair);
	#endif

	return repair-old;
 }

 //--thing so you can remove ent after time--//
 public poke_remove_entity(ent)
 {
	ent -= REMOVE_TASK;
	if( pev_valid(ent) && ent )
		fm_remove_entity( ent );
 }

public check_use(id,pos)
{
	if(id)
	{
		if(canuse[id] == false)
		{
			if(pos)
				client_print(id,print_chat,"You are not allowed to use the bank.")
			else
				console_print(id,"You are not allowed to use the bank.")
			return 0
		}
	}
	new cvarrounds = get_cvar_num("bank_offrounds")
	if(rounds <= cvarrounds)
	{
		if(pos)
			client_print(id,print_chat,"Sorry, the bank is disabled for the first %d rounds of the map.",cvarrounds)
		else
			console_print(id,"Sorry, the bank is disabled for the first %d rounds of the map.",cvarrounds)
		return 0
	}
	if(!get_cvar_num("bank_state"))
	{
		if(pos)
			client_print(id,print_chat,"Sorry, the bank is closed and no transactions are being processed.")
		else
			console_print(id,"Sorry, the bank is closed and no transactions are being processed.")
		return 0
	}
	new players = get_playersnum()
	new minplayers = get_cvar_num("bank_min_players")
	if(players < minplayers)
	{
		if(pos)
			client_print(id,print_chat,"There must be at least %d players connected to use the bank.",minplayers)
		else
			console_print(id,"There must be at least %d players connected to use the bank.",minplayers)
		return 0
	}
	return 1
}

public get_balance(id)
{
	new sid[35]
	new balance = -1
	sid=g_szIdentifier[id]
	//server_print("### SID: %s",sid)
	new key[51]
	formatex(key,50,"%s",sid)
	//server_print("### KEY: %s",key)
	if(vaultdata_exists(key))
	{
		new balancestr[21]
		get_vaultdata(key,balancestr,20)
		balance = str_to_num(balancestr)
	} else {
		//server_print("### COULD FIND FIND IT")
	}

	return balance
}

public set_balance(id,balance)
{
	new sid[35]
	sid=g_szIdentifier[id]

	new key[51]
	formatex(key,50,"%s",sid)
	if(vaultdata_exists(key))
	{
		new balancestr[21]
		num_to_str(balance,balancestr,20)
		set_vaultdata(key,balancestr)
		return 1
	}
	else
		return -1

	return -1	
}

public bank_menu(id)
{
	new client = 0
	if(read_argc() > 1)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new menubody[276], keys = 0,len;
	new bool:hasacc = true;
	len = format(menubody,275,"Bank Menu:\n");
	if(get_balance(id) == -1)
	{
		hasacc = false
		len += format(menubody[len],275-len,"1. Open a Bank Account\n")
		keys |= (1<<0|1<<9)		
	}
	else
		len += format(menubody[len],275-len,"1. Open a Bank Account\n")
	len += format(menubody[len],275-len,"2. Check your Balance\n3. Deposit Money\n4. Deposit All\n5. Withdraw Money\n6. Withdraw Maximum\n7. Bank Help\n8. Transfer Money\n\n")
	if(hasacc)
	{
		len += format(menubody[len],275-len,"0. Exit")
		keys |= (1<<1|1<<2|1<<3|1<<4|1<<5|1<<6|1<<7|1<<9)
	}
	else
		len += format(menubody[len],275-len,"0. Exit")
	show_menu(id,keys,menubody,-1,"Bank Menu:")
	return PLUGIN_CONTINUE
}

public bank_menu_cmd(id,key)
{
	switch(key)
	{
		case 0: client_cmd(id,"bank_create 1")
		case 1: client_cmd(id,"bank_amount 1")
		case 2:
		{
			sayspecial[id] = 1
			client_print(id,print_chat,"Please enter the amount you want to deposit in chat:")
		}			
		case 3: client_cmd(id,"maxdep")
		case 4:
		{	
			sayspecial[id] = 2
			client_print(id,print_chat,"Please enter the amount you want to withdraw in chat:")
		}
		case 5: client_cmd(id,"maxwit")
		case 6:	client_cmd(id,"bank_help")
		case 7:
		{
			sayspecial[id] = 3
			client_print(id,print_chat,"Please enter the person you want to transfer to and the amount you want to transfer in chat:")
		}
	}
	return PLUGIN_HANDLED
}

public bank_givemoney(id,level,cid)
{
	if(!cmd_access(id,level,cid,3))
		return PLUGIN_HANDLED
	new target[32], tid
	read_argv(1,target,31)
	tid = cmd_target(id,target,2)
	if(!tid)
		return PLUGIN_HANDLED
	new amountstr[10], amount
	read_argv(2,amountstr,9)
	amount = str_to_num(amountstr)
	new totam = amount
	new curmoney = cs_get_user_money(tid)
	new newtotal = curmoney + amount
	if(newtotal > 16000)
	{		
		cs_set_user_money(tid,16000)
		amount = newtotal - 16000
	}
	else
	{
		cs_set_user_money(tid,newtotal)
		amount = 0
	}
	if(amount > 0)
	{	
		new balance = get_balance(tid)
		if(balance != -1)
			set_balance(id,balance + amount)
	}
	new name[32], tname[32]
	get_user_name(id,name,31)
	get_user_name(tid,tname,31)
	if(read_argc() == 4)
		client_print(id,print_chat,"You gave %s $%d.",tname,totam)
	else
		console_print(id,"You gave %s $%d.",tname,totam)
	client_print(tid,print_chat,"%s gave you $%d, $%d of which went into your bank account.",name,totam,amount)
	return PLUGIN_HANDLED
}	

public bank_transfer(id)
{
	new client = 0
	if(read_argc() > 3)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new target[32]
	read_argv(1,target,31)
	new tgt = cmd_target(id,target,8)
	if(!tgt)
		return PLUGIN_HANDLED
	if(id == tgt)
	{
		if(client)
			client_print(id,print_chat,"You may not transfer money to yourself.")
		else
			console_print(id,"You may not transfer money to yourself.")
		return PLUGIN_HANDLED
	}		
	new tamounts[9],tamount
	read_argv(2,tamounts,8)
	tamount = str_to_num(tamounts)
	if(tamount <= 0) return PLUGIN_HANDLED
	new balance = get_balance(id)
	if(balance == -1)
	{
		if(client)
			client_print(id,print_chat,"You do not have a bank account to transfer money from.")
		else
			console_print(id,"You do not have a bank account to transfer money from.")
		return PLUGIN_HANDLED
	}
	new tbalance = get_balance(tgt)
	new name[32], tname[32]
	get_user_name(tgt,tname,31)
	get_user_name(id,name,31)	
	if(tbalance == -1)
	{
		if(client)
			client_print(id,print_chat,"%s does not have a bank account to transfer money to.",tname)
		else
			console_print(id,"%s does not have a bank account to transfer money to.",tname)
		client_print(tgt,print_chat,"%s tried to transfer money to your account but you don't have a bank account!",name)
		return PLUGIN_HANDLED
	}	
	balance -= tamount
	balance -= bankfees
	if(balance < 0)
	{
		if(client)
			client_print(id,print_chat,"You do not have enough money in your bank account.")
		else
			console_print(id,"You do not have enough money in your bank account.")
		return PLUGIN_HANDLED
	}
	tbalance += tamount
	if(bankfees > 0)
	{
		if(client)
			client_print(id,print_chat,"You paid $%d in bank fees.",bankfees)
		else
			console_print(id,"You paid $%d in bank fees.",bankfees)
	}		
	set_balance(id,balance)
	set_balance(tgt,tbalance)
	if(client)
		client_print(id,print_chat,"You have transferred $%d to %s's bank account. You now have $%d in your account.",tamount,tname,balance)
	else
		console_print(id,"You have transferred $%d to %s's bank account. You now have $%d in your account.",tamount,tname,balance)
	client_print(tgt,print_chat,"%s has transferred $%d to your bank account. You now have $%d in your account.",name,tamount,tbalance)
	return PLUGIN_HANDLED
}

public hookmoney()
{
	if(!get_cvar_num("bank_state"))
		return PLUGIN_CONTINUE
	new curmoney = read_data(1)
	if(curmoney < 16000)
		return PLUGIN_CONTINUE
	new id
	for(new inum=0;inum<=32;inum++)
	{
		if(!is_user_connected(inum)) continue
		new rmoney = cs_get_user_money(inum)
		if(rmoney == curmoney)
		{
			id = inum
			break;
		}
	}
	if(canuse[id] == false)
		return PLUGIN_CONTINUE
	new cvarrounds = get_cvar_num("bank_offrounds")
	if(rounds <= cvarrounds)
		return PLUGIN_CONTINUE
	if(get_playersnum() >= get_cvar_num("bank_min_players"))
	{
		new balance = get_balance(id)
		if(balance == -1)
			return PLUGIN_CONTINUE
		balance += 10000
		set_balance(id,balance)
		cs_set_user_money(id,curmoney-10000)
		client_print(id,print_chat,"$10000 has been automatically deposited in your bank account. You now have $%d in your account.",balance)
	}
	return PLUGIN_CONTINUE
}

public bank_spam()
{
	new cvarval = get_cvar_num("bank_state")
	if(cvarval)
	{
		new message[256]
		get_cvar_string("bank_msg",message,255)
		client_print(0,print_chat,message)
	}
	set_task(float(get_cvar_num("bank_msg_interval")),"bank_spam")
}

public bank_help(id)
{
	show_motd(id,HELPPAGE,"AMX Bank Help")
}

public say_cheese(id)
{
	new said[191]
	read_args(said,190)
	remove_quotes(said)
	if(sayspecial[id])
	{
		switch(sayspecial[id])
		{
			case 1: client_cmd(id,"bank_deposit %s 1",said)
			case 2: client_cmd(id,"bank_withdraw %s 1",said)
			case 3: client_cmd(id,"bank_transfer %s 1",said)
		}
		sayspecial[id] = 0
		return PLUGIN_HANDLED
	}				
	if(said[0] == 'm')
	{
		if(equali(said,"maxwit"))
		{
			withdrawl_maximum(id)
			return PLUGIN_HANDLED
		}
		if(equali(said,"maxdep"))
		{
			deposit_maximum(id)
			return PLUGIN_HANDLED
		}
	}
	else if(said[0] == 'b')
	{
		if(containi(said,"bank_") != -1)
		{
			if(equali(said,"bank_amount"))
			{
				client_cmd(id,"bank_amount 1")
				return PLUGIN_HANDLED
			}
			if(containi(said,"bank_withdraw") != -1)
			{
				replace(said,190,"bank_withdraw","")
				client_cmd(id,"bank_withdraw %s 1",said)
				return PLUGIN_HANDLED
			}
			if(containi(said,"bank_deposit") != -1)
			{
				replace(said,190,"bank_deposit","")
				client_cmd(id,"bank_deposit %s 1",said)
				return PLUGIN_HANDLED
			}
			if(containi(said,"bank_transfer") != -1)
			{
				replace(said,190,"bank_transfer","")
				new target[51],amountstr[51]
				parse(said,target,50,amountstr,50)
				client_cmd(id,"bank_transfer %s %s 1",target,amountstr)
				return PLUGIN_HANDLED
			}
			if(containi(said,"bank_givemoney") != -1)
			{
				replace(said,190,"bank_givemoney","")
				new target[51],amountstr[51]
				parse(said,target,50,amountstr,50)
				client_cmd(id,"bank_givemoney %s %s 1",target,amountstr)
				return PLUGIN_HANDLED
			}
			if(equali(said,"bank_create"))
			{
				client_cmd(id,"bank_create 1")
				return PLUGIN_HANDLED
			}			
			if(equali(said,"bank_help"))
			{
				bank_help(id)
				return PLUGIN_HANDLED
			}
			if(equali(said,"bank_open"))
			{
				client_cmd(id,"bank_open 1")
				return PLUGIN_HANDLED
			}
			if(equali(said,"bank_close"))
			{
				client_cmd(id,"bank_close 1")
				return PLUGIN_HANDLED
			}					
			if(equali(said,"bank_menu"))
			{
				client_cmd(id,"bank_menu")
				return PLUGIN_HANDLED
			}
		}
	}
	return PLUGIN_CONTINUE
}

public giveinterest()
{
	rounds++
	if(!check_use(0,1)) return PLUGIN_CONTINUE
	bankfees = get_cvar_num("bank_fees_base")
	new Float:rate = get_cvar_float("bank_interest_rate")
	new irounds = get_cvar_num("bank_interest_rounds")
	if(!get_cvar_num("bank_state"))
		return PLUGIN_CONTINUE
	for(new i = 1;i<=32;i++)
	{
		if(is_user_connected(i))
		{
			if(canuse[i])
			{
				if(bPlayerInTeam(i)) {
					interest[i]++
					if(interest[i] >= irounds)
					{
						interest[i] = 0
						new balance = get_balance(i)
						if(balance != -1)
						{
							new Float:give = floatmul(rate,float(balance))
							new givint = floatround(give)
							if(givint > 0)
							{
								new allowed = 16000 - cs_get_user_money(i)
								if(givint <= allowed)
								{
									cs_set_user_money(i,cs_get_user_money(i)+givint)
									client_print(i,print_chat,"You were given $%d in interest.",givint)
								}
								else
								{
									new dep = givint - allowed
									client_print(i,print_chat,"You were given $%d in interest $%d of which went into your account.",givint,dep)
									cs_set_user_money(i,16000)
									balance += dep
									set_balance(i,balance)
								}
							}
						}
					}
				}
			}
		}
	}
	return PLUGIN_CONTINUE
}
 
 
public deposit_maximum(id)	
{
	if(!check_use(id,1)) return PLUGIN_HANDLED	
	new curmoney = cs_get_user_money(id)
	new balance = get_balance(id)
	if(balance == -1)
	{
		client_print(id,print_chat,"You do not have a bank account.")
		return PLUGIN_HANDLED	
	}
	balance += curmoney
	set_balance(id,balance)
	cs_set_user_money(id,0)
	client_print(id,print_chat,"You have deposited $%d in your bank account. You now have $%d in your account.",curmoney,balance)
	return PLUGIN_HANDLED
}

public withdrawl_maximum(id)
{
	if(!check_use(id,1)) return PLUGIN_HANDLED
	new balance = get_balance(id)
	if(balance == -1)
	{
		client_print(id,print_chat,"You do not have a bank account.")
		return PLUGIN_HANDLED
	}
	new curmoney = cs_get_user_money(id)
	new maxmoney = 16000 - cs_get_user_money(id)
	if(maxmoney > balance)
		maxmoney = balance
	balance -= maxmoney
	cs_set_user_money(id,curmoney + maxmoney,1)
	if((balance - bankfees) > 0)
		balance -= bankfees
	else
		cs_set_user_money(id,cs_get_user_money(id) - bankfees)
	if(bankfees > 0)
		client_print(id,print_chat,"You paid $%d in bank fees.",bankfees)
	bankfees += get_cvar_num("bank_fees_increase")		
	set_balance(id,balance)
	client_print(id,print_chat,"You have withdrawn $%d from your bank account. You now have $%d in your account.",maxmoney,balance)
	return PLUGIN_HANDLED
}
	
public bank_amount(id)
{
	new client = 0
	if(read_argc() > 1)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new balance = get_balance(id)
	if(balance == -1)
	{
		if(client)
			client_print(id,print_chat,"You do not have a bank account.")
		else
			console_print(id,"You do not have a bank account.")
		return PLUGIN_HANDLED		
	}
	else
	{
		if(client)
			client_print(id,print_chat,"You have $%d in your bank account.",balance)
		else
			console_print(id,"You have $%d in your bank account.",balance)
	}
	return PLUGIN_HANDLED
}

public bank_open(id,level,cid)
{
	if(!cmd_access(id,level,cid,1))
		return PLUGIN_HANDLED
	new client = 0
	if(read_argc() > 1)
		client = 1
	if(get_cvar_num("bank_state"))
	{
		if(client)
			client_print(id,print_chat,"The AMX bank is already open.")
		else
			console_print(id,"The AMX bank is already open.")
	}
	else
	{
		console_cmd(id,"amx_cvar bank_state 1")
		if(get_cvar_num("bank_state"))
		{
			if(client)
				client_print(id,print_chat,"The bank is now open.")
			else
				console_print(id,"The bank is now open.")
			client_print(0,print_chat,"The bank is now open for business.")
		}		
		else
		{
			if(client)
				client_print(id,print_chat,"You may not open the bank.")
			else
				console_print(id,"You may not open the bank.")
		}	
	}
	return PLUGIN_HANDLED
}

public bank_close(id,level,cid)
{	
	if(!cmd_access(id,level,cid,1))
		return PLUGIN_HANDLED
	new client = 0 
	if(read_argc() > 1)
		client = 1
	if(!get_cvar_num("bank_state"))
	{
		if(client)
			client_print(id,print_chat,"The AMX bank is already closed.")
		else
			console_print(id,"The AMX bank is already closed.")
	}
	else
	{
		console_cmd(id,"amx_cvar bank_state 0")
		if(!get_cvar_num("bank_state"))
		{
			if(client)
				client_print(id,print_chat,"The bank is now closed.")
			else
				console_print(id,"The bank is now closed.")
			client_print(0,print_chat,"The bank is now closed.")
		}		
		else
		{
			if(client)
				client_print(id,print_chat,"You may not close the bank.")
			else
				console_print(id,"You may not close the bank.")
		}	
	}
	return PLUGIN_HANDLED
}

public bank_create(id)
{
	new client = 0
	if(read_argc() > 1)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new curmoney,neededmoney, amount
	neededmoney = get_cvar_num("bank_default_opening")
	curmoney = cs_get_user_money(id)
	if(curmoney >= neededmoney)
	{
		amount = neededmoney
		curmoney -= neededmoney
	}
	else
	{
		amount = curmoney
		curmoney = 0
	}

	new sid[35],key[51]
	sid=g_szIdentifier[id]
	
	if(!IsValidSteamID(sid) && (containi(g_szIdentifier[id],"FORUM:") == -1)) {
		client_print(id,print_chat,"This function for IP users is temporary disabled.")
		return PLUGIN_HANDLED
	}

	format(key,50,"%s",sid)
	if(vaultdata_exists(key))
	{
		if(client)
			client_print(id,print_chat,"You already have a bank account!")
		else
			console_print(id,"You already have a bank account!")
		return PLUGIN_HANDLED
	}
	new saveamstr[21]
	num_to_str(amount,saveamstr,20)
	set_vaultdata(key,saveamstr)
		
	cs_set_user_money(id,curmoney)
	if(client)
		client_print(id,print_chat,"Bank account created successfully. Your account has $%d in it.",amount)
	else
		console_print(id,"Bank account created successfully. Your account has $%d in it.",amount)
	return PLUGIN_HANDLED
}

public bank_withdrawl(id)
{
	new client = 0
	if(read_argc() > 2)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new balance = get_balance(id)
	if(balance == -1)
	{
		if(client)
			client_print(id,print_chat,"You do not have a bank account.")
		else
			console_print(id,"You do not have a bank account.")
		return PLUGIN_HANDLED		
	}
	new ams[9],amn,maxam	
	read_args(ams,8)
	amn = str_to_num(ams)
	if(amn <= 0) return PLUGIN_HANDLED	
	maxam = 16000 - cs_get_user_money(id)
	if(amn > maxam)
		amn = maxam
	if(amn > balance)
	{
		if(client)
			client_print(id,print_chat,"There is not enough money in your bank account.")
		else
			console_print(id,"There is not enough money in your bank account.")
		return PLUGIN_HANDLED
	}
	balance -= amn
	cs_set_user_money(id,cs_get_user_money(id) + amn)
	if(balance >= bankfees)
		balance -= bankfees
	else
		cs_set_user_money(id,cs_get_user_money(id) - bankfees)	
	set_balance(id,balance)
	if(bankfees > 0)
	{
		if(client)
			client_print(id,print_chat,"You paid $%d in bank fees.",bankfees)
		else
			console_print(id,"You paid $%d in bank fees.",bankfees)
	}
	bankfees += get_cvar_num("bank_fees_increase")
	if(client)
		client_print(id,print_chat,"You have withdrawn $%d from your bank account. You now have $%d in your account.",amn,balance)
	else
		console_print(id,"You have withdrawn $%d from your bank account. You now have $%d in your account.",amn,balance)
	return PLUGIN_HANDLED
}

public bank_deposit(id)
{
	new client = 0
	if(read_argc() > 2)
		client = 1
	if(!check_use(id,client)) return PLUGIN_HANDLED
	new damounts[9],damount,curmoney
	read_args(damounts,8)
	damount = str_to_num(damounts)
	if(damount <= 0) return PLUGIN_HANDLED
	curmoney = cs_get_user_money(id)
	if(damount > curmoney)
	{
		if(client)
			client_print(id,print_chat,"You don't have that much money.")
		else
			console_print(id,"You don't have that much money.")
		return PLUGIN_HANDLED
	}
	new balance = get_balance(id)
	if(balance == -1)
	{
		if(client)
			client_print(id,print_chat,"You do not have a bank account.")
		else
			console_print(id,"You do not have a bank account.")
		return PLUGIN_HANDLED
	}
	balance += damount
	set_balance(id,balance)
	cs_set_user_money(id,curmoney - damount)
	if(client)
		client_print(id,print_chat,"You have deposited $%d in your bank account. You now have $%d in your account.",damount,balance)
	else
		console_print(id,"You have deposited $%d in your bank account. You now have $%d in your account.",damount,balance)
	return PLUGIN_HANDLED
}
 
 public DelayedSetSpeed(id)
	SetSpeed(id)
 //--Sets Speed Correctly--//
 SetSpeed(id, bool:skillcall=false)
 {
	if( !PMon() || !ValidPlayer(id) ) return

	//thanks to a plugin i wrote and used
	static Float:speed, Float:toadd, clip, ammo, weapon, i;
	speed=0.0;
	toadd=0.0;
	weapon = get_user_weapon(id, clip, ammo);
	switch(weapon)
	{
		#if MOD==CS
			case	CSW_P228,CSW_HEGRENADE,CSW_C4,CSW_MAC10,CSW_SMOKEGRENADE,
					CSW_ELITE,CSW_FIVESEVEN,CSW_UMP45,CSW_USP,CSW_GLOCK18,CSW_MP5NAVY,
					CSW_TMP,CSW_FLASHBANG,CSW_DEAGLE,CSW_KNIFE: speed = 250.0
			case	CSW_SCOUT: speed = 260.0
			case	CSW_XM1014,CSW_AUG,CSW_GALI,CSW_FAMAS: speed = 240.0
			case	CSW_SG550,CSW_AWP,CSW_G3SG1: speed = 210.0
			case	CSW_M249: speed = 220.0
			case	CSW_M3,CSW_M4A1: speed = 230.0
			case	CSW_SG552: speed = 235.0
			case	CSW_AK47: speed = 221.0
			case	CSW_P90: speed = 245.0

			default: speed = 240.0	//if for some reason it aint here, this is the average speed
		#endif
		#if MOD==DOD
			default: speed = 100.0 //all weapons in dod have 600, but were setting stamina, so lets set to 100.0 (default stamina)
		#endif
		#if MOD==NS
			default:{
				speed = float(ns_get_maxspeed(id))
				ns_set_speedchange(id)
				toadd = float(ns_get_speedchange(id))
			}
		#endif
	}

	PlayerStat[id][STAT_SPEED] = 0;

	if(PM_disabled == PM_ON){
		if( skillcall )
			skills_handler(id, SI_SPEED|SI_SKILLCALL);
		else
			skills_handler(id, SI_SPEED);
	}

	toadd += PlayerStat[id][STAT_SPEED];

	//Items that affect speed
	for( i=0; i < ItemsCreated; i++){
		if( !HasPokeItem(id, i) )
			continue
		if( ItemInfo[i] & II_BIKE && g_Status[id][STATUS_BIKE])
			toadd += ItemSpeed[i]*PlayersItem[id][i]
		else if( ItemInfo[i] & II_SPEED )
			toadd += ItemSpeed[i]*PlayersItem[id][i]
	}

	//Statuses that affect speed
	if( g_Status[id][STATUS_SLOW] ){
		toadd -= SPEED_SLOW
		debugMessage(id,6,"Player", "speed reduced by %0.1f for being slowed",SPEED_SLOW)
	}
	if( g_Status[id][STATUS_FROZEN] ){
		toadd -= SPEED_FROZEN
		debugMessage(id,6,"Player", "speed reduced by %0.1f for being frozen",SPEED_FROZEN)
	}
	if(FreezeTime || g_Status[id][STATUS_PARALYZE] || g_Status[id][STATUS_ASLEEP] || g_Status[id][STATUS_OAK_STOP]){
		#if MOD==CS || MOD==DOD
			speed = 1.0
			toadd = 0.0
		#endif
		#if MOD==NS
			toadd = -1.0*MAX_SPEED
			speed = 1.0
		#endif
	}
	if( toadd > MAX_SPEED )
		toadd = MAX_SPEED

	if(Pokemod_Alive(id)){

	#if MOD==CS
		set_user_maxspeed(id, speed+toadd)
	#endif
	#if MOD==DOD
		dod_set_stamina(id, STAMINA_SET, clamp( floatround(toadd), 0, 99 ), clamp( floatround(speed+toadd), 1, 100 ))
	#endif
	#if MOD==NS
		ns_set_speedchange(id, floatround(toadd))
		set_user_maxspeed(id, speed)
	#endif

		debugMessage(id,8,"Player", "speed set to %0.1f with additional %0.1f",speed,toadd)
	}
 }
 
 register_pokeadmincmd(const cmd[],const function[],flags=-1, info[]="")
 {
	new final_cmd[51];
	#if PM_ADMIN_COMMANDS==1
	formatex(final_cmd, 50, "pm_%s",cmd);
	#else
	formatex(final_cmd, 50, "amx_%s",cmd);
	#endif
	return register_concmd(final_cmd,function,flags,info);
 }

 register_pokecvar(const cvar_num,const string[],flags = 0,Float:fvalue = 0.0)
	return pcvars[cvar_num] = register_cvar(get_pokecvar_name(cvar_num),string,flags,fvalue);
 get_pokecvar_name(const cvar_num, bool:prefix = true)
 {
	new cvar[51];
	if(prefix){
		#if PM_CVAR_PREFIX==1
		formatex(cvar, 50, "pm_%s",cvars[cvar_num]);
		#else
		formatex(cvar, 50, "amx_%s",cvars[cvar_num]);
		#endif
	}
	else
		copy(cvar, 50, cvars[cvar_num]);
	return cvar;
 }
 get_pokecvar(const cvar_name[], check_both = false)
 {
	for(new i=0; i<MAX_CVARS; i++)
		if(	( equal(get_pokecvar_name(i), cvar_name) )
		||	( check_both && equal(get_pokecvar_name(i, false), cvar_name) ) )
			return i;
	return MAX_CVARS;
 }
 #define get_ppcvar_flags(%1)			get_pcvar_flags(pcvars[%1])
 #define get_ppcvar_num(%1)				get_pcvar_num(pcvars[%1])
 #define get_ppcvar_float(%1)			get_pcvar_float(pcvars[%1])
 #define get_ppcvar_string(%1,%2,%3)	get_pcvar_string(pcvars[%1],%2,%3)
 #define set_ppcvar_flags(%1,%2)		set_pcvar_flags(pcvars[%1],%2)
 #define set_ppcvar_num(%1,%2)			set_pcvar_num(pcvars[%1],%2)
 #define set_ppcvar_float(%1,%2)		set_pcvar_float(pcvars[%1],%2)
 #if AMXX_VERSION_NUM < 180
	#define set_ppcvar_string(%1,%2)	set_cvar_string(get_pokecvar_name(%1),%2)
 #else
	#define set_ppcvar_string(%1,%2)	set_pcvar_string(pcvars[%1],%2)
 #endif

 get_ppcvar_line(const cvar_num)
 {
	new PokeFile[128];
	Poke_FileName( F_ModConfig, PokeFile);
	new Left[NAME_LENGTH], Right[151];
	new i, charnum, size = file_size(PokeFile, 1);

	for(i=0; i<=size; i++){
		read_file(PokeFile,i,Right,150,charnum)
		strbreak(Right, Left, NAME_LENGTH-1, Right, 1)

		if( equal(Left, get_pokecvar_name(cvar_num) ) )
			return i;
	}

	return -1;
 }
 set_ppcvar_line(const cvar_num, const new_cvar[])
 {
	new PokeFile[128];
	Poke_FileName( F_ModConfig, PokeFile);
	new writeme[256], iLen;
	iLen += formatex(writeme[iLen], (255-iLen), "%-20s ", get_pokecvar_name(cvar_num));
	iLen += formatex(writeme[iLen], (255-iLen), "%-15s //", new_cvar);
	iLen += formatex(writeme[iLen], (255-iLen), ppcvar_description(cvar_num) );
	write_file(PokeFile, writeme, get_ppcvar_line(cvar_num));
 }
 ppcvar_description(const cvar_num)
 {
	new data[256];
	switch(cvar_num)
	{
		case pm_pokemod:			copy(data, 255, "turns pokemod on and off (1=on, 0=off)");
		case pm_admin:				copy(data, 255, "what accessflag admins need to mess with PokeMod");
		case pm_save:				copy(data, 255, "save players xp (1=on, 0=off)");
		case pm_xpsavedays:			copy(data, 255, "how many days to save xp for before erasing it");
		case pm_safari_mode:		copy(data, 255, "turns safari mode on and off (1=on, 0=off)");
		case pm_debug:				copy(data, 255, "turns pokemod debug messages on and off (1=on, 0=off)");
		case pm_debug_key:			copy(data, 255, "only shows debug messages with this in it");
#if ALLOW_WILD==1
		case pm_wild:				copy(data, 255, "how many people needed to be wild");
		case pm_wild_level:			copy(data, 255, "max level that wild pokemon will be (set to 0 to disable)");
#endif
		case pm_maxhp:				copy(data, 255, "max hp that someone can get with bonuses");
		case pm_maxarmor:			copy(data, 255, "max armor that someone can get with bonuses");
		case pm_botsays:			copy(data, 255, "turns on bot radio commands when they choose/buy a pokemon/item");
		case pm_pokecenter:			copy(data, 255, "set to 0 if you dont want the npcs to be spawned. otherwise this is the distance you can be from pokecenter to use it");
		case pm_oak:				copy(data, 255, "prof oak properties (a = pokemart, b = heal, c = team specific)");
		case pm_oakhp:				copy(data, 255, "how much hp oak has (set to 0 for godmode)");
		case pm_pokeballs:			copy(data, 255, "spawn pokeballs with pokemon in them at spawns (1=yes, 0=no)");
		case pm_buytime:			copy(data, 255, "minutes that you can buy things from pokemart, set to 0 for infinite time");
		case pm_itemtime:			copy(data, 255, "items disappear after this time, set to 0 to remove at end of round");
		case pm_pokemart_open:		copy(data, 255, "allow buying from pokemart");
		case pm_ffa:				copy(data, 255, "make pokemod work with free for all");

		case pm_burntime:			copy(data, 255, "how long until players are no longer on fire");
		case pm_burndamage:			copy(data, 255, "how much damage per half second burn does");
		case pm_waterheal:			copy(data, 255, "how much hp is healed per second in water (if using a water pokemon)");
		case pm_poisontime:			copy(data, 255, "how long until players are no longer poisoned");
		case pm_poisondamage:		copy(data, 255, "how much damage per second poison does");
		case pm_sleeptime:			copy(data, 255, "players will fall asleep for this amount of time");
#if SAVE==SAVE_MYSQL
		case pm_mysql_host:			copy(data, 255, "domain or ip for the mysql server");
		case pm_mysql_user:			copy(data, 255, "user login name for the mysql server");
		case pm_mysql_pass:			copy(data, 255, "password for the user login for the mysql server");
		case pm_mysql_db:			copy(data, 255, "database to save pokemod data in the mysql server");
		case pm_mysql_persistent:	copy(data, 255, "only connect to mysql server once (if saving problems, set to 0)");
#endif
	}

	return data;
 }
 SkillDescription( skill, id )
 {
	new data[256], len;

	if( skill == -1 || !SkillInfo[skill] || SkillInfo[skill] & SI_DISABLE )
		formatex( data, 255, LANG, id, "POKEMOD_SKILLDISABLED" );
	else {
		len += copy( data[len], 255-len, SkillDesc[skill] );

		new const SPACELANG[] = " %L";
		if( SkillInfo[skill] & SI_ATTACK ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLATTACK" );
		}
		if( SkillInfo[skill] & SI_BIND ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLBIND" );
		}
		if( SkillInfo[skill] & SI_TOUCH ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLTOUCH" );
		}
		if( SkillInfo[skill] & SI_DAMAGED ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLDAMAGED" );
		}
		if( SkillInfo[skill] & SI_LOOP ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLSECOND" );
		}
		if( SkillInfo[skill] & SI_SPAWN ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLSPAWN" );
		}
		if( SkillInfo[skill] & SI_DEAD ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLDEATH" );
		}
		if( SkillInfo[skill] & SI_PATTACK ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLPATTACK" );
		}
		if( SkillInfo[skill] & SI_PDAMAGED ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLPDAMAGED" );
		}
		if( SkillInfo[skill] & SI_LOOK ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLLOOK" );
		}
		if( SkillInfo[skill] & SI_SPEED ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLSPEED" );
		}
		if( SkillInfo[skill] & SI_STATUS ){
			len += formatex( data[len], 255-len, SPACELANG, id, "POKEMOD_SKILLSTATUS" );
		}
		if( SkillInfo[skill] & SI_ADMIN )
			len += formatex( data[len], 255-len, " (%L)", id, "POKEMOD_ADMINONLY" );
	}

	return data;
 }
 
 SetAction( id, action=AC_NONE )
 {
	debugMessage(id,6,"Player", "set menu action=%s, was %s", ActionToName( action ), ActionToName( Action[id] ) )
	Action[id] = action;
 }
 ActionToName( action, name[]="", len=0 )
 {
	static _name[NAME_LENGTH];
	switch( action )
	{
		case AC_START:				copy( _name, NAME_LENGTH-1, "START");
		case AC_MAIN:				copy( _name, NAME_LENGTH-1, "MAIN");
		case AC_MART:				copy( _name, NAME_LENGTH-1, "MART");
		case AC_GO:					copy( _name, NAME_LENGTH-1, "GO");
		case AC_RELEASE:			copy( _name, NAME_LENGTH-1, "RELEASE");
		case AC_PC_GIVE:			copy( _name, NAME_LENGTH-1, "PC_GIVE");
		case AC_PC_TAKE:			copy( _name, NAME_LENGTH-1, "PC_TAKE");
		case AC_ITEM:				copy( _name, NAME_LENGTH-1, "ITEM");
		case AC_HELP:				copy( _name, NAME_LENGTH-1, "HELP");
		case AC_SKILL:				copy( _name, NAME_LENGTH-1, "SKILL");
		case AC_GIVE_ITEM:			copy( _name, NAME_LENGTH-1, "GIVE_ITEM");
		case AC_SAFARI:				copy( _name, NAME_LENGTH-1, "SAFARI");
		case AC_PC:					copy( _name, NAME_LENGTH-1, "PC");
		case AC_SELECT:				copy( _name, NAME_LENGTH-1, "SELECT");
		default:					copy( _name, NAME_LENGTH-1, "NONE");
	}
	if(len)	copy(name, len, _name);
	return _name;
 }

 NameCompare( const name[], const len, const name2[], const bool:case_sens=false )
 {
	new i;
	while( i < len ){
		if( name2[i] == '\0' )
			return i+1;
		if( name[i] == '\0' )
			return i;
		if( case_sens ?
			( name[i] != name2[i] ) :
			( tolower(name[i]) != tolower(name2[i]) )
		)
			return i;
		i++;
	}
	return len;
 }
 
 ActionPrint( id, print_type, apples, verb[] )
 {
	if( !apples )
		return false;
	if( apples == 1 )
		poke_print(id, print_type, "You %s someone", verb);
	else
		poke_print(id, print_type, "You %s %d people", verb, apples);
	return true;
 }
 
 CheckerNumber( CHECK_POKES:checker, bool:max )
 {
	switch( checker )
	{
		case CHECK_ACTIVE:	return (max) ? MAX_ACTIVE_POKES : 0 ;
		case CHECK_PC:		return (max) ? MAX_POKES : MAX_ACTIVE_POKES ;
		case CHECK_ALL:		return (max) ? MAX_POKES : 0 ;
	}
	return 0;
 }
 
 poke_printlang(index, const ___lang[], any:...)
 {
// The variable gets used via vformat, but the compiler doesn't know that, so it still cries.
#pragma unused ___lang

	new msg[256];
	vformat(msg, 255, LANG, 1);
	if( index == LANG_PLAYER ) index = 0;
	client_print(index, print_chat, "%s %s", PREFIX, msg);
 }
 poke_print(index, type, const message[], any:...)
 {
	new msg[256];
	vformat(msg, 255, message, 4);
	if( type == print_center )
		client_print(index, type, msg);
	else
		client_print(index, type, "%s %s", PREFIX, msg);
 }
