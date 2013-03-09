
 ///////////////////////
 //--PokeMod Natives--//
 //  To be used with other plugins to interact with PokeMod with ease  //
 ////////////////////////////////////////////////////////////////////////
 public plugin_natives()
 {
	register_library("pokemod")

	register_native("PokeMod","_PokeMod")

	register_native("Poke_NameToPoke","_NameToPoke")
	register_native("Poke_PokeToName","_PokeToName")
	register_native("Poke_NameToItem","_NameToItem")
	register_native("Poke_ItemToName","_ItemToName")

	register_native("Poke_PokeInfo","_PokeInfo")
	register_native("Poke_ItemInfo","_ItemInfo")

	register_native("Poke_PlayerPokemon","_PlayerPokemon")
	register_native("Poke_PlayerXP","_PlayerXP")
	register_native("Poke_PlayerItem","_PlayerItem")
	register_native("Poke_PlayerStatus","_PlayerStatus")
	register_native("Poke_PlayerStat","_PlayerStat")
	register_native("Poke_PlayerWild","_PlayerWild")

	register_native("Poke_Set_PlayerPokemon","_Set_PlayerPokemon")
	register_native("Poke_Set_PlayerXP","_Set_PlayerXP")
	register_native("Poke_Set_PlayerItem","_Set_PlayerItem")
	register_native("Poke_Set_PlayerStatus","_Set_PlayerStatus")
	register_native("Poke_Set_PlayerStat","_Set_PlayerStat")
	register_native("Poke_Set_PlayerWild","_Set_PlayerWild")

	#if POKERANK_SYSTEM==1
		register_native("Poke_PlayerRank","_PlayerRank")
		register_native("Poke_Set_PlayerRank","_Set_PlayerRank")
	#endif

	register_native("Poke_Damage","_Damage")
	register_native("Poke_AoE_Damage","_AoE_Damage")
	register_native("Poke_Line_Damage","_Line_Damage")

	register_native("Poke_Give_Poke","_Give_Poke")
	register_native("Poke_Give_XP","_Give_XP")
	register_native("Poke_Give_Item","_Give_Item")

	register_native("Poke_LevelXP","_LevelXP")

	register_native("Poke_MakeThisOak","_MakeThisOak")

	register_native("Poke_MaxHP","_MaxHP")
	register_native("Poke_MaxArmor","_MaxArmor")
	register_native("Poke_Money","_Money")
	register_native("Poke_Delay","_Delay")

	register_native("Poke_Set_MaxHP","_Set_MaxHP")
	register_native("Poke_Set_MaxArmor","_Set_MaxArmor")
	register_native("Poke_Set_Money","_Set_Money")
	register_native("Poke_Set_Delay","_Set_Delay")
	register_native("Poke_Reset_Speed","_Reset_Speed")

	register_native("Poke_Heal","_Heal")
	register_native("Poke_Pay","_Pay")
	register_native("Poke_Repair","_Repair")

	register_native("Poke_Unstuck","_Unstuck")

	register_native("Poke_EventInfo","_EventInfo")

	register_native("Poke_Item_Price","_Item_Price")
	register_native("Poke_Buy_Check","_Buy_Check")

	register_native("Poke_ItemsRegistered","_ItemsRegistered")

	register_native("Register_PokeItem","_Register_PokeItem")
 }
 public _PokeMod(iPlugin,iParams)
 {
	if( PM_disabled == PM_ON && PMon() )
		return 1

	return 0
 }
 public _LevelXP(iPlugin,iParams)
 {
	if(iParams!=1)
		return 0
	new level = get_param(1)
	if( !level )
		return TotalLevels

	if( level < 0 || level > TotalLevels )
		return 0

	return PokeLevels[level-1]
 }
 public _Unstuck(iPlugin,iParams)
 {
	if(iParams!=2)
		return 0
	new id = get_param(1)
	if( !Pokemod_Alive(id) )
		return 0

	return poke_UnStuck(id, bool:get_param(2))
 }
 public _EventInfo(iPlugin,iParams)
 {
	if(iParams!=3)
		return 0
	new id = get_param(1)
	if( !Pokemod_Connected(id) )
		return 0

	new damage, weapon, target = PokeSkillTarget(id, damage, weapon);
	set_param_byref( 2, damage );
	set_param_byref( 3, weapon );
	return target;
 }

 public _PokeInfo(iPlugin,iParams)
 {
	if(iParams!=1)
		return 0
	new poke = get_param(1)
	if( !ValidPoke(poke) )
		return 0
	return PokeInfo[poke]
 }
 public _ItemInfo(iPlugin,iParams)
 {
	if(iParams!=1)
		return 0
	new item = get_param(1)
	if( !ValidItem(item) )
		return 0
	return ItemInfo[item]
 }
 public _NameToPoke(iPlugin,iParams)
 {
	if(iParams!=1)
		return 0
	new name[NAME_LENGTH]
	get_string(1, name, NAME_LENGTH-1)
	return NameToPoke(name)
 }
 public _PokeToName(iPlugin,iParams)
 {
	if(iParams!=3)
		return 0
	new poke = get_param(1)
	new len = get_param(3)
	new name[NAME_LENGTH]
	PokeToName(poke, name, len)
	set_string(2, name, len)
	return 1
 }
 public _NameToItem(iPlugin,iParams)
 {
	if(iParams!=1)
		return 0
	new name[NAME_LENGTH]
	get_string(1, name, NAME_LENGTH-1)
	return NameToItem(name,false)
 }
 public _ItemToName(iPlugin,iParams)
 {
	if(iParams!=3)
		return 0
	new item = get_param(1)
	new len = get_param(3)
	new name[NAME_LENGTH]
	ItemToName(item, name, len)
	set_string(2, name, len)
	return 1
 }
 public _Give_XP(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new poke = get_param(2)
	if( poke == -1 ) poke = PlayersActive[id];
	if( !ValidSlot(poke) )
		return 0

	new xp = get_param(3)
	GiveXP(id, poke, xp)

	return xp
 }
 public _Buy_Check(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	return BuyCheck(id)
 }
 public _Item_Price(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new num = get_param(1)
	if( num < 0 )
		return 0

	new is_item = get_param(2)
	if( !is_item ){
		if( !ValidPoke(num) )
			return 0;
		return CostOfPoke(num)
	}

	if( !ValidItem(num) )
		return 0
	return CostOfItem(num)
 }
 public _Give_Item(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new item = get_param(2)
	if( !ValidItem(item) )
		return 0

	new amount = get_param(3)
	if( !amount )
		return PlayersItem[id][item]
	PlayersItem[id][item] += amount

	return 1
 }
 
 public _Damage(iPlugin,iParams)
 {
	if(iParams!=4)
		return 0
	new victim = get_param(1)
	new attacker = get_param(2)
	if(!Pokemod_Connected(victim) || !Pokemod_Connected(attacker))
		return 0
	new damage = get_param(3)
	new Attack[51]
	get_string(4, Attack, 50)
	new POKE_TYPE:_attack_type = POKE_TYPE:get_param(5)
	new POKE_TYPE:_victim_type = POKE_TYPE:get_param(6)
	new PM_STATUS:_status = PM_STATUS:get_param(7)
	new status_mult = get_param(8)
	return pokedamage(victim, attacker, damage, Attack, _attack_type, _victim_type, _status, status_mult)
 }
 public _AoE_Damage(iPlugin,iParams)
 {
	if(iParams!=6)
		return 0
	new aid = get_param(1)
	if(!Pokemod_Connected(aid))
		return 0
	new Float:explode[3]
	get_array_f(2, explode, 3)
	new radius = get_param(3)
	new maxdamage = get_param(4)
	new Attack[51]
	get_string(5, Attack, 50)
	new AOE_FLAGS:aoe_flags = AOE_FLAGS:get_param(6)
	new POKE_TYPE:_attack_type = POKE_TYPE:get_param(7)
	new POKE_TYPE:_victim_type = POKE_TYPE:get_param(8)
	new PM_STATUS:_status = PM_STATUS:get_param(9)
	new status_mult = get_param(10)
	return AoEdamage(aid, explode, radius, maxdamage, Attack, aoe_flags, _attack_type, _victim_type, _status, status_mult)
 }
 public _Line_Damage(iPlugin,iParams)
 {
	if(iParams!=6)
		return 0
	new aid = get_param(1)
	if(!Pokemod_Connected(aid))
		return 0
	new Float:start[3], Float:end[3]
	get_array_f(2, start, 3)
	get_array_f(3, end, 3)
	new radius = get_param(4)
	new maxdamage = get_param(5)
	new Attack[51]
	get_string(6, Attack, 50)
	new POKE_TYPE:_attack_type = POKE_TYPE:get_param(7)
	new POKE_TYPE:_victim_type = POKE_TYPE:get_param(8)
	new PM_STATUS:_status = PM_STATUS:get_param(9)
	new status_mult = get_param(10)
	return Linedamage(aid, start, end, radius, maxdamage, Attack, _attack_type, _victim_type, _status, status_mult)
 }

 public _Give_Poke(iPlugin,iParams)
 {
	if(iParams != 5)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new poke = get_param(2)
	if( !ValidPoke( poke ) )
		return 0

	new level = get_param(3)
	if(0>=level || level>TotalLevels){
		debugMessage(0,0, "Native", "Plugin %d tryed to give a pokemon with a bad level (%d)",iPlugin,level)
		return 0
	}
	new giver = get_param(4)
	new CHECK_POKES:checker = CHECK_POKES:get_param(5);

	if(AvailableSlot(id, poke, checker)!=AS_OK)
		return 0

	AddPoke(id, poke, PokeLevels[level-1], giver, checker)
	return 1
 }
 
 public _Set_PlayerItem(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new item = get_param(2)
	if( !ValidItem(item) )
		return 0

	new new_num = get_param(3)
	PlayersItem[id][item] = new_num

	return 1
 }
 
 public _PlayerItem(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new item = get_param(2)
	if( !ValidItem(item) )
		return PokeItem(id)

	return PlayersItem[id][item]
 }
 public _PlayerStat(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if( num <= -1 || num >= MAX_STAT )
		return 0

	return PlayerStat[id][num]
 }
 public _Set_PlayerStat(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if( num <= -1 || num >= MAX_STAT )
		return 0
	new amount = get_param(3)
	PlayerStat[id][num] = amount;
	return 1
 }
 public _Set_PlayerWild(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2);
	g_isWild[id] = num;
	return 1
 }
 public _PlayerWild(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	return g_isWild[id];
 }
 
 #if POKERANK_SYSTEM==1
 public _PlayerRank(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	return PlayersCatch[id]
 }
 public _Set_PlayerRank(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new amount = get_param(2)
	PlayersCatch[id] = amount;
	return 1
 }
 #endif
 
 public _Set_PlayerStatus(iPlugin,iParams)
 {
	if(iParams != 4)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new inflictor = get_param(4)
	if(!Pokemod_Connected(inflictor))
		return 0

	new PM_STATUS:_status = PM_STATUS:get_param(2)
	new Float:_time = get_param_f(3)

	if( _status == MAX_STATUS && _time == 0.0 ){
		ResetStatus(id)
		return 1
	}
	if( _status < STATUS_BURN || _status >= MAX_STATUS )
		return 0

	ApplyStatus( inflictor, id, _status, _time );

	return 1
 }
 
 public _PlayerStatus(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new PM_STATUS:_status = PM_STATUS:get_param(2)
	if( _status < STATUS_BURN || _status >= MAX_STATUS )
		return 0

	return g_Status[id][_status]
 }
 
 public _Set_PlayerPokemon(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if( !ValidSlot(num) )
		return 0

	new newpoke = get_param(3)
	PlayersPoke[id][num] = newpoke
	RefreshPoke(id)

	return 1
 }
 
 public _PlayerPokemon(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if(num==-1)
		return Pokemon[id]

	return PlayersPoke[id][num]
 }
 
 public _Set_PlayerXP(iPlugin,iParams)
 {
	if(iParams != 3)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if( !ValidSlot(num) )
		return 0

	new newxp = get_param(3)
	PlayersXP[id][num] = newxp
	RefreshLevels(id);
	RefreshPoke(id)

	return 1
 }
 
 public _PlayerXP(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if(num==-1)
		return PlayersXP[id][PlayersActive[id]]

	return PlayersXP[id][num]
 }
 public _PlayerLevel(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0
	new num = get_param(2)
	if(num==-1)
		return PlayersLevel(id, PlayersActive[id])

	return PlayersLevel(id, num)
 }

 // Thanks to Hawk552 
 public _MakeThisOak(iPlugin,iParams)
 {
	if(iParams != 5)
		return 0

	new Float:Origin[3],Float:Angles[3],hp,team,props[MAX_OAKPROP]

	get_array_f(1,Origin,3)
	Angles[1] = get_param_f(2) - 180
	hp = get_param(3)
	team = get_param(4)
	get_string(5,props,MAX_OAKPROP-1)

	new oak_num = find_free_oak()
	if(oak_num==-1)
		return 0
	PCent[oak_num] = fm_create_entity("info_target")
	if(!PCent[oak_num])
		return 0

	if(!create_oak(PCent[oak_num], Origin, Angles, team, props, hp)){
		fm_remove_entity(PCent[oak_num])
		return 0
	}

	return PCent[oak_num]
 }
 
 public _Money(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	return Pokemod_Money(id)
 }
 public Float:_Delay(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0.0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0.0

	return BindUsed[id]
 }
 public _Set_Delay(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	BindUsed[id] = get_param_f(2)

	return 1
 }
 public _MaxHP(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	CheckMaxHP(id)

	return PlayerStat[id][STAT_MAXHP]
 }
 public _MaxArmor(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	CheckMaxArmor(id)

	return PlayerStat[id][STAT_MAXAP]
 }
 
 public _Set_MaxHP(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new num = get_param(2)
	PlayerStat[id][STAT_MAXHP] = num

	return 1
 }
 public _Set_MaxArmor(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new num = get_param(2)
	PlayerStat[id][STAT_MAXAP] = num

	return 1
 }
 public _Set_Money(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new newmoney = clamp(get_param(2),0,MAX_MONEY)

	#if MOD==CS
		cs_set_user_money(id, newmoney , 1)
	#endif

	money[id] = newmoney

	return 1
 }

 public _Reset_Speed(iPlugin,iParams)
 {
	if(iParams != 1)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	SetSpeed(id)

	return 1
 }
 
 public _Heal(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new num = get_param(2)

	Pokemod_Heal(id, num)

	return 1
 }
 public _Pay(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new num = get_param(2)
	new do_forward = get_param(3)

	Pokemod_Pay(id, num, do_forward)

	return 1
 }
 public _Repair(iPlugin,iParams)
 {
	if(iParams != 2)
		return 0

	new id = get_param(1)
	if(!Pokemod_Connected(id))
		return 0

	new num = get_param(2)

	Pokemod_Repair(id, num)

	return 1
 }
 public _ItemsRegistered(iPlugin,iParams)
	return ItemsCreated;
 public _Register_PokeItem(iPlugin,iParams)
 {
	if(iParams != 5)
		return 0

	new Left[NAME_LENGTH], item;
	get_string(1, Left, NAME_LENGTH-1);

	if( (item = NameToItem(Left)) != MAX_ITEMS ){
		return item;
	}

	item = ItemsCreated
	if(item == MAX_ITEMS){
		debugMessage(_,_, "Native", "Max items reached. Cannot create item %s",Left)
		return 0
	}

	copy( ItemName[item], NAME_LENGTH-1, Left)
	ItemsCreated++
	ItemPrice[ item ] = get_param(2);

	new Right[151];
	get_string(3, Right, 150);

	ItemInfo[item] = 0

	while( ValidData(Right) ){
		strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
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

	get_string(4, Left, NAME_LENGTH-1);
	new cat = NameToCat(Left);
	if( cat == MAX_ITYPE && ItemCatCreated < MAX_ITYPE ){
		copy( ItemCatName[ItemCatCreated], NAME_LENGTH-1, Left);
		ItemCat[item] = ItemCatCreated++;
	}
	else{
		ItemCat[item] = cat;
	}

	get_string(5, ItemDesc[item], 100);

	return item;
 }
 
 
 