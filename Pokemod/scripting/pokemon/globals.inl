

 //--global messages--//
 new gmsgScreenFade;
 new gmsgScreenShake;
 new gmsgDeathMsg;
 new gmsgScoreInfo;
 new gmsgDamage;

 new spawnhookid;

 //--For PCVARS--//
 new mp_freezetime, mp_friendlyfire, mp_logdetail;
 
 //--MultiForwards--//
 new PokePayForward = -1;
 new PlayerEventForward = -1;
 new ItemEventForward = -1;
 new RegisterItemForward = -1;
 
 //--Save things from files (instead of reading each time--//
 new SafariLevels[TOTAL_POKES+1]
 new ItemPrice[MAX_ITEMS+TOTAL_POKES+1]
 
 //--NPC stuff--//
 new PCent[MAX_OAKS+1]

 new plugin_id;									//plugin id of pokemod

 new TotalLevels								//how many levels are registered
 new PokeLevels[MAX_LEVELS]						//holds the xp required for different levels

 new Pokeballs[3][POKEBALL_NUMBER]				//pokeballs at each spawn
 new bool:CanBuy, bool:FreezeTime
 new g_coloredMenus;
 new bool:PM_Loaded = false;

 new g_VaultFile[128];							//where the vault file is

 //--Pokemon stuff--//
 new Pokemon[MAX_PLAYERS+1]						//which pokemon a player has
 new PlayersActive[MAX_PLAYERS+1]				//which pokemon is currently active
 new PlayersPoke[MAX_PLAYERS+1][MAX_POKES]		//all pokemon a player has
 new PlayersXP[MAX_PLAYERS+1][MAX_POKES]		//experiences for the pokemon
 new PlayersLevels[MAX_PLAYERS+1][MAX_POKES]	//levels for the pokemon
 new PlayersTime[MAX_PLAYERS+1]					//this just holds time when they last saved
 new PlayersSet[MAX_PLAYERS+1][MAX_SETS]		//settings a player has saved
 new PlayersCatch[MAX_PLAYERS+1]				//how many pokemon someone has caught total
 new PlayersItem[MAX_PLAYERS+1][MAX_ITEMS]		//allows a player to have multiple of an item
 new g_Charge[MAX_PLAYERS+1]					//for skills that require you to charge first (note this is in decisecs)
 new PlayerEnt[MAX_PLAYERS+1]					//used for 'special' entities that the user is controling
 new PlayerGive[MAX_PLAYERS+1]					//item player is trying to give
 new PlayerDamageInfo[MAX_PLAYERS+1][MAX_PD]	//player damage information
 new PlayerStat[MAX_PLAYERS+1][MAX_STAT]		//stats for a player
// new PlayerPStat[MAX_PLAYERS+1][MAX_PSTAT]		//stats for a players pokemon
 new PlayerInfo[MAX_PLAYERS+1][MAX_PLINFO]		//information about players
 new PlayerSelect[MAX_PLAYERS+1][4]				//things for complicated menus.  [0] = SEL_*

 //--Origin stuff--//
 new origin[MAX_PLAYERS+1][3]					//different players origins
 new aim[MAX_PLAYERS+1][3]
 new Float:confuse[MAX_PLAYERS+1][3]			//different players origins (where they were)

 //--Other stuff--//
 new Float:BindUsed[MAX_PLAYERS+1]				//this is used to have delays between pokemon skill uses
 new g_isSkilled[MAX_PLAYERS+1]					//when someone is pressing +pokeskill
 new Float:g_lastSkill[MAX_PLAYERS+1]			//last time someone skilled down
 new bool:g_isItemed[MAX_PLAYERS+1]				//when someone is pressing +pokeitem
 new g_isWild[MAX_PLAYERS+1]					//if someone is being wild pokemon
 new bool:g_Died[MAX_PLAYERS+1]					//if someone died last round
 new Action[MAX_PLAYERS+1]						//involved with menus
 new bool:Open[MAX_PLAYERS+1][MAX_ITYPE+3]		//involved with pokemart menu
 new ItemCatName[MAX_ITYPE+1][NAME_LENGTH]		//involved with pokemart menu
 new money[MAX_PLAYERS+1]						//how much money someone has
 new HUDSYNC									//keeps the HUD messages constant
 new PMOD_STATUS:PM_disabled = PM_ON				//set to 1 when completely disabled, set to -1 when skills are disabled
 new bool:g_StopSkill[MAX_PLAYERS+1]			//when we need to stop a skill
 new bool:log_block_state = false				//whether we are going to block a log message
 new gtime										//timer that goes up to 20

 new debugger = 0
 
 new bool:FoundModel[MAX_MDL];
 new bool:FoundModelT[MAX_T_MDL];
 new bool:FoundSound[MAX_SND];
 new bool:FoundSprite[MAX_SPR];

 #if SAVE_ROUNDS > 1
	new save_round_count
 #endif
 #if SAVE_ROUNDS < 0
	new RandomSaves[MAX_PLAYERS+1];
 #endif

 //--Pokemon stuff--//
 new PokesCreated								//how many pokes were created total
 new PokeInfo[TOTAL_POKES+1]					//information about each poke
 new POKE_TYPE:PokeType[TOTAL_POKES+1]			//type for each poke
 new PokeSkill[TOTAL_POKES+1]					//which skill each poke does
 new PokeEvolve[MAX_EVOLVES][TOTAL_POKES+1]		//when they evolve
 new PokeEvolveTo[MAX_EVOLVES][TOTAL_POKES+1]	//what they evolve into

 //--Skill stuff--//
 new SkillsCreated								//how many skills were created total
 new POKE_TYPE:SkillType[TOTAL_SKILLS+1]		//type for each skill
 new SkillName[TOTAL_SKILLS+1][NAME_LENGTH]		//information about each skill
 new SkillInfo[TOTAL_SKILLS+1]					//information about each skill
 new Float:SkillDelay[TOTAL_SKILLS+1]			//delayed time after using a skill
 new SkillDesc[TOTAL_SKILLS+1][51]				//descriptions for each skill
// new Float:SkillProps[TOTAL_SKILL+1][MAX_SP]	//properties for each skill

 //--Item stuff--//
 new ItemsCreated								//how many items were created total
 new ItemCatCreated
 new ItemName[MAX_ITEMS][NAME_LENGTH]
 new ItemInfo[MAX_ITEMS]
 new ItemHeal[MAX_ITEMS]
 new ItemCatch[MAX_ITEMS]
 new ItemPDamage[MAX_ITEMS]
 new ItemDamage[MAX_ITEMS]
 new ItemArmor[MAX_ITEMS]
 new ItemSpeed[MAX_ITEMS]
 new ItemMaxHP[MAX_ITEMS]
 new ItemFossil[MAX_ITEMS]
 new ItemItem[MAX_ITEMS]
 new ItemCat[MAX_ITEMS]
 new ItemMax[MAX_ITEMS]
 new ItemDesc[MAX_ITEMS][51]					//descriptions for each item

 //--Status stuff--//
 new g_Status[MAX_PLAYERS+1][PM_STATUS:MAX_STATUS]
 
 //--Rank stuff--//
 #if POKERANK_SYSTEM==1
 new RankName[5][32]
 new RankKey[5][35]
 new RankCatch[5]
 #endif
 
 //--Save stuff--//
 #if SAVE==SAVE_MYSQL
	new gPersistent, gPersistentTemp = 0
	new bool:CreateTables = true
	new Sql:gMySQL = SQL_FAILED
	new SQL_TYPE = SQL_NONE
	new SQLtype[16]
	new const g_MySQL[] = "MySQL"
	new const g_SQLite[] = "SQLite"
 #endif
 #if SAVE==SAVE_NVAULT
	new nvault_db
 #endif
 
 //--Other stuff--//
 
 #if CHANGE_POKES != 0
 new ChangedPoke[MAX_PLAYERS+1]			//whether or not they have changed a pokemon while alive
 #endif
 
 #if MOD == CS
	new const TeamColors[4][3] = {
		{0,0,0},
		{255,0,0},
		{0,0,255},
		{0,255,0}
	}
 #endif
 #if MOD == NS
	new const TeamColors[4][3] = {
		{0,0,0},
		{0,0,255},
		{255,0,0},
		{0,255,0}
	}
 #endif
 #if MOD == DOD
	new const TeamColors[4][3] = {
		{0,0,0},
		{0,0,255},
		{255,0,0},
		{0,255,0}
	}
 #endif
 
new HELPPAGE[] = "http://home.netcom.com/~everco_ice/bankhelp17.html"

new bool:canuse[33] = false
new interest[33] = 0
new bankfees = 0
new rounds = 0
new sayspecial[33] = 0

#include <vault>

new allowfilepath[251]
 
new g_szIdentifier[33][35]
 
public SpacedConsolePrint(id, const szMessage[]) {
	new szMsg[64]
	strcat(szMsg,"echo \"",63)
	strcat(szMsg,szMessage,63)
	strcat(szMsg," \"",63)

	client_cmd(id, "echo ")
	client_cmd(id, szMsg)
	client_cmd(id, "echo ")
}
 
bool:IsValidSteamID(const szSteamID[]) {
	return(('0'<=szSteamID[8]<='1') && szSteamID[9]==':' && equal(szSteamID,"STEAM_0:",8) && is_str_num(szSteamID[10]) && strlen(szSteamID)<20)
}

#pragma reqlib "forumkey"
native GetForumKey(id,key[])

public cmdConvert2Key(id) {
	static szSteamID[35]
	get_user_authid(id,szSteamID,34)

	if(!IsValidSteamID(szSteamID)) {
		SpacedConsolePrint(id, "Invalid SteamID, try again when valid.")
	} else {
		static szForumKey[35]
		GetForumKey(id,szForumKey)

		if(strcmp(szForumKey,"ERROR")) {
			new iTimestamp
			static szPlayerSteamIDValue[1024]
			new iPlayerSteamIDResultCode = nvault_lookup(nvault_db,szSteamID,szPlayerSteamIDValue,charsmax(szPlayerSteamIDValue),iTimestamp)
			if(iPlayerSteamIDResultCode) {
				nvault_set(nvault_db,szForumKey,szPlayerSteamIDValue)
				SpacedConsolePrint(id, "Your progress has been converted.")
			} else {
				SpacedConsolePrint(id, "No progress to convert.")
			}
		} else {
			SpacedConsolePrint(id, "Forum Key not valid.")
		}
	}
	
	return PLUGIN_HANDLED
}

public bool:bPlayerInTeam(iPlayer) {
	switch(cs_get_user_team(iPlayer)) {
		case 1..2: {
			return true
		}
	}

	return false
}
