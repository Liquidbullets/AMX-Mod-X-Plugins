
 ///////////////////////////////
 //  Checks if PokeMod is on  //
 ///////////////////////////////
 bool:PMon()
	return PM_disabled != PM_ON ? false : get_ppcvar_num(pm_pokemod) ? true : false

 //////////////////////////////
 //  Checks if Saving is on  //
 //////////////////////////////
 bool:SaveXPon()
	return Safari() ? false : (get_ppcvar_num(pm_save) ? true : false)

 //////////////////////////////
 //  Checks if Safari is on  //
 //////////////////////////////
 bool:Safari()
	return get_ppcvar_num(pm_safari_mode) ? true : false

 /////////////////////////////////////
 //  Checks if Friendly Fire is on  //
 /////////////////////////////////////
 bool:FFon()
	return get_pcvar_num(mp_friendlyfire) ? true : false

 ///////////////////////////////
 //  Checks if PokeMod is on  //
 ///////////////////////////////
 bool:FFAon()
	return get_ppcvar_num(pm_ffa) ? true : false
 
 //////////////////////////////////////////////
 //  Checks if they are near oak/in buyzone  //
 //////////////////////////////////////////////
 BuyCheck(id)
 {
	if(PMon()){
		if( Pokemod_Alive(id) ){
			if(NearOakCheck(id)>0){
				if( !CanBuy ) poke_print(id, print_center, "%d seconds have passed. \nYou can't buy anything now!", floatround(get_ppcvar_float(pm_buytime)*60))
				else return 1
			}
			#if MOD==CS
			else if(cs_get_user_buyzone(id)){
				if( !CanBuy ) poke_print(id, print_center, "%d seconds have passed. \nYou can't buy anything now!", floatround(get_ppcvar_float(pm_buytime)*60))
				else return 1
			}
			#endif
		}
		else return 1
	}
	return 0
 }
 
 ///////////////////////////////////
 //  Checks if they are near oak  //
 ///////////////////////////////////
 NearOakCheck(id)
 {
	if(!PMon())
		return -1
	if(Pokemod_Alive(id)){
		new pokedist = get_ppcvar_num(pm_pokecenter), Float:dist = 2.0*pokedist

		for(new i=0; i<MAX_OAKS; i++){
			if(ValidPC(i)){
				if(dist>pokedist)
					dist = fm_entity_range(id,PCent[i])

				if(dist<=pokedist){
					new oak_props[MAX_OAKPROP]
					pev(PCent[i], pev_globalname, oak_props, MAX_OAKPROP-1)
					if(containi(oak_props, "c")==-1 || (containi(oak_props, "c")!=-1 && (Pokemod_Team(PCent[i])==0 || Pokemod_Team(id)==Pokemod_Team(PCent[i]))))
						return PCent[i]
				}
			}
		}
	}
	else
		return 0
	return -1
 }
 
 /////////////////////////////////////////
 //  Returns multiplier for pokedamage  //
 /////////////////////////////////////////
 Float:TypeCheck(attacker, victim, POKE_TYPE:atype=MAX_TYPES, POKE_TYPE:vtype=MAX_TYPES )
 {
	//This returns the multiplier for the damage
	new Float:green = 2.0, Float:orange = 0.5, Float:red = 0.0, Float:blank = 1.0;
	new Float:banana = blank;

	if( atype == MAX_TYPES )
		atype = PokeType[ Pokemon[attacker] ];
	else if( PokeType[ Pokemon[attacker] ] & atype )
		banana *= green-orange	//does 50% more damage

	if( vtype == MAX_TYPES )
		vtype = PokeType[ ValidPoke( Pokemon[victim] ) ];

	if( vtype == TYPE_NONE )
		return banana;

	if( atype & (TYPE_NORMAL) ){
		if(vtype & (TYPE_ROCK) || vtype & (TYPE_STEEL))
			banana *= orange
		if(vtype & (TYPE_GHOST))
			banana *= red
	}
	if( atype & (TYPE_FIRE) ){
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_BUG) || vtype & (TYPE_ICE) || vtype & (TYPE_STEEL))
			banana *= green
		if(vtype & (TYPE_FIRE) || vtype & (TYPE_WATER) || vtype & (TYPE_ROCK) || vtype & (TYPE_DRAGON))
			banana *= orange
	}
	if( atype & (TYPE_WATER) ){
		if(vtype & (TYPE_FIRE) || vtype & (TYPE_ROCK) || vtype & (TYPE_GROUND))
			banana *= green
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_WATER) || vtype & (TYPE_DRAGON))
			banana *= orange
	}
	if( atype & (TYPE_GRASS) ){
		if(vtype & (TYPE_WATER) || vtype & (TYPE_ROCK) || vtype & (TYPE_GROUND))
			banana *= green
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_FIRE) || vtype & (TYPE_BUG) || vtype & (TYPE_POISON) || vtype & (TYPE_FLYING) || vtype & (TYPE_DRAGON) || vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_ELECTRIC) ){
		if(vtype & (TYPE_WATER)|| vtype & (TYPE_FLYING))
			banana *= green
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_ELECTRIC) || vtype & (TYPE_DRAGON))
			banana *= orange
		if(vtype & (TYPE_GROUND))
			banana *= red
	}
	if( atype & (TYPE_ICE) ){
		if(vtype & (TYPE_WATER) || vtype & (TYPE_ICE) || vtype & (TYPE_STEEL))
			banana *= orange
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_FLYING) || vtype & (TYPE_DRAGON))
			banana *= green
	}
	if( atype & (TYPE_FIGHTING) ){
		if(vtype & (TYPE_POISON) || vtype & (TYPE_FLYING) || vtype & (TYPE_PSYCHIC) || vtype & (TYPE_BUG))
			banana *= orange
		if(vtype & (TYPE_NORMAL) || vtype & (TYPE_ICE) || vtype & (TYPE_ROCK) || vtype & (TYPE_DARK) || vtype & (TYPE_STEEL))
			banana *= green
		if(vtype & (TYPE_GHOST))
			banana *= red
	}
	if( atype & (TYPE_POISON) ){
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_BUG))
			banana *= green
		if(vtype & (TYPE_ROCK) || vtype & (TYPE_GROUND) || vtype & (TYPE_POISON) || vtype & (TYPE_GHOST))
			banana *= orange
		if(vtype & (TYPE_STEEL))
			banana *= red
	}
	if( atype & (TYPE_GROUND) ){
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_BUG))
			banana *= orange
		if(vtype & (TYPE_FIRE) || vtype & (TYPE_ELECTRIC) || vtype & (TYPE_ROCK) || vtype & (TYPE_POISON) || vtype & (TYPE_STEEL))
			banana *= green
		if(vtype & (TYPE_FLYING))
			banana *= red
	}
	if( atype & (TYPE_FLYING) ){
		if(vtype & (TYPE_ELECTRIC) || vtype & (TYPE_ROCK) || vtype & (TYPE_STEEL))
			banana *= orange
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_FIGHTING) || vtype & (TYPE_BUG))
			banana *= green
	}
	if( atype & (TYPE_PSYCHIC) ){
		if(vtype & (TYPE_POISON) || vtype & (TYPE_FIGHTING))
			banana *= green
		if(vtype & (TYPE_PSYCHIC) || vtype & (TYPE_STEEL))
			banana *= orange
		if(vtype & (TYPE_DARK))
			banana *= red
	}
	if( atype & (TYPE_BUG) ){
		if(vtype & (TYPE_GRASS) || vtype & (TYPE_PSYCHIC) || vtype & (TYPE_POISON) || vtype & (TYPE_DARK))
			banana *= green
		if(vtype & (TYPE_FIRE) || vtype & (TYPE_FIGHTING) || vtype & (TYPE_FLYING) || vtype & (TYPE_GHOST) || vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_ROCK) ){
		if(vtype & (TYPE_FIRE) || vtype & (TYPE_ICE) || vtype & (TYPE_FLYING) || vtype & (TYPE_BUG))
			banana *= green
		if(vtype & (TYPE_FIGHTING) || vtype & (TYPE_GROUND) || vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_GHOST) ){
		if(vtype & (TYPE_GHOST))
			banana *= green
		if(vtype & (TYPE_DARK) || vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_DRAGON) ){
		if(vtype & (TYPE_DRAGON))
			banana *= green
		if(vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_DARK) ){
		if(vtype & (TYPE_DARK) || vtype & (TYPE_STEEL))
			banana *= orange
	}
	if( atype & (TYPE_STEEL) ){
		if(vtype & (TYPE_STEEL))
			banana *= orange
	}
	return banana
 }
 
 ///////////////////////////////////
 //  If someone can do something  //
 ///////////////////////////////////
 PassTest(id, num)
 {
	#if MOD==CS
	if( get_user_weapon(id) == CSW_C4 ){
		poke_printlang(id,"POKEMOD_CHECKC4")
		return false
	}
	#endif

	if( num > 0 && fm_ViewContents(id) == CONTENTS_SKY ){
		poke_printlang(id,"POKEMOD_CHECKSKY")
		return false
	}
	return true
 }
 
 ////////////////////
 //  Checks Maxhp  //
 ////////////////////
 public CheckMaxHP(id)
 {
	if(!Pokemod_Connected(id))
		return 0;

	if( PlayerStat[id][STAT_MAXHP] >= 0 ){
		new servermax = get_ppcvar_num(pm_maxhp)
		new bonus

		for(new i=0; i<ItemsCreated; i++)
			bonus += ItemMaxHP[i]*HasPokeItem(id, i)

		bonus += AverageLevel(id)

		new start = 100
		//aliens in NS have different starting HPs
		#if MOD==NS
			switch(ns_get_class(id))
			{
				case CLASS_SKULK: start = 70
				case CLASS_GORGE: start = 150
				case CLASS_LERK: start = 125
				case CLASS_FADE: start = 300
				case CLASS_ONOS: start = 700
				case CLASS_GESTATE: start = 200
				default: start = 100
			}
		#endif

		PlayerStat[id][STAT_MAXHP] = start+bonus

		if(PlayerStat[id][STAT_MAXHP] > servermax){
			poke_printlang(id,"POKEMOD_CHECKHP", servermax)
			PlayerStat[id][STAT_MAXHP] = servermax
		}
	}
	set_pev(id, pev_max_health, float( abs( PlayerStat[id][STAT_MAXHP] ) ) )
	return abs( PlayerStat[id][STAT_MAXHP] );
 }
 ///////////////////////
 //  Checks Maxarmor  //
 ///////////////////////
 public CheckMaxArmor(id)
 {
	if(!Pokemod_Connected(id))
		return 0;

	if( PlayerStat[id][STAT_MAXAP] >= 0 ){
		new servermax = get_ppcvar_num(pm_maxarmor)

		new bonus

		for(new i=0; i<ItemsCreated; i++)
			if( ItemInfo[i] & II_ARMOR && HasPokeItem(id, i) )
				bonus += ItemArmor[i]*PlayersItem[id][i];

		new start = 100
		#if MOD==NS
			new thingy;
			pev(id, pev_iuser4, thingy);
			if( Pokemod_Team(id) == 1 ){
				if(thingy&MASK_ARMOR3)
					start = 90;
				else if(thingy&MASK_ARMOR2)
					start = 70;
				else if(thingy&MASK_ARMOR1)
					start = 50;
				else
					start = 30;
				if(ns_get_class(id)==CLASS_HEAVY)
					start += 200;
			}
			else{
				switch(ns_get_class(id))
				{
					case CLASS_SKULK:		start = (thingy&MASK_CARAPACE) ? 30 : 10 ;
					case CLASS_GORGE:		start = (thingy&MASK_CARAPACE) ? 120 : 70 ;
					case CLASS_LERK:		start = (thingy&MASK_CARAPACE) ? 60 : 30 ;
					case CLASS_FADE:		start = (thingy&MASK_CARAPACE) ? 250 : 150 ;
					case CLASS_ONOS:		start = (thingy&MASK_CARAPACE) ? 950 : 600 ;
					case CLASS_GESTATE:		start = 150;
					default:				start = 10;
				}
			}
		#endif

		PlayerStat[id][STAT_MAXAP] = start+bonus

		if(PlayerStat[id][STAT_MAXAP] > servermax){
			poke_printlang(id,"POKEMOD_CHECKHP", servermax)
			PlayerStat[id][STAT_MAXAP] = servermax
		}
	}
	return abs( PlayerStat[id][STAT_MAXAP] );
 }
 
 /////////////////////////////////////////
 //  Gives a chance based on users lvl  //
 /////////////////////////////////////////
 stock Chance(id)
	return PokeChance(id, -1);

 //////////////////////////////////////////////////////
 //  Gives a chance based on users lvl of a pokemon  //
 //////////////////////////////////////////////////////
 stock PokeChance(id, pnum)
	return ( PlayersLevel(id, pnum) > random_num(0, TotalLevels + 1) );

 ////////////////////////////////////////////////////
 //  Gives a chance based on users lvl of a skill  //
 ////////////////////////////////////////////////////
 stock SkillChance(id, skill)
	return ( SkillLevel(id, skill) > random_num(0, TotalLevels + 1) );

 ///////////////////////////////////////////////////////
 //  Gives a percent based on users lvl of a pokemon  //
 ///////////////////////////////////////////////////////
 stock SkillPercent(id, skill, num)
	return (( num * SkillLevel(id,skill) ) / ( TotalLevels ));
 stock Float:f_SkillPercent(id, skill, Float:num)
	return (( num * SkillLevel(id,skill) ) / ( TotalLevels ));

 stock SkillClamp(id, skill, low, high)
	return ( low + SkillPercent(id, skill, high-low) );
 stock Float:f_SkillClamp(id, skill, Float:low, Float:high)
	return ( low + f_SkillPercent(id, skill, high-low) );

 stock SkillChanceClamp(id, skill, min=0, max=100)
 {
	if( max <= 0 ) return false;
	return ( SkillLevel(id,skill) > random( (( 100 * (TotalLevels+1) ) / SkillClamp(id, skill, min, max) ) + 1 ) ) ? true : false;
 }

 /////////////////////////////////////////////////////
 //  Gives a chance based on users lvl, with a max  //
 /////////////////////////////////////////////////////
 stock MaxChance(id, max)
 {
	if( max <= 0 ) return false;
	return ( PlayersLevel(id) > random( (( 100 * (TotalLevels+1) ) / max ) + 1 ) ) ? true : false;
 }

 /////////////////////////////////////////////////////
 //  Gives a chance based on skill lvl, with a max  //
 /////////////////////////////////////////////////////
 stock MaxSkillChance(id, skill, max)
 {
	if( max <= 0 ) return false;
	return ( SkillLevel(id,skill) > random( (( 100 * (TotalLevels+1) ) / max ) + 1 ) ) ? true : false;
 }
 
 ///////////////////////////
 //  If user has pokemon  //
 ///////////////////////////
 HasPokemon(id, poke, CHECK_POKES:checker=CHECK_ACTIVE)
 {
	if( !ValidPoke(poke) )
		return -1

	static i, start, max;
	start = CheckerNumber( checker, false );
	max = CheckerNumber( checker, true );

	for( i=start; i<max; i++ )
		if( PlayersPoke[id][i] == poke )
			return i;

	if( Pokemon[id] == poke )
		return -2;

	return -1;
 }
 
 /////////////////////////////////
 //  Chance to catch a pokemon  //
 /////////////////////////////////
 CatchChance(id, poke, level)
 {
	if( !ValidPoke(poke) )
		return false

	if( g_isWild[id]>0 || PokeDisabled(poke) )
		return false

	#if CATCH_GHOST == 1
	if( PokeType[poke] & TYPE_GHOST && HasSilphScope(id) == MAX_ITEMS )
		return false
	#endif

	new i, ran, flaga, flagb, catch

	switch( AvailableSlot(id, poke) )
	{
		case AS_DUPLICATE: flaga = 1
		case AS_NOROOM: flagb = 1
	}

	for( i = ItemsCreated; i >= 0; i-- )
	{
		while( HasPokeItem(id, i) && ItemInfo[i] & II_CATCH ){
			ran = random_num(0,TotalLevels + level)
			if( PokeInfo[poke] & PI_SPECIAL )
				ran += random_num(0,level) //make it harder

			PlayersItem[id][i]--

			if( ItemCatch[i]>=1000 ){
				catch = 1
				break
			}
			if( ItemCatch[i]>=ran && !(PokeInfo[poke]&PI_NOCATCH) ){

				#if CATCH_MAX_LEVEL > 0
				if( level > CATCH_MAX_LEVEL )
					continue
				#endif

				catch = 1
				break
			}
		}
	}

	if( catch ){
		if( flagb ){
			poke_printlang(id,"POKEMOD_CATCHROOM", MAX_ACTIVE_POKES)
			return false
		}
		if( flaga ){
			poke_printlang(id,"POKEMOD_CATCHAGAIN", PokeToName(poke))
			return false
		}
		return true
	}

	return false
 }
 EntIsPC(ent)
 {
	for(new i=0; i<MAX_OAKS; i++){
		if( PCent[i] == ent && ValidPC(i) )
			return true;
	}
	return false;
 }
 ValidPC(num)
 {
	if( pev_valid( PCent[num] ) ){
		new class[32]
		pev( PCent[num], pev_classname, class, 31 );
		if(equali(class,"npc_pokecenter"))
			return true;
		else
			PCent[num] = 0;
	}
	return false;
 }
 knife_weapon(wpn)
 {
	if( !wpn )
		return true;

	#if MOD==CS
		if(wpn == CSW_KNIFE
		)
			return true;
	#endif
	#if MOD==DOD
		if(wpn == DODW_GERKNIFE
		|| wpn == DODW_AMERKNIFE
		|| wpn == DODW_SPADE
		)
			return true;
	#endif
	#if MOD==NS
		if(wpn == WEAPON_BITE
		|| wpn == WEAPON_BITE2
		|| wpn == WEAPON_SWIPE
		|| wpn == WEAPON_KNIFE
		|| wpn == WEAPON_CLAWS
		)
			return true;
	#endif
	return false;
 }
 grenade_weapon(wpn)
 {
	#if MOD==CS
		if(wpn == CSW_FLASHBANG
		|| wpn == CSW_HEGRENADE
		|| wpn == CSW_SMOKEGRENADE
		)
			return true;
	#endif
	#if MOD==DOD
		if(wpn == DODW_HANDGRENADE
		|| wpn == DODW_STICKGRENADE
		|| wpn == DODW_HANDGRENADE_EX
		|| wpn == DODW_STICKGRENADE_EX
		)
			return true;
	#endif
	#if MOD==NS
		if(wpn == WEAPON_GRENADE
		|| wpn == WEAPON_MINE
		)
			return true;
	#endif
	return false;
 }
 
 AvailableSlot(id, pid, CHECK_POKES:checker=CHECK_ACTIVE)
 {
	if(g_isWild[id]>0)
		return AS_NOGOOD
	new i, has = 0, start, max;
	start = CheckerNumber( checker, false );
	max = CheckerNumber( checker, true );

	for( i=start; i<max; i++ ){
		if( PlayersPoke[id][i] == pid )
			return AS_DUPLICATE
		if( !PlayersPoke[id][i] )
			return AS_OK
		has++
	}
	if( has == max-start )
		return AS_NOROOM

	return AS_NOGOOD
 }
 
 Slot(id, pid=0, CHECK_POKES:checker=CHECK_ACTIVE)
 {
	static i, start, max;
	start = CheckerNumber( checker, false );
	max = CheckerNumber( checker, true );

	for( i=start; i<max; i++){
		if( PlayersPoke[id][i] == pid )
			return i
	}
	return -1
 }

 SkillTrulyActive(id, skill)
 {
	new pid = Pokemon[id];
	while( pid ){
		if( PokeSkill[pid]==skill )
			return true;
		pid = AnyEvolveTo(pid);
	}
	return false;
 }

 BindTime(id)
 {
	if( BindUsed[id] ){
		if( BindUsed[id] > get_gametime() || BindUsed[id] == WAIT_TIME )
			return true;
		BindUsed[id] = 0.0;
	}
	return false;
 }
 
 ValidData( Data[] )
 {
	if( Data[0] == ';' || equali(Data,"//",2) )
		return 0;
	return strlen(Data);
 }
 ValidPlayer( id )
 {
	return is_user_connected(id);
/*
	if( id > 0 && id <= MAX_PLAYERS )
		return true;
	return false;
*/
 }
 ValidPoke( pid )
 {
	if( pid > 0 && pid <= PokesCreated )
		if( PokeType[pid] || PokeInfo[pid] )
			return pid;
	return 0;
 }
 bool:ValidSkill( skill )
 {
	if( skill > -1 && skill < SkillsCreated )
		return true;
	return false;
 }
 bool:ValidItem( item )
 {
	if( item > -1 && item < ItemsCreated )
		return true;
	return false;
 }
 bool:ValidSlot( slot )
 {
	if( slot > -1 && slot < MAX_POKES )
		return true;
	return false;
 }
 is_str_float( str[] )
 {
	new len = strlen( str );
	for( new i=0; i<len; i++ ){
		switch( str[i] )
		{
			case '0','1','2','3','4','5','6','7','8','9','.','-': continue;
			default: return false;
		}
	}
	return true;
 }
	
 