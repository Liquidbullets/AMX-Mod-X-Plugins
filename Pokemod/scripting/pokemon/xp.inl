
 /*
 // Use this to create the table in MySQL

 CREATE TABLE IF NOT EXISTS `pm_save` ( 
	`SAVE_KEY` varchar(34) binary NOT NULL default '',
	`XPS` varchar(255) binary NOT NULL default '',
	`LAST_PLAY_DATE` timestamp(14) NOT NULL,
	PRIMARY KEY  (`SAVE_KEY`)
 ) TYPE=MyISAM COMMENT='PokeMod Saving Table'; 
 
 // now it should work fine 
  
 //if using MySQL and are updating from v1.2.0 Beta J2 or lower, run these queries
 ALTER TABLE `pm_save` ADD `LAST_PLAY_DATE` timestamp(14) NOT NULL;
 ALTER TABLE `pm_save` MODIFY `XPS` varchar(255) binary NOT NULL default '';
 
 //if using MySQL and are updating from v1.2.0 Beta K2 or lower, run this query
 UPDATE `pm_save` SET `LAST_PLAY_DATE`=SYSDATE()
 
 */

 // This makes sure theres no gaps in PlayersPoke
 //		Example:
 //			From:	5 390 0 0 10 200 16 1000
 //			To:		5 390 10 200 16 1000 0 0
 public RefreshPoke(id)
 {
	if(Safari() || !id || !Pokemod_Connected(id))
		return

	new PlaceHolder[3][MAX_POKES]

	new i, p = 0;
	for(i = 0; i<MAX_ACTIVE_POKES; i++){
		if( ValidPoke( PlayersPoke[id][i] ) ){
			PlaceHolder[0][p] = PlayersPoke[id][i];
			PlaceHolder[1][p] = PlayersXP[id][i];
			PlaceHolder[2][p] = PlayersLevels[id][i];
			p++;
		}
	}
	for(i = p; i<MAX_ACTIVE_POKES; i++){
		PlayersPoke[id][i] = 0;
		PlayersXP[id][i] = 0;
		PlayersLevels[id][i] = 0;
	}

	p = MAX_ACTIVE_POKES;
	for(i = MAX_ACTIVE_POKES; i<MAX_POKES; i++){
		if( ValidPoke( PlayersPoke[id][i] ) ){
			PlaceHolder[0][p] = PlayersPoke[id][i];
			PlaceHolder[1][p] = PlayersXP[id][i];
			PlaceHolder[2][p] = PlayersLevels[id][i];
			p++;
		}
	}
	for(i = p; i<MAX_POKES; i++){
		PlayersPoke[id][i] = 0;
		PlayersXP[id][i] = 0;
		PlayersLevels[id][i] = 0;
	}

	for(i = 0; i<MAX_POKES; i++){
		PlayersPoke[id][i] = PlaceHolder[0][i];
		PlayersXP[id][i] = PlaceHolder[1][i];
		PlayersLevels[id][i] = PlaceHolder[2][i];
	}

	if(!SwitchPoke(id, Pokemon[id])){
		for(i=0; i<MAX_ACTIVE_POKES; i++){
			if(SwitchPoke(id, PlayersPoke[id][i]))
				break;
		}
	}
 }

 ///////////////////////////////////////////
 //  For MYSQL, find the database to use  //
 ///////////////////////////////////////////
 #if SAVE==SAVE_MYSQL 
 public adminCopyVault(id)
 {
	if (!(get_user_flags(id)&ADMIN_RCON ) && id!=0) return PLUGIN_HANDLED

	mySQLConnect()

	if ( gMySQL <= SQL_FAILED ) return PLUGIN_HANDLED

	//Turn Persistant on for this function
	gPersistentTemp = true

	// Open up the vault file - read line by line
	if ( !file_exists(g_VaultFile) ) return PLUGIN_HANDLED

	console_print(id,"Please wait while the XP data is copied")

	// Read through the file looking for shinfo entries
	new parm[2]
	parm[0] = id
	parm[1] = 0
	adminCopyVHelper(parm)

	//Rest of this is done in the helper function
	return PLUGIN_HANDLED
 }
 public adminCopyVHelper(parm[])
 {
	//Get data from parm
	new id = parm[0]
	new nextLine = parm[1]

	// Read through the file looking for shinfo entries
	new lengthRead
	new data[512], Right[401], Left[111]

	while ((nextLine = read_file(g_VaultFile,nextLine,data,511,lengthRead)) != 0) {
		if ( lengthRead > 0 && equali(data, "PokeMod", 7) ) {
			strbreak(data, Left, 110, Right, 400)
			replace(Left,110,"PokeMod-",EMPTY)
			replace_all(Left,34,"`","\\`")
			replace_all(Left,34,"'","\\'")
			writeData(0, Left, Right)
			if (nextLine % 200 == 0) {
				parm[1] = nextLine
				console_print(id,"Delaying a few seconds before copying the next set of entries")
				set_task(3.0, "adminCopyVHelper",0,parm,2)
				return
			}
		}
	}

	//If it makes it this far it must be done copying

	// OK now load the XP for any players playing
	load_all();

	console_print(id,"Saved XP has been copied from the vault to the MySQL Database Successfully")

	//Set Persistent back how it was
	gPersistentTemp = false

	//Closes the connection if set not to be persistent
	close_mysql()
 }
 public saving_init()
 {
	register_pokeadmincmd("vault_to_sql","adminCopyVault", ADMIN_RCON,"- Copies vault XP to MySQL, should only be done once.")

	// Verify the type of DBI module that is loaded is supported
	dbi_type(SQLtype, 15);
	if (equali(SQLtype, g_MySQL))
	{
		SQL_TYPE = SQL_MYSQL;
	}
	else if (equali(SQLtype, g_SQLite))
	{
		SQL_TYPE = SQL_SQLITE;
	}
	else
	{
		SQL_TYPE = SQL_NONE;
		debugMessage(_,_,"XP", "Unsupported database type found (%s), the supported databases are %s or %s", SQLtype, g_MySQL, g_SQLite);
		return;
	}

	debugMessage( 0,7,"XP", "MYSQL registering done" )
 }

 public mySQLConnect()
 {
	gPersistent = get_ppcvar_num(pm_mysql_persistent)

	if ( gMySQL >= SQL_OK ) {
		if (!gPersistent && !gPersistentTemp) close_mysql()
		else return
	}

	new host[64],user[32],pass[32],db[32],error[128]

	get_ppcvar_string(pm_mysql_host,host,63)
	get_ppcvar_string(pm_mysql_user,user,31)
	get_ppcvar_string(pm_mysql_pass,pass,31)
	get_ppcvar_string(pm_mysql_db,db,31)

	if ( !strlen(host) && !strlen(user) && !strlen(db) ) {
		get_cvar_string("amx_mysql_host",host,63)
		get_cvar_string("amx_mysql_user",user,31)
		get_cvar_string("amx_mysql_pass",pass,31)
		get_cvar_string("amx_mysql_db",db,31)
	}

	// Set a default DB if it's SQLite and the user didn't supply one
	if ( SQL_TYPE == SQL_SQLITE && strlen(db) < 1 )
		copy( db, 127, "addons/amxmodx/data/amxx.db" );

	gMySQL = dbi_connect(host,user,pass,db,error,127)
	if (gMySQL <= SQL_FAILED) {
		debugMessage(0,0,"XP", "MySQL connect error: '%s' (%s,%s,%s)",error,host,user,db)
		return
	}
	//Only try to create the tables once
	else if (CreateTables) {
		new sql[512]
		new Result:retval

		if ( SQL_TYPE == SQL_SQLITE ){
			if ( !sqlite_table_exists( gMySQL, "pm_save" ) ){

				copy(sql, 511, "CREATE TABLE `pm_save` ( `SAVE_KEY` varchar(34) binary NOT NULL default '', `XPS` varchar(255) binary NOT NULL default '', `LAST_PLAY_DATE` timestamp(14) NOT NULL, PRIMARY KEY  (`SAVE_KEY`) ) TYPE=MyISAM COMMENT='PokeMod Saving Table'")
				retval = dbi_query(gMySQL,sql)
				if (retval <= RESULT_FAILED) {
					dbi_error(gMySQL,error,127)
					debugMessage(0,0,"XP", "Error Making Tables: '%s' - '%s'", error, sql)
					return
				}

				if ( SQL_TYPE == SQL_SQLITE ) 
				{
					new IntegrityCheck[64];
					copy(sql, 511, "PRAGMA integrity_check");
					new Result:res = dbi_query(gMySQL, sql);
					
					// Check for an error
					if ( res < RESULT_NONE )
					{
						dbi_error(gMySQL,error,127)
						debugMessage(0,0,"XP", "Error Making Tables: '%s' - '%s'", error, sql)
						return;
					}
					
					// Get the integrity check value
					while ( res && dbi_nextrow(res) > 0 )
					{
						dbi_result(res, "integrity_check", IntegrityCheck, 63);
					}

					// Free the result
					dbi_free_result(res);

					// Check to make sure the integrity check passed
					if ( !equali(IntegrityCheck, "ok") )
					{
						// Should we disable saving here?
						debugMessage(_,_,"XP", "SQL Lite integrity check failed");
						return;
					}
						
					// Do some synchronous crap
					format(sql, 511, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF);
					dbi_query(gMySQL, sql);
				}
			}
		}
		else {
			copy(sql, 511, "CREATE TABLE IF NOT EXISTS `pm_save` ( `SAVE_KEY` varchar(34) binary NOT NULL default '', `XPS` varchar(255) binary NOT NULL default '', `LAST_PLAY_DATE` timestamp(14) NOT NULL, PRIMARY KEY  (`SAVE_KEY`) ) TYPE=MyISAM COMMENT='PokeMod Saving Table'")
			retval = dbi_query(gMySQL,sql)
			if (retval <= RESULT_FAILED) {
				dbi_error(gMySQL,error,127)
				debugMessage(0,0,"XP", "Error Making Tables: '%s' - '%s'", error, sql)
				return
			}
		}
		CreateTables = false
	}
 }
 writeData(x, key[], xps_to_save[])
 {
	mySQLConnect()
	if ( gMySQL <= SQL_FAILED ) return

	debugMessage( x, 8,"XP", "Trying to save XP data: '%s' - '%s'", key, xps_to_save)

	new error[128],sql[512]
	new Result:retval, Result:retvalins

	//Thanks to HC for help with writing more efficient queries
	//Check if this user has an entry already, if not make one
	formatex(sql, 511, "SELECT * FROM `pm_save` WHERE `SAVE_KEY` = '%s'", key)
	retval = dbi_query(gMySQL,sql)

	if (retval <= RESULT_FAILED) {
		dbi_error(gMySQL,error,127)
		debugMessage(0,0,"XP", "Error Querying MySQL DB for %s: '%s' - '%s'", key, error, sql)
		return
	}
	else if (!dbi_nextrow(retval)) {
		formatex(sql, 511, "INSERT INTO `pm_save` (SAVE_KEY) VALUES ('%s')", key )
		retvalins = dbi_query(gMySQL,sql)
		if (retvalins <= RESULT_FAILED) {
			dbi_error(gMySQL,error,127)
			debugMessage(0,0,"XP", "Error Writing MySQL XP for %s: '%s' - '%s'", key, error, sql)
			return
		}
	}

	if (retval >= RESULT_OK) dbi_free_result(retval)

	//Update users entry with current data
	formatex(sql, 511, "UPDATE `pm_save` SET `XPS`='%s' WHERE (SAVE_KEY='%s')",xps_to_save,key)
	retval = dbi_query(gMySQL,sql)
	if (retval <= RESULT_FAILED) {
		dbi_error(gMySQL,error,127)
		debugMessage(0,0,"XP", "Error Writing MySQL XP for %s: '%s' - '%s'", key, error, sql)
		return
	}

	//Closes the connection if set not to be persistent
	close_mysql()
 }
 public close_mysql()
 {
	if (gMySQL <= SQL_FAILED || gPersistent || gPersistentTemp) return
	dbi_close(gMySQL)
 }
 public saving_end()
 {
	if ( gMySQL <= SQL_FAILED ) return
	dbi_close(gMySQL)
 }
 #endif
 #if SAVE==SAVE_VAULT
 writeData(x, key[], xps_to_save[])
 {
	debugMessage(x, 8,"XP", "Trying to save XP data: '%s' - '%s'", key, xps_to_save)

	new vaultSaveKey[40]
	//Make the key to save to
	formatex(vaultSaveKey,39,"PokeMod-%s",key)
	set_vaultdata(vaultSaveKey, xps_to_save)
 }
 #endif
 #if SAVE==SAVE_NVAULT
 public saving_init()
 {
	register_pokeadmincmd("vault_to_nvault","adminCopyVault", ADMIN_RCON,"- Copies vault XP to NVault, should only be done once.")

	nvault_db = nvault_open("pm_save")

	debugMessage( 0,7,"XP", "NVault registering done" )
 }
 writeData(x, key[], xps_to_save[])
 {
	debugMessage( x, 8,"XP", "Trying to save XP data: '%s' - '%s'", key, xps_to_save)

	new vaultSaveKey[40]
	//Make the key to save to
	formatex(vaultSaveKey,39,"PokeMod-%s",key)
	nvault_set(nvault_db, key, xps_to_save)
 }
 public adminCopyVault(id)
 {
	if (!(get_user_flags(id)&ADMIN_RCON ) && id!=0) return PLUGIN_HANDLED

	// Open up the vault file - read line by line
	if ( !file_exists(g_VaultFile) ) return PLUGIN_HANDLED

	console_print(id,"Please wait while the XP data is copied")

	// Read through the file looking for shinfo entries
	new parm[2]
	parm[0] = id
	parm[1] = 0
	adminCopyVHelper(parm)

	//Rest of this is done in the helper function
	return PLUGIN_HANDLED
 }
 public adminCopyVHelper(parm[])
 {
	//Get data from parm
	new id = parm[0]
	new nextLine = parm[1]

	// Read through the file looking for shinfo entries
	new lengthRead
	new data[512], Right[401], Left[111]

	while ((nextLine = read_file(g_VaultFile,nextLine,data,511,lengthRead)) != 0) {
		if ( lengthRead > 0 && equali(data, "PokeMod", 7) ) {
			strbreak(data, Left, 110, Right, 400)
			replace(Left,110,"PokeMod-",EMPTY)
			writeData(0, Left, Right)
			if (nextLine % 200 == 0) {
				parm[1] = nextLine
				console_print(id,"Delaying a few seconds before copying the next set of entries")
				set_task(3.0, "adminCopyVHelper",0,parm,2)
				return
			}
		}
	}

	//If it makes it this far it must be done copying

	// OK now load the XP for any players playing
	load_all();

	console_print(id,"Saved XP has been copied from the vault to the NVault Database Successfully")
 }
 #endif
 
 public LoadXP(id)
 {
	if( !PM_Loaded ){
		Pokemod_Connect(id,0);
		return false;
	}

	if( g_isWild[id] > 0 )
		return true
	if( !SaveXPon() )
		return true

	new key[35]
	if( getSaveKey(id, key, true) )
	{
		#if SAVE==SAVE_NVAULT
			new Right[1501]
			new vaultSaveKey[40], time
			formatex(vaultSaveKey,39,"PokeMod-%s",key)
			nvault_lookup(nvault_db, key, Right, 1500, time)
		#endif
		#if SAVE==SAVE_VAULT
			new Right[1501]
			new vaultSaveKey[40]
			formatex(vaultSaveKey,39,"PokeMod-%s",key)
			get_vaultdata(vaultSaveKey, Right, 1500)
		#endif
		#if SAVE==SAVE_MYSQL
			new Right[1501]
			new sql[512], error[128]
			new Result:retvalxp

			mySQLConnect()
			if ( gMySQL <= SQL_FAILED ) return false

			formatex(sql, 511, "SELECT `XPS` FROM `pm_save` WHERE `SAVE_KEY` = '%s'", key)
			retvalxp = dbi_query(gMySQL,sql)

			if(retvalxp <= RESULT_FAILED){
				dbi_error(gMySQL,error,127)
				debugMessage(id,0,"XP", "Error Querying MySQL DB for %s: '%s' - '%s'", key, error, sql)

				//Closes the connection if set not to be persistent
				close_mysql()

				return false
			}
			else if (!dbi_nextrow(retvalxp)) {
				dbi_error(gMySQL,error,127)
				debugMessage(id,3,"XP", "No Saved XP Loaded for %s: '%s'", key, error)

				if (retvalxp >= RESULT_OK) dbi_free_result(retvalxp)

				//Closes the connection if set not to be persistent
				close_mysql()

				Pokemod_Connect(id,-1);
				return true
			}
			dbi_field(retvalxp, 1, Right, 500)
		#endif

			return HandleLoad(id,Right)
	}
	Pokemod_Connect(id,0);
	return false;
 }
 HandleLoad(id, Right[1501])
 {
	debugMessage(id,8,"XP", "loading %s",Right );

	//we don't have any saved data
	if(equali(Right,EMPTY))
		return NewPlayerSettings(id);

	new Left[51], i;
	for( i = 0; i<MAX_POKES; i++)
	{
		strbreak(Right, Left, 50, Right, 1500)
		if( equal(Left, COLON, 1) ) break	//stop if this is the end of the data we want to look at

		PlayersPoke[id][i] = str_to_num(Left)

		strbreak(Right, Left, 50, Right, 1500)
		if( equal(Left, COLON, 1) ) break	//stop if this is the end of the data we want to look at

		PlayersXP[id][i] = str_to_num(Left)
		PlayersLevel(id, i, true);
	}

	while( !equal(Left, COLON, 1) && !equali(Right,EMPTY)){ //if we didn't stop because we found a :, lets get the next one
		strbreak(Right, Left, 50, Right, 1500)
	}

	//we ran out of data
	if(equali(Right,EMPTY))
		return NewPlayerSettings(id, true);

	strbreak(Right, Left, 50, Right, 1500)
	if( !equal(Left, COLON, 1) ){	//we found the time, and not the next seperator
		PlayersTime[id] = str_to_num(Left)
	}

	while( !equal(Left, COLON, 1)  && !equali(Right,EMPTY)) //if we didn't stop because we found a :, lets get the next one
		strbreak(Right, Left, 50, Right, 1500)

	//strbreak(Right, Left, 50, Right, 1500)
	if( equal(Left, COLON, 1) ){	//we found the seperator, now this will break it down in here
		for( i = 0; i<MAX_SETS; i++)
		{
			strbreak(Right, Left, 50, Right, 1500)
			if( equal(Left, COLON, 1) ) break	//stop if this is the end of the data we want to look at

			PlayersSet[id][i] = str_to_num(Left)
		}
	}

	while( !equal(Left, COLON, 1) && !equali(Right,EMPTY)) //if we didn't stop because we found a :, lets get the next one
		strbreak(Right, Left, 50, Right, 1500)

	strbreak(Right, Left, 50, Right, 1500)
	if( !equal(Left, COLON, 1) ){	//we found their catches
		PlayersCatch[id] = str_to_num(Left)
	}

	if( is_user_bot(id) )
		BotSelect(id);

	return ConnectPlayer(id)
 }
 NewPlayerSettings(id, bool:had_pokemon=false)
 {
	PlayersSet[id][SET_HUD_Y] = -4;
	if( !had_pokemon )
		Pokemod_Connect(id,-1);
	else
		return ConnectPlayer(id);
	return true;
 }
 ConnectPlayer(id)
 {
	Pokemod_Connect(id,1);
	RefreshPoke(id)
	return true
 }
 
 public load_all()
 {
	for( new x=0; x < MAX_PLAYERS; x++ )
		LoadXP(x);
	#if POKERANK_SYSTEM==1
		LoadRanks()
	#endif
 }
 
 public save_all()
 {
	if( !PM_Loaded )
		return;

	poke_print(0, print_center, "%s Saving All Data",PREFIX);
	new players[MAX_PLAYERS], pnum;
	poke_get_players(players, pnum);
	for( new i=0; i<pnum; i++)
		set_task( 1.5*(i+1), "delayed_save", players[i]);

	#if POKERANK_SYSTEM==1
		SaveRanks();
	#endif
 }
 public delayed_save(id)
	SaveXP(id, -1);

 SaveXP(id, forced=1)
 {
	if(g_isWild[id]>0)
		return false;

	static key[35]
	static Float:last_save[MAX_PLAYERS+1], Float:time_left, Float:current_time;

	if( id ){
		current_time = get_gametime();
		time_left = current_time - last_save[id];

		if( forced < 1 && time_left < SAVE_SPAM){
			if( !forced )
				poke_printlang(id,"POKEMOD_SAVESPAM", SAVE_SPAM-time_left);
			return false;
		}
		last_save[id] = current_time;
	}

	if( Pokemod_Connected(id) && getSaveKey(id, key) && SaveXPon() )
	{
		RefreshPoke(id)

		new Data[1501], iLen = 0, i

		//save their pokemon and xp
		for(i = 0; i<MAX_POKES; i++)
			iLen += formatex(Data[iLen],(1501-iLen),"%d %d ", PlayersPoke[id][i], PlayersXP[id][i])

		//save the time
		iLen += copy(Data[iLen],(1501-iLen),": ")
		iLen += formatex(Data[iLen],(1501-iLen),"%d ",get_systime())
		iLen += copy(Data[iLen],(1501-iLen),": ")

		//save their settings
		for(i = 0; i<MAX_SETS; i++)
			iLen += formatex(Data[iLen],(1501-iLen),"%d ", PlayersSet[id][i])

		//save their pokerank crap
		iLen += formatex(Data[iLen],(1501-iLen),": %d",PlayersCatch[id])

		writeData(id, key, Data)

		debugMessage(id,5,"XP", "saved data with save key %s", key)
		return true;
	}
	return false;
 }

 #if SAVE_ID != 0
 public client_infochanged(id)
 {
	new savekey[35], newname[NAME_LENGTH];
	getSaveKey(id, savekey);
	get_user_info(id, "name", newname, NAME_LENGTH-1);
	if( !equal(savekey, newname) ){
		SaveXP(id);
		LoadXP(id);
	}
 }
 #endif
 //thanks to SHmod
 getSaveKey(id, key[35]="", bool:update=false)
 {
	static player_key[MAX_PLAYERS+1][35];
	if( update ) {
		#if SAVE_ID == 0
		if(is_user_bot(id)) {
			new botname[32]
			get_user_name(id,botname,31)

			//Get Rid of BOT Tag

			//PODBot
			replace(botname,31,"[POD]",EMPTY)
			replace(botname,31,"[P*D]",EMPTY)
			replace(botname,31,"[P0D]",EMPTY)

			//CZ Bots
			replace(botname,31,"[BOT] ",EMPTY)

			//Attempt to get rid of the skill tag so we save with bots true name
			new lastchar = strlen(botname) - 1
			if ( equal(botname[lastchar],")",1) ) {
				new x
				for ( x = lastchar - 1; x > 0; x--) {
					if ( equal(botname[x],"(",1) ) {
						botname[x - 1] = 0
						break
					}
					if ( !isdigit(botname[x]) ) break
				}
			}
			if (strlen(botname) > 0 ) {
				#if SAVE==SAVE_MYSQL
				replace_all(botname,31,"`","\\`")
				replace_all(botname,31,"'","\\'")
				#endif

				replace_all(botname,31," ","_")
				formatex( player_key[id], 34, "[BOT]%s", botname )
			}
		}
		//Hack for STEAM's retardedness with listen servers
		else if (!is_dedicated_server() && id == 1) {
			copy( player_key[id], 34, "loopback" )
		} else {
			player_key[id]=g_szIdentifier[id]
		}
		#else
		get_user_name( id, player_key[id], 34 )
		#endif
		debugMessage(id,5,"XP", "updated savekey");
	}

	//Check to make sure we got something useable
	if( equali(player_key[id], "STEAM_ID_PENDING") || equali(player_key[id], EMPTY) ){
		debugMessage(id,5,"XP", "does not have a valid savekey ('%s')", player_key[id]);
		return false;
	}

	copy( key, 34, player_key[id] );
	return true;
 }
 
 ///////////////////////////
 //  Levels a pokemon up  //
 ///////////////////////////
 LevelUpPokemon(id, num, levels=1, take_item=MAX_ITEMS)
 {
	new pastlevel = PlayersLevel(id, num)
	new pname[NAME_LENGTH]
	PokeToName(PlayersPoke[id][num], pname, NAME_LENGTH)

	if( take_item != MAX_ITEMS ){
		if(pastlevel == TotalLevels){
			poke_printlang(id,"POKEMOD_MAXLEVEL", pname)
			return false
		}
		PlayersItem[id][take_item]--
	}

	new newlevel = pastlevel+levels
	if(newlevel > TotalLevels)
		newlevel = TotalLevels
	PlayersXP[id][num] = PokeLevels[newlevel-1]
	PlayersLevels[id][num] = newlevel;
	poke_printlang(id,"POKEMOD_NEWLEVEL", pname,newlevel)
	Check_Evolve(id, PlayersPoke[id][num], newlevel)
	return true

 }
 
 ///////////////////////////////////////
 //  Returns what level a players is  //
 ///////////////////////////////////////
 PlayersLevel(id, num=-1, bool:update=false )
 {
	if(0>id || id>MAX_PLAYERS)
		return 0;

	if(Safari())
		return SafariLevel(Pokemon[id]);

	if( num == -1 )
		num = PlayersActive[id];

	if( !ValidSlot(num) )
		return 0;

	static i;
	if( update ){
		i = 1;
		if(g_isWild[id]<1){
			while(PlayersXP[id][num]>=PokeLevels[i]){
				i++;
				if(i==TotalLevels)
					break;
			}
		}
		else{
			while(PlayersXP[id][num]>=PokeLevels[i]/2){
				i++;
				if(i==TotalLevels)
					break;
			}
		}
		PlayersLevels[id][num] = i;
	}
	else i = PlayersLevels[id][num];

	#if RARE_CANDY_MODE == 1 || RARE_CANDY_MODE == 3
		static k;
		for(k=0; k<ItemsCreated; k++)
			if( ItemInfo[k] & II_LEVEL )
				i += PlayersItem[id][k];
	#endif

	return i;
 }
 RefreshLevels(id)
 {
	for(new i=0; i<MAX_POKES; i++)
		PlayersLevel(id, i, true);
 }
 
 /////////////////////////////////////////////
 //  Returns what players average level is  //
 /////////////////////////////////////////////
 AverageLevel(id)
 {
	if(Safari())
		return SafariLevel(Pokemon[id]);
	if(g_isWild[id]>0)
		return PlayersLevel(id, 0);

	new pokes, total
	for(new i = 0; i<MAX_ACTIVE_POKES; i++)
	{
		if( ValidPoke(PlayersPoke[id][i]) ){
			pokes++;
			total += PlayersLevel(id, i);
		}
	}
	if(pokes){
		new average = total/pokes;
		return average;
	}

	return 0
 }

 /////////////////////////////////////
 //  Adds a pokemon to playerspoke  //
 /////////////////////////////////////
 bool:AddPoke(id, pid, xp=1, giver=0, CHECK_POKES:checker=CHECK_ACTIVE)
 {
	RefreshPoke(id);

	if(!ValidPoke(pid) || AvailableSlot(id, pid, checker)!=AS_OK || Safari())
		return false;

	new check_rcon = RCON_POKEADMINS;
	if( check_rcon && giver && PokeInfo[pid] & PI_SPECIAL && !(get_user_flags(giver)&ADMIN_RCON) )
		return false;

	if(!xp)
		xp = 1;

	new i = Slot(id,_,checker);

	PlayersPoke[id][i] = pid;
	PlayersXP[id][i] = xp;
	PlayersLevel(id, i, true);

	if( i >= MAX_ACTIVE_POKES )
		poke_printlang(id,"POKEMOD_PCADDSENT", PokeToName(pid) );

	ShowPokeInfo(id, pid);

	SaveXP(id);

	return true;
 }
 
 ////////////////
 //  Gives XP  //
 ////////////////
 GiveXP(id, poke, amount)
 {
	if(Safari())
		return
	if( !ValidSlot(poke) || !ValidPoke(PlayersPoke[id][poke]) )
		return
	new oldlevel, newlevel

	oldlevel = PlayersLevel(id, poke)
	PlayersXP[id][poke] += amount

	if(PlayersXP[id][poke]<1)
		PlayersXP[id][poke] = 1

	newlevel = PlayersLevel(id, poke, true)

	if(oldlevel<newlevel){
		new oldmax = PlayerStat[id][STAT_MAXHP]
		new name[32]
		get_user_name(id, name, 31)
		poke_printlang(LANG_PLAYER,"POKEMOD_LEVELUP", name,PokeToName(PlayersPoke[id][poke]),newlevel)

		//ding for level up
		poke_sound(id, CHAN_AUTO, SND_BELL)

		if(Pokemod_Alive(id)){
			get_user_origin(id, origin[id])
			//sprites to show level up (only once)
			message_begin(MSG_BROADCAST ,SVC_TEMPENTITY) //message begin
			write_byte(TE_LARGEFUNNEL)
			write_coord(origin[id][0]) // funnel position
			write_coord(origin[id][1])
			write_coord(origin[id][2]-32)
			write_short(SPRITE_INDEX[SPR_MUZZLEFLASH1]) // sprite index
			write_short(8) // flags
			message_end()
		}

		Check_Evolve(id, PlayersPoke[id][poke], newlevel)

		#if POKERANK_SYSTEM==1
			PlayersCatch[id] += newlevel

			CheckMaxHP(id)
			if(oldmax!=PlayerStat[id][STAT_MAXHP]){
				poke_printlang(id,"POKEMOD_NEWHPRANK", PlayerStat[id][STAT_MAXHP],PlayersCatch[id])
			}
			else
				poke_printlang(id,"POKEMOD_NEWRANK", PlayersCatch[id])
		#else
			CheckMaxHP(id)
			if(oldmax!=PlayerStat[id][STAT_MAXHP]){
				poke_printlang(id,"POKEMOD_NEWHP", PlayerStat[id][STAT_MAXHP])
			}
		#endif
	}
	else if(oldlevel>newlevel){
		new oldmax = PlayerStat[id][STAT_MAXHP]
		new name[32]
		get_user_name(id, name, 31)
		poke_printlang(id,"POKEMOD_LEVELDOWN", name,PokeToName(PlayersPoke[id][poke]),newlevel)

		#if POKERANK_SYSTEM==1
			PlayersCatch[id] -= oldlevel

			CheckMaxHP(id)
			if(oldmax!=PlayerStat[id][STAT_MAXHP]){
				poke_printlang(id,"POKEMOD_NEWHPRANKL", PlayerStat[id][STAT_MAXHP],PlayersCatch[id])
			}
			else
				poke_printlang(id,"POKEMOD_NEWRANKL", PlayersCatch[id])
		#else
			CheckMaxHP(id)
			if(oldmax!=PlayerStat[id][STAT_MAXHP]){
				poke_printlang(id,"POKEMOD_NEWHPL", PlayerStat[id][STAT_MAXHP])
			}
		#endif

		#if POKEMON_UNEVOLVE==1
			Check_UnEvolve(id, PlayersPoke[id][poke])
		#endif
	}

	#if POKERANK_SYSTEM==1
	if(oldlevel != newlevel){
		if(SpecialRank(id)!=-1)	//update their thing, and check if they went higher
			UpdateSpecialRank(id)
		else
			CheckUpdateRank(id) //only check if they went higher than the last person in Elite Four
	}
	#endif
 }
 
 /////////////////////
 //  Gives Kill XP  //
 /////////////////////
 KillXP(attacker, victim)
 {
	if(attacker==victim)
		return
	new xp_to_give = PlayersLevel(victim)
	if( Pokemod_FF(victim, attacker) ){
		GiveXP(attacker, PlayersActive[attacker], -1*xp_to_give)
		poke_printlang(attacker,"POKEMOD_KILLMATE", xp_to_give)
	}
	else
		GiveXP(attacker, PlayersActive[attacker], xp_to_give)
 }

 /////////////////////////
 //  Adds level to num  //
 /////////////////////////
 AddLvl(id, num)
 {
	num += PlayersLevel(id);
	return true;
 }

 ///////////////////////////
 //  Adds level to float  //
 ///////////////////////////
 f_AddLvl(id, Float:num)
 {
	num += PlayersLevel(id);
	return true;
 }
 
 ////////////////////////////////
 //  Switches a users pokemon  //
 ////////////////////////////////
 SwitchPoke(id, pid, bool:force=true)
 {
	if( !ValidPoke(pid) )
		return false;

	if( pid == Pokemon[id] )
		return true;

	#if ADMIN_SPECIAL == 1
	if( PokeInfo[pid] & PI_SPECIAL && !poke_access(id) ){
		poke_printlang(id,"POKEMOD_RESTRICTUSE", PokeToName(pid));
		return false;
	}
	#endif

	if(Safari()){
		if(SafariLevel(pid)){
			ResetSkills(id, false)
			Pokemon[id] = pid
			SetSpeed(id)
			return true
		}
		return false
	}

	#if CHANGE_POKES > 0
		if( !force && ChangedPoke[id] < 1 && Pokemod_Alive(id) ){
			#if CHANGE_POKES > 1
			poke_printlang(id,"POKEMOD_CHANGEMORE", CHANGE_POKES)
			#else
			poke_printlang(id,"POKEMOD_CHANGEONCE")
			#endif
			return false
		}
	#else
		#if CHANGE_POKES == -1
		if( is_user_bot(id) && ChangedPoke[id] == 0 )
			return false
		#endif
	#endif

	for(new i=0; i<MAX_ACTIVE_POKES; i++){
		if(PlayersPoke[id][i]==pid){

			// reset skills that were in use
			ResetSkills(id, false)

			Pokemon[id] = pid
			PlayersActive[id] = i

			// reset skills to be in use
			ResetSkills(id, false)

			#if CHANGE_POKES > 0
			ChangedPoke[id]--
			#endif

			if( !force && CHANGE_DELAY > 0.0 )
				bind_used(id, _, CHANGE_DELAY);

			return true
		}
	}
	return false
 }
 
 ////////////////////////////////
 //  Releases a users pokemon  //
 ////////////////////////////////
 ReleasePoke(id, pid, bool:allow_cancel=true)
 {
	if( !ValidPoke(pid) )
		return
	new slot = Slot(id, pid, CHECK_ALL)
	if( ValidSlot(slot) ){
		if(allow_cancel){
			if( slot == PlayersActive[id] ){
				poke_printlang(id,"POKEMOD_RELACTIVE");
				return;
			}
			if( task_exists(id+RELEASE_TASK) ){
				poke_printlang(id,"POKEMOD_RELONE");
				return;
			}

			new parm[2]
			parm[0] = id
			parm[1] = pid
			set_task(RELEASE_TIME/2, "ReleaseRemind", id+RELEASE2_TASK, parm, 2)
			set_task(RELEASE_TIME, "FinalRelease", id+RELEASE_TASK, parm, 2)
			poke_printlang(id,"POKEMOD_RELINIT",RELEASE_TIME,PokeToName(pid))
		}
		else{
			PlayersPoke[id][slot] = 0
			PlayersXP[id][slot] = 0
			PlayersLevels[id][slot] = 0

			SaveXP(id)

			if( !SwitchPoke(id, Pokemon[id]) )
				SwitchPoke(id, PlayersPoke[id][PlayersActive[id]])
		}
	}
 }
 public ReleaseRemind(parm[])
 {
	poke_printlang(parm[0],"POKEMOD_RELREMIND",PokeToName(parm[1]),RELEASE_TIME/2)
 }
 public FinalRelease(parm[])
 {
	new id = parm[0], poke = parm[1];
	new slot = Slot(id, poke, CHECK_ALL)
	if( ValidSlot(slot) ){
		poke_printlang(id,"POKEMOD_RELFINAL",PokeToName(poke))
		PlayersPoke[id][slot] = 0
		PlayersXP[id][slot] = 0
		PlayersLevels[id][slot] = 0
		SaveXP(id)
	}
 }
 
 ////////////////////////////
 //  Trades users pokemon  //
 ////////////////////////////
 TradePoke(id, pid, nid)
 {
	if( !Pokemod_Connected(id) || !ValidPoke(pid) || !Pokemod_Connected(nid) )
		return false

	#if RCON_POKEADMINS == 1
	if( PokeInfo[pid] & PI_SPECIAL && !(get_user_flags(id)&ADMIN_RCON) ){
		poke_printlang(id,"POKEMOD_NOGIVESPEC");
		return false;
	}
	#endif

	new pnum = HasPokemon(id, pid, CHECK_ALL)
	if( pnum >= 0 ){
		if( pnum == PlayersActive[id] ){
			poke_printlang(id,"POKEMOD_GIVACTIVE");
			return false;
		}

		new name[32], Nname[32], pname[NAME_LENGTH]
		get_user_name(id, name, 31)
		get_user_name(nid, Nname, 31)
		PokeToName(pid, pname, NAME_LENGTH)

		switch(AvailableSlot(nid, pid))
		{
			case AS_OK:{
				new i = Slot(nid)

				PlayersPoke[nid][i] = PlayersPoke[id][pnum]
				PlayersXP[nid][i] = PlayersXP[id][pnum]
				PlayersLevels[nid][i] = PlayersLevels[id][pnum]

				#if POKE_POPULATION!=2
					ReleasePoke(id, pid, false);
				#endif

				poke_printlang(nid,"POKEMOD_GAVE", name, pname)
				poke_printlang(id,"POKEMOD_GAVEOK", Nname, pname)
				Check_Trade_Evolve(nid, pid);
				return true;
			}
			case AS_NOGOOD:
				poke_printlang(id,"POKEMOD_GIVNOGOOD")
			case AS_NOROOM:
				poke_printlang(id,"POKEMOD_GIVNOROOM", Nname)
			case AS_DUPLICATE:
				poke_printlang(id,"POKEMOD_GIVDUP", Nname, pname)
		}
	}
	else
		poke_printlang(id,"POKEMOD_GIVNOHAS", PokeToName(pid))
	return false
 }
 
 ////////////////////////////////
 //  Gets rid of Pokemod info  /////////////  TY SHmod  ///////////
 ////////////////////////////////
 cleanXP(bool:clear_all=false)
 {
	#if SAVE==SAVE_VAULT
		// Open up the vault file - read line by line
		// Use vault to delete to any data over a certain age
		if ( !file_exists(g_VaultFile) ) return

		// Read through the file looking for pokemod entries
		new lengthRead, nextLine = 0, last_time = (get_ppcvar_num(pm_xpsavedays) * SECONDS_IN_DAY)
		new data[1501], key[35]

		// This is an imperfect clean - but gets the job done
		// Vault needs to have a way to walk it
		// Deleting a Key can actually reorder the vault.ini file
		while ((nextLine = read_file(g_VaultFile,nextLine,data,1500,lengthRead)) != 0) {
			if ( lengthRead > 0 && equali(data, "PokeMod", 7) ) {
				parse(data, key, 34)
				if ( clear_all ) {
					remove_vaultdata( key )
					// since remove_vaultdata can reorder the whole file - we will go back some lines
					nextLine = max(nextLine - 1, 0)
				}
				else {
					HandleLoad(0, data)
					if( PlayersTime[0] + last_time < get_systime() ){
						remove_vaultdata( key )
						// since remove_vaultdata can reorder the whole file - we will go back some lines
						nextLine = max(nextLine - 1, 0)
					}
				}
			}
		}
	#endif
	#if SAVE==SAVE_NVAULT
		if(nvault_db) {
			if(clear_all) {
				nvault_prune(nvault_db, 0, get_systime())
			} else {
				nvault_prune(nvault_db, 0, get_systime() - (SECONDS_IN_DAY*get_ppcvar_num(pm_xpsavedays)) )
			}
		}
	#endif
	#if SAVE==SAVE_MYSQL
		new error[127], sql[255]
		new Result:retval

		mySQLConnect()
		if( gMySQL <= SQL_FAILED ) return

		if(clear_all){
			copy(sql, 254, "TRUNCATE TABLE `pm_save`")
			retval = dbi_query(gMySQL,sql)
			if (retval <= RESULT_FAILED) {
				dbi_error(gMySQL,error,127)
				debugMessage(0,0,"XP", "Couldn't Erase XP: '%s' - '%s' - %d", error, sql, gMySQL)
				return
			}
		}
		else{

			if ( SQL_TYPE == SQL_SQLITE )
				formatex( sql, 254, "DELETE FROM `pm_save` WHERE ((julianday(`time`) + %d) < julianday('now'))", get_ppcvar_num(pm_xpsavedays) );
			else
				formatex( sql, 254, "DELETE FROM `pm_save` WHERE pm_save.`LAST_PLAY_DATE` < (SYSDATE() - INTERVAL '%d' DAY)", get_ppcvar_num(pm_xpsavedays) )

			retval = dbi_query(gMySQL,sql)
			if (retval <= RESULT_FAILED) {
				dbi_error(gMySQL,error,127)
				debugMessage(0,0,"XP", "Couldn't Erase XP: '%s' - '%s' - %d", error, sql, gMySQL)
				return
			}

			// Vacuum the SQL LITE DB Table
			if ( SQL_TYPE == SQL_SQLITE )
			{
				copy( sql, 254, "VACUUM `pm_save`" );
				dbi_query( gMySQL, sql );
			}
		}

		//Closes the connection if set not to be persistent
		close_mysql()
	#endif
 }
 
 SafariLevel(poke)
	return SafariLevels[poke]
 
 public SetSafariLevels()
 {
	new PokeFile[128];
	Poke_FileName( F_Safari, PokeFile);

	new Data[256]
	new Left[NAME_LENGTH], Right[151], LeftN, RightN

	new f = fopen(PokeFile, "rt");
	while( !feof(f) ){

		fgets(f, Data, 255);
		if( !ValidData( Data ) )
			continue;

		strbreak(Data, Left, NAME_LENGTH-1, Right, 150)
		LeftN = NameToPoke(Left)
		if( !LeftN )
			continue

		strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
		RightN = str_to_num(Left)
		if( RightN ){
			SafariLevels[LeftN] = RightN
		}
	}
	fclose(f);
 }
 
 #if ALLOW_WILD==1
 AssignWild(id, bool:first_time=false)
 {
	new poke, special_chance = random_num(1, 100);
	do poke = random_num(1,PokesCreated);
	while(	PokeDisabled(poke)
	||		PokeSkill[poke] == -1
	||		(PokeInfo[poke] & PI_SPECIAL && special_chance > POKEBALL_SPECIAL)
	||		PokeInfo[poke] & PI_NOFIND
	)

	if(first_time){
		new name[32]
		get_user_name(id, name, 31)

		poke_printlang(LANG_PLAYER,"POKEMOD_WILDLEAVE", name)
		SaveXP(id)

		for(new i=0; i<MAX_POKES; i++){
			PlayersPoke[id][i] = 0
			PlayersXP[id][i] = 0
			PlayersLevels[id][i] = 0
		}
	}
	Pokemon[id] = poke
	PlayersActive[id] = 0
	PlayersPoke[id][0] = poke
	PlayersXP[id][0] = PokeLevels[random_num(1, get_ppcvar_num(pm_wild_level))-1]/2
	PlayersLevel(id, 0, true);

	new pname[NAME_LENGTH]
	PokeToName(poke, pname, NAME_LENGTH)
	poke_printlang(LANG_PLAYER,"POKEMOD_WILDAPPEAR", pname)
	poke_print(id, print_center, LANG, id,"POKEMOD_WILDARE", pname)
 }
 
 UnAssignWild(id)
 {
	if(WildCount()<=get_ppcvar_num(pm_wild) && get_ppcvar_num(pm_wild)>0){
		poke_printlang(id,"POKEMOD_NOWILDS", WildCount(), get_ppcvar_num(pm_wild))
		return
	}
	new name[32]
	get_user_name(id, name, 31)

	if(Pokemod_Alive(id))
		poke_printlang(LANG_PLAYER,"POKEMOD_WILDWANDER", PokeToName(Pokemon[id]))
	poke_printlang(LANG_PLAYER,"POKEMOD_WILDJOIN", name)

	LoadXP(id)
 }
 
 WildCount()
 {
	new count
	new players[MAX_PLAYERS], pnum
	poke_get_players(players, pnum)
	for(new i=0; i<pnum; i++){
		if(g_isWild[players[i]]>0)
			count++
	}
	return count
 }
 
 public WildMessage()
 {
	if(!Safari() && get_ppcvar_num(pm_wild)>0){
		poke_printlang(LANG_PLAYER,"POKEMOD_WILDREQ", get_ppcvar_num(pm_wild))
	}
 }
 #endif
 
 
 