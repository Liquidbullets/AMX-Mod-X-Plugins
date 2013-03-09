
 ///////////////////
 //  Fake damage  //
 ///////////////////
 PokeEntDamage(ent, szClass[], damage, dmg_type=DMG_GENERIC)
 {
	if( pev_valid( ent ) ){
		static Float:imafloat;
		pev( ent, pev_takedamage, imafloat );
		if( imafloat && fm_fakedamage(ent, szClass, float( damage ), dmg_type) )
			return damage;
	}
	return 0;
 }
 skilldamage(victim, attacker, damage, skill, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1, bool:allow_extra = true)
	return pokedamage(victim, attacker, damage, SkillToName(skill), SkillType[skill], victim_type, status, status_mult, allow_extra);
 pokedamage(victim, attacker, damage, Attack[], POKE_TYPE:attack_type=MAX_TYPES, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1, bool:allow_extra = true)
 {
	if( !Pokemod_Alive(victim) || !Pokemod_Connected(attacker) || !PMon() )
		return 0;

	if( !ValidPlayer( victim ) ){
		if( pev_valid( victim ) )
			return PokeEntDamage( victim, Attack, damage );
		return 0;
	}

	new namek[32], namev[32]
	get_user_name(victim,namev,31)
	get_user_name(attacker,namek,31)

	new tk = Pokemod_FF(victim, attacker);

	debugMessage(attacker,5,"Player", "damaged %s with %s for %d (TK=%d)",namev,Attack,damage,tk)

	if( attack_type & TYPE_WATER )
		InWater(victim)

	//if it heals, don't bother with anything, just heal them
	if( damage < 0 ){
		Pokemod_Heal(victim, -1*damage);
		return -1*damage;
	}

	new hp = Health(victim)
	damage = floatround(damage*TypeCheck(attacker, victim, attack_type, victim_type))

	if( tk==1 && !FFon())
		damage = 0

	else if( get_user_godmode(victim) )
		damage = 0

	else if( damage != AUTO_KILL ){
		if(hp>HP_KILL_ALLOW && damage>HP_KILL_ALLOW)
			damage = HP_KILL_ALLOW-1
	}

	if( !damage )
		return 0

	// *** Damage calculation due to armor from: multiplayer/dlls/player.cpp ***
	new Float:new_damage = damage * ARMOR_RATIO
	new Float:armor_damage = (damage - new_damage) * ARMOR_BONUS
	new armor = get_user_armor(victim)

	// Does this use more armor than we have figured for?
	if( armor_damage > armor ){
		armor_damage = armor / ARMOR_BONUS
		new_damage = damage - armor_damage
		armor = 0
	}
	else{
		armor = floatround(armor - armor_damage)
	}

	damage = floatround(new_damage);
	new new_hp = hp-damage;

	PlayerDamageInfo[victim][PD_ATTACKER] = attacker;
	PlayerDamageInfo[victim][PD_DAMAGE] = damage;
	PlayerDamageInfo[victim][PD_WEAPON] = 0;

	skills_handler(attacker, SI_PATTACK);
	skills_handler(victim, SI_PDAMAGED);

	PlayerDamageInfo[victim][PD_ATTACKER] = PlayerDamageInfo[victim][PD_DAMAGE] = PlayerDamageInfo[victim][PD_WEAPON] = 0;

	#if POKEDAMAGE_AWAKEN==1
	//wake them up if they were sleeping
	if(g_Status[victim][STATUS_ASLEEP] && !equali(Attack,"Dream Eat") && !equali(Attack,"Poison"))
		unsleep(victim)
	#endif

	set_pev(victim, pev_dmg_inflictor, attacker);

	//they might die (depending on their armor)
	if( new_hp <= 0 ){
		//Kill the victim and block the messages
		set_msg_block(gmsgDeathMsg,BLOCK_ONCE);
		set_msg_block(gmsgScoreInfo,BLOCK_ONCE);

		log_block_state = true;
		user_kill(victim);
		log_block_state = false;

		if(!Pokemod_Alive(victim)){	//they died, oh noes!! let's tell people about it!
			ShowKill(victim, attacker, Attack, tk);
			return damage;
		}
	}

	//they didn't die
	logDmg(attacker, victim, Attack,"body",damage,new_hp);

	if( tk==1 && damage > 0 )
		poke_print(0,print_chat,"%s attacked a teammate",namek)

	set_user_health(victim, new_hp);
	set_user_armor(victim, armor);

	new aOrigin[3]
	get_user_origin(attacker, aOrigin)

	//Damage message
	message_begin(MSG_ONE_UNRELIABLE, gmsgDamage, _, victim)
	write_byte(0) // dmg_save
	write_byte(damage) // dmg_take
	write_long(0) // visibleDamageBits
	write_coord(aOrigin[0]) // damageOrigin.x
	write_coord(aOrigin[1]) // damageOrigin.y
	write_coord(aOrigin[2]) // damageOrigin.z
	message_end()

	if( status != MAX_STATUS ){
		if( status_mult == -1 || random_num(0,100) <= damage*status_mult )
			ApplyStatus( attacker, victim, status, damage/10.0 );
	}

	if( allow_extra ){
		for(new i=0; i < ItemsCreated; i++)
			if( ItemInfo[i] & II_PDAMAGE && HasPokeItem(attacker, i) )
				pokedamage(victim, attacker, (PlayersItem[attacker][i]*ItemPDamage[i]), ItemName[i], TYPE_NONE, _, _, _, false)
	}

	#if AUTO_ITEMS > 0
	AutoItems(victim);
	#endif

	return damage
 }
 
 ShowKill(victim, attacker, Attack[], tk)
 {
	//don't need to give kill xp since we resend the death message, so the kill xp is given in the other death event
	//KillXP(attacker, victim)

	#if MOD==CS
	//make them view who killed them
	set_pev(victim, pev_iuser3, attacker);
	#endif

	logKill(attacker, victim, Attack);

	new afrags = get_user_frags(attacker);
	new vfrags = get_user_frags(victim);

	#if MOD==CS
		Pokemod_Pay(attacker, tk ? -300:300 );
	#endif

	set_user_frags(attacker, tk ? --afrags:++afrags );

	if( tk == 1 ){
		poke_print(attacker,print_center,"You killed a teammate");
		set_user_frags(victim, ++vfrags);
	}

	//Replaced HUD death message
	emessage_begin(MSG_BROADCAST,gmsgDeathMsg,_,0);
	ewrite_byte(attacker);
	ewrite_byte(victim);
	ewrite_byte(0);
	ewrite_string(Attack);
	emessage_end();

	//Update killers scorboard with new info
	emessage_begin(MSG_BROADCAST,gmsgScoreInfo)
	ewrite_byte(attacker);
	ewrite_short(afrags);
	ewrite_short(get_user_deaths(attacker))
	ewrite_short(0);
	ewrite_short(Pokemod_Team(attacker));
	emessage_end();

	//Update victims scoreboard with correct info
	emessage_begin(MSG_BROADCAST,gmsgScoreInfo);
	ewrite_byte(victim);
	ewrite_short(vfrags);
	ewrite_short(get_user_deaths(victim));
	ewrite_short(0);
	ewrite_short(Pokemod_Team(victim));
	emessage_end();
 }

 ///////////////////
 //  Logs a kill  //
 ///////////////////
 stock logKill(attacker, victim, weaponDescription[])
 {
	// Save Hummiliation
	new namea[24], namev[24], authida[20], authidv[20], teama[10], teamv[10]

	// Info On Attacker
	get_user_name(attacker, namea, 23 )
	get_user_team(attacker, teama, 9 )
	get_user_authid(attacker, authida, 19 )
	if(!IsValidSteamID(authida)) {
		get_user_ip(attacker,authida,19,1);
	}
	// Info On Victim
	get_user_name(victim, namev, 23 )
	get_user_team(victim, teamv, 9 )
	get_user_authid(victim, authidv, 19 )
	if(!IsValidSteamID(authidv)) {
		get_user_ip(victim,authidv,19,1);
	}
	// Log This Kill
	if(attacker==victim){
		log_message("\"%s<%d><%s><%s>\" killed self with \"%s\"",
		namea, get_user_userid(attacker), authida, teama, namev, get_user_userid(victim), authidv, teamv, weaponDescription)
	}
	else{
		log_message("\"%s<%d><%s><%s>\" killed \"%s<%d><%s><%s>\" with \"%s\"",
		namea, get_user_userid(attacker), authida, teama, namev, get_user_userid(victim), authidv, teamv, weaponDescription)
	}
 }
 
 ///////////////////
 //  Logs damage  //
 ///////////////////
 stock logDmg(attacker, victim, weaponDescription[],hit[],damage,health)
 {
	if(get_pcvar_num(mp_logdetail) != 3) return

	// Save Hummiliation
	new namea[24], namev[24], authida[20], authidv[20], teama[10], teamv[10]

	// Info On Attacker
	get_user_name(attacker, namea, 23 )
	get_user_team(attacker, teama, 9 )
	get_user_authid(attacker, authida, 19 )
	if(!IsValidSteamID(authida)) {
		get_user_ip(attacker,authida,19,1);
	}
	// Info On Victim
	get_user_name(victim, namev, 23 )
	get_user_team(victim, teamv, 9 )
	get_user_authid(victim, authidv, 19 )
	if(!IsValidSteamID(authidv)) {
		get_user_ip(victim,authidv,19,1);
	}
	// Log This Damage
	log_message("\"%s<%d><%s><%s>\" attacked \"%s<%d><%s><%s>\" with \"%s\" (hit \"%s\") (damage \"%d\") (health \"%d\")",
	namea,get_user_userid(attacker),authida,teama,namev,get_user_userid(victim),authidv,teamv,weaponDescription,hit,damage,health)
 }
 
 /////////////////////////////
 //  Area of Effect Damage  //
 /////////////////////////////
 skillAoEdamage(attacker, Float:explode[3], radius, maxdamage, skill, AOE_FLAGS:aoe_flags=AOE_NORMAL, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
	return AoEdamage(attacker, explode, radius, maxdamage, SkillToName(skill), aoe_flags, SkillType[skill], victim_type, status, status_mult)
 AoEdamage(attacker, Float:explode[3], radius, maxdamage, Attack[], AOE_FLAGS:aoe_flags=AOE_NORMAL, POKE_TYPE:attack_type=MAX_TYPES, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
 {
	new Float:temporigin[3], Float:dist, damage, total_damage;

	new players[MAX_PLAYERS], pnum, id;
	new bool:ffa = FFAon();

	if( !ffa ){
		if( aoe_flags & AOE_ATEAM )
			poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_INTEAM, Pokemod_Team(attacker) );
		else if( aoe_flags & AOE_VTEAM )
			poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_NOTEAM, Pokemod_Team(attacker) );
		else
			poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL );
	}
	else poke_get_players( players, pnum, PLAYER_ALIVE|PLAYER_MORTAL );

	for(new i = 0; i<pnum; i++)
	{
		id = players[i];

		if( aoe_flags&AOE_IGNORE && attacker==id )
			continue;
		if( ffa ){
			if( aoe_flags & AOE_ATEAM && attacker!=id ) continue;
			else if( aoe_flags & AOE_VTEAM && attacker==id ) continue;
		}

		pev(id, pev_origin, temporigin);
		dist = get_distance_f( temporigin, explode );
		if( dist <= radius ){
			if( aoe_flags&AOE_MAXDAM )
				damage = maxdamage - floatround( ( maxdamage * dist) / radius );
			else
				damage = maxdamage;

			// check if the damage is less than max, because of crazy damage that can happen with weird ratios
			if( abs( damage ) <= abs( maxdamage ) )
				total_damage += pokedamage(id, attacker, damage, Attack, attack_type, victim_type, status, status_mult);
		}
	}
	debugMessage(attacker,5,"Player", "exploded %s for %d total AoE damage", Attack, total_damage);
	return total_damage;
 }
 
 /////////////////////
 //  Radial Damage  //
 /////////////////////
 stock skillradialdamage(attacker, victim, Float:explode[3], radius, maxdamage, skill, AOE_FLAGS:aoe_flags=AOE_NORMAL, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
	return radialamage(attacker, victim, explode, radius, maxdamage, SkillToName(skill), aoe_flags, SkillType[skill], victim_type, status, status_mult)
 stock radialdamage(attacker, victim, Float:explode[3], radius, maxdamage, Attack[], AOE_FLAGS:aoe_flags=AOE_NORMAL, POKE_TYPE:attack_type=MAX_TYPES, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
 {
	new Float:temporigin[3];
	pev(id, pev_origin, temporigin);
	new Float:dist = get_distance_f( temporigin, explode );
	if( dist <= radius ){
		new damage;
		if( aoe_flags&AOE_MAXDAM )
			damage = maxdamage - floatround( ( maxdamage * dist) / radius );
		else
			damage = maxdamage;

		// check if the damage is less than max, because of crazy damage that can happen with weird ratios
		if( abs( damage ) <= abs( maxdamage ) )
			return pokedamage(id, attacker, damage, Attack, attack_type, victim_type, status, status_mult);
	}
	return 0;
 }
 
 ///////////////////////////
 //  Damage along a line  //
 ///////////////////////////
 skillLinedamage(attacker, Float:start[3], Float:end[3], radius, maxdamage, skill, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
	return Linedamage(attacker, start, end, radius, maxdamage, SkillToName(skill), SkillType[skill], victim_type, status, status_mult)
 Linedamage(attacker, Float:start[3], Float:end[3], radius, maxdamage, Attack[], POKE_TYPE:attack_type=MAX_TYPES, POKE_TYPE:victim_type=MAX_TYPES, PM_STATUS:status=MAX_STATUS, status_mult=1)
 {
	new Float:temporigin[3], dist, damage, total_damage;

	new players[MAX_PLAYERS], pnum, id;
	poke_get_players(players,pnum,PLAYER_ALIVE|PLAYER_MORTAL);

	for(new i = 0; i<pnum; i++)
	{
		id = players[i]

		if( id == attacker ) continue

		pev(id, pev_origin, temporigin)
		dist = get_distance_to_line(start, end, temporigin)
		if( -1 < dist <= radius && fm_is_ent_visible(id, attacker) ){
			damage = maxdamage - ( (maxdamage * dist) / radius )

			// check if the damage is less than max, because of crazy damage that can happen with weird ratios
			if( abs( damage ) <= abs( maxdamage ) )
				total_damage += pokedamage(id, attacker, damage, Attack, attack_type, victim_type, status, status_mult);
		}
	}
	debugMessage(attacker,5,"Player", "shot %s for %d total line damage", Attack, total_damage)
	return total_damage
 }
 
 
 
 
 