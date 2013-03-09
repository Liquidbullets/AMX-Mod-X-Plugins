
 #if POKEMON_UNEVOLVE==1
 Check_UnEvolve(id, pid)
 {
	new num = HasPokemon(id, pid)
	if(num<0)
		return false

	if( !AnyEvolveTo(pid) )
		return false

	new poke_that_evolves = 1;
	while( poke_that_evolves <= PokesCreated && !EvolveToThis(poke_that_evolves, pid) )
		poke_that_evolves++;

	new evolve = EvolveTo(poke_that_evolves, pid)

	new level = PlayersLevel(id, num)
	if( evolve >= 0 && level < evolve ){
		new name[32]
		get_user_name(id, name, 31)
		poke_printlang(id,"POKEMOD_UNEVOLVE",name,PokeToName(pid),PokeToName(poke_that_evolves))
		PlayersPoke[id][num] = poke_that_evolves
		RefreshPoke(id)

		return true
	}

	return false
 }
 EvolveTo(pid, this)
 {
	for(new i=0; i<MAX_EVOLVES; i++)
		if(PokeEvolveTo[i][pid]==this)
			return EvolveAtNum(pid, i)
	return 0
 }
 #endif
 
 Check_Evolve(id, pid, level=-1)
 {
	#if POKEMON_UNEVOLVE==1
	if(Check_UnEvolve(id, pid))
		return false
	#endif

	if( !(PokeInfo[pid] & PI_EVOLVE) || HasPokemon(id, pid, CHECK_ALL) < 0 ){
		return false
	}
	if(level==-1)
		level = PlayersLevel(id, HasPokemon(id,pid))

	new evolves_at = EvolveAt(pid, level)
	if(evolves_at > -1)
	{
		PlayerSelect[id][0] = SEL_EVOLVE
		PlayerSelect[id][1] = pid
		PlayerSelect[id][2] = PokeEvolveTo[evolves_at][pid]
		SelectMenu(id);

		return true
	}
	return false
 }
 
 Check_Stone_Evolve(id, pid, item)
 {
	if(!(PokeInfo[pid] & PI_EVOLVE) || HasPokemon(id, pid, CHECK_ALL)<0){
		return false
	}
	new iname[NAME_LENGTH], pname1[NAME_LENGTH]
	PokeToName(pid, pname1, NAME_LENGTH)
	ItemToName(item, iname, NAME_LENGTH)

	PlayersItem[id][item]--
	poke_printlang(id,"POKEMOD_GIVEITEM",pname1,iname)

	new evolves_at = EvolveAt(pid, -1*item)
	if(evolves_at > -1)
	{
		new pnum = HasPokemon(id, pid, CHECK_ALL)
		if(PokeChance(id, pnum)){
			new name[32]
			get_user_name(id, name, 31)
			PlayersPoke[id][pnum] = PokeEvolveTo[evolves_at][pid]
			if(pnum==PlayersActive[id])
				Pokemon[id] = PlayersPoke[id][pnum]
			poke_printlang(LANG_PLAYER,"POKEMOD_EVOLVE",name,pname1,PokeToName(PlayersPoke[id][pnum]))
			return true
		}
		else{
			poke_printlang(id,"POKEMOD_FAILGIVE",pname1,iname)
		}
	}
	else
		poke_printlang(id,"POKEMOD_NOWANT",pname1,iname)
	return false
 }
 Check_Trade_Evolve(id, pid)
 {
	if(!(PokeInfo[pid] & PI_EVOLVE) || HasPokemon(id, pid, CHECK_ALL) < 0 ){
		return false
	}

	new evolves_at = EvolveAt(pid, TRADE_EVOLVE)
	new pnum = HasPokemon(id, pid, CHECK_ALL)
	new pname1[NAME_LENGTH]
	PokeToName(pid, pname1, NAME_LENGTH)

	if(evolves_at > -1)
	{
		if(PokeChance(id, pnum)){
			new name[32], pnum = HasPokemon(id, pid, CHECK_ALL)
			get_user_name(id, name, 31)
			PlayersPoke[id][pnum] = PokeEvolveTo[evolves_at][pid]
			if(pnum==PlayersActive[id])
				Pokemon[id] = PlayersPoke[id][pnum]
			poke_printlang(LANG_PLAYER,"POKEMOD_EVOLVE",name,pname1,PokeToName(PlayersPoke[id][pnum]))
			return true
		}
	}

	switch(random(5))
	{
		case 0:{
			poke_printlang(id,"POKEMOD_TRADEGROWL",pname1)
			// happiness = 10
		}
		case 1:{
			poke_printlang(id,"POKEMOD_TRADEODD",pname1)
			// happiness = 20
		}
		case 2:{
			poke_printlang(id,"POKEMOD_TRADESMILE",pname1)
			// happiness = 30
		}
		case 3:{
			poke_printlang(id,"POKEMOD_TRADELIKE",pname1)
			// happiness = 40
		}
		case 4:{
			poke_printlang(id,"POKEMOD_TRADEJUMP",pname1)
			// happiness = 50
		}
	}

	return false
 }
 AnyEvolveTo(pid)
 {
	if( PokeInfo[pid] & PI_BASE )
		return 0;
	new i,k;
	for(k=0; k<=PokesCreated; k++)
	for(i=0; i<MAX_EVOLVES; i++)
		if(PokeEvolveTo[i][k]==pid)
			return k;
	return 0;
 }
 LevelEvolve(pid)
 {
	new low, evolve
	for(new i=0; i<MAX_EVOLVES; i++)
	{
		evolve = EvolveAtNum(pid, i)
		if(evolve > 0 && ( evolve < low || !low ) )
			low = evolve
	}
	return low
 }
 OtherEvolve(pid)
 {
	new low, evolve
	for(new i=0; i<MAX_EVOLVES; i++)
	{
		evolve = EvolveAtNum(pid, i)
		if(evolve < 0 && ( evolve < low || !low ) )
			low = evolve
	}
	return low
 }
 EvolveAt(pid, this)
 {
	for(new i=0; i<MAX_EVOLVES; i++)
		if( EvolveAtNum(pid, i)==this
		|| (EvolveAtNum(pid, i) > 0 && this > EvolveAtNum(pid, i)) )
			return i
	return -1
 }
 EvolveAtNum(pid, num)
 {
	return PokeEvolve[num][pid]
 }


