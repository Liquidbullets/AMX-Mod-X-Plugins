 
 ////////////////////////
 //  Shows user stats  //
 ////////////////////////
 PokeDisplay(id, aid)
 {
	if ( !PMon() || !Pokemod_Connected(id) || !Pokemod_Connected(aid) ) return

	static message[256], iLen
	copy(message, 255, EMPTY)
	iLen = 0

	static pname[NAME_LENGTH], level
	if(PlayersSet[id][SET_HUD_PARTY] && !Safari()){
		for(new x=0; x<MAX_ACTIVE_POKES; x++){
			if(!PlayersPoke[aid][x])
				break
			iLen += formatex(message[iLen],(255-iLen),"%s(Lvl:%d)\n", PokeToName(PlayersPoke[aid][x]), PlayersLevel(aid, x))
		}
	}

	if(id != aid)
		iLen += formatex(message[iLen],(255-iLen)," HP: %d/%d  ", Health(aid), PlayerStat[aid][STAT_MAXHP])
	#if MOD==CS
	else if(Health(aid) > 255 || PlayerStat[aid][STAT_MAXHP] > 255)
		iLen += formatex(message[iLen],(255-iLen)," HP: %d/%d  ", Health(aid), PlayerStat[aid][STAT_MAXHP])
	#endif

	if(Pokemon[aid]){
		new temp[56], moneytemp[56]

		level = PlayersLevel(aid, PlayersActive[aid])
		PokeToName(Pokemon[aid], pname, NAME_LENGTH)

		if(BindUsed[aid]==WAIT_TIME)
			copy(temp,55,"Wait")
		else if(BindUsed[aid]>0.0)
			formatex(temp,55,"%.01f",BindUsed[aid]-get_gametime())
		else
			copy(temp,55,"Ready")

		#if MOD!=CS
			formatex(moneytemp,55," $%d",Money(aid))
		#else
			copy(moneytemp,55,EMPTY)
		#endif

		if(id==aid){
			if(Safari()){
				iLen += formatex(message[iLen],(255-iLen),"%L: %s (Skl:%s)%s", id,"POKEMOD_POKEMON", pname, temp, moneytemp)
			}
			else{
				if( level >= TotalLevels )
					iLen += formatex(message[iLen],(255-iLen),"%L: %s (Skl:%s Lvl:%d Exp:%d)%s", id,"POKEMOD_POKEMON", pname, temp, level, PlayersXP[id][PlayersActive[id]], moneytemp )
				else
					iLen += formatex(message[iLen],(255-iLen),"%L: %s (Skl:%s Lvl:%d Exp:%d/%d)%s", id,"POKEMOD_POKEMON", pname, temp, level, PlayersXP[id][PlayersActive[id]], g_isWild[id]>0?PokeLevels[level]/2:PokeLevels[level], moneytemp )
			}
		}
		else {
			if(Safari())
				iLen += formatex(message[iLen],(255-iLen),"%s (Skl:%s)%s", pname, temp, moneytemp )
			else
				iLen += formatex(message[iLen],(255-iLen),"%s (Skl:%s Lvl:%d)%s", pname, temp, level, moneytemp )
		}
	}

	//for some reason, in NS it flashes, so were going to make it last longer
	#if MOD==NS
		set_hudmessage(255,255,0, 0.00, PlayersSet[id][SET_HUD_Y]*0.01, 0, 0.1, 1.5, 0.1, 0.1, -1)
	#else
		set_hudmessage(255,255,0, 0.00, PlayersSet[id][SET_HUD_Y]*0.01, 0, 0.1, 1.0, 0.1, 0.1, -1)
	#endif

	ShowSyncHudMsg( id, HUDSYNC, message )

	#if MOD!=CS
		pokemon_statustext(id)
	#endif
 }
 
 ////////////////////////////////////
 //  Displays Personal Info Again  //
 ////////////////////////////////////
 public self_view(id)
 {
	if(Pokemod_Alive(id))
		fm_attach_view(id, id)
 }
 
 ///////////////////////////////////
 //  Every time someone connects  //
 ///////////////////////////////////
 public client_connect(id)
 {
	g_szIdentifier[id][0]='\0'
	if(PM_disabled==PM_OFF)
		return

	new i
	PlayersActive[id] = 0
	Pokemon[id] = 0
	for(i=0; i<MAX_POKES; i++){
		PlayersPoke[id][i] = 0
		PlayersXP[id][i] = 0
		PlayersLevels[id][i] = 0;
	}

	#if ALLOW_WILD==1
		if(get_ppcvar_num(pm_wild)>0)
			g_isWild[id] = 1
		else
			g_isWild[id] = 0
	#else
		g_isWild[id] = 0
	#endif
	#if POKERANK_SYSTEM==1
		PlayersCatch[id] = 0
	#endif

	money[id] = 0
	NoItems(id)
	for(i=0; i<MAX_ITYPE+3; i++){
		Open[id][i] = false
	}
	Pokemod_Connect(id,0);
	debugMessage(id,4,"Player", "connected.")
 }
 
 //////////////////////////////////////
 //  Every time someone disconnects  //
 //////////////////////////////////////
 public client_disconnect(id)
 {
 	canuse[id] = false
	interest[id] = 0
	
	if(PM_disabled!=PM_OFF) {
		if(SaveXPon())
			SaveXP(id)
		Pokemod_Connect(id,0);
		debugMessage(id,4,"Player", "disconnected")
	}

	g_szIdentifier[id][0]='\0'
 }
 
 ///////////////////////////////////
 //  Every time someone connects  //
 ///////////////////////////////////
 public client_putinserver(id)
 {
 	if(PM_disabled!=PM_OFF) {
		interest[id] = 0
		canuse[id] = false
		switch(get_cvar_num("bank_restrict"))
		{
			case 0:
			{
				canuse[id] = true
			}
			case 1:
			{
				if(access(id,ADMIN_CHAT))
					canuse[id] = true
				else
					canuse[id] = false
			}
			case 2:
			{
				canuse[id] = false
				new sid[35]
				sid=g_szIdentifier[id]

				new retstr[35],a,i
				while(read_file(allowfilepath,i,retstr,34,a))
				{
					if(equali(sid,retstr))
						canuse[id] = true
					i++
				}
			}
		}
	 
		if( PMon() )
			client_cmd(id,"play %s",SOUNDS[SND_THEME])

		debugMessage(id,2,"Player", "Data Load%s", LoadXP(id)?"ed Successfully":" Failed" );
	}
 }
 
#pragma reqlib "forumkey"
native GetForumKey(id,key[])
 
 ///////////////////////////////////////////
 //  When somone is officially authorized  //
 ///////////////////////////////////////////
public client_authorized(id) {
	get_user_authid(id,g_szIdentifier[id],34)

	if(!IsValidSteamID(g_szIdentifier[id])) {
		get_user_ip(id,g_szIdentifier[id],34,1)

		new ForumKey[35]

		GetForumKey(id,ForumKey)

		if(strcmp(ForumKey,"ERROR")) {
			client_cmd(id, "echo ")
			client_cmd(id, "echo \" Forum Key is valid, using it. \" ")
			client_cmd(id, "echo ")

			// convert IP to Key
			new iTimestamp
			static szPlayerForumValue[1024]
			new iPlayerForumResultCode = nvault_lookup(nvault_db,ForumKey,szPlayerForumValue,charsmax(szPlayerForumValue),iTimestamp)
			static szPlayerIPValue[1024]
			new iPlayerIPResultCode = nvault_lookup(nvault_db,g_szIdentifier[id],szPlayerIPValue,charsmax(szPlayerIPValue),iTimestamp)
			if(!iPlayerForumResultCode && iPlayerIPResultCode) {
				client_cmd(id, "echo ")
				client_cmd(id, "echo \" Your progress has been converted. \" ")
				client_cmd(id, "echo ")
				nvault_set(nvault_db,ForumKey,szPlayerIPValue)
			}

			g_szIdentifier[id][0]='\0'
			strcat(g_szIdentifier[id],ForumKey,34)
		} else {
			client_cmd(id, "echo ")
			client_cmd(id, "echo \" Forum Key not valid, using IP. \" ")
			client_cmd(id, "echo ")
		}
	} else {
		client_cmd(id, "echo ")
		client_cmd(id, "echo \" Valid SteamID detected, using it. \" ")
		client_cmd(id, "echo ")
	}
}
 
 ///////////////////////////////////////
 //  Stops people from buying things  //
 ///////////////////////////////////////
 public EndBuying()
	CanBuy = false
 
 ///////////////////////////////////////////////////
 //  Stops people from moving during freeze time  //
 ///////////////////////////////////////////////////
 public EndFreezeTime()
 {
	FreezeTime = false
	new players[32], pnum
	poke_get_players(players, pnum, PLAYER_ALIVE)
	for(new i=0; i<pnum; i++)
		SetSpeed(players[i])
 }
 