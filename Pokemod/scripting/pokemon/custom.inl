 #if defined _pokemod_custom_included
	#endinput
 #endif
 #define _pokemod_custom_included

 /////////////////////////////////
 enum       //                  //
 {          //                  //
    CS,     //    Do NOT Edit   //
    DOD,    //       These      //
    NS      //                  //
 }          //                  //
 /////////////////////////////////////////
 enum               //                  //
 {                  //                  //
    SAVE_VAULT,     //    Do NOT Edit   //
    SAVE_MYSQL,     //       These      //
    SAVE_NVAULT     //                  //
 }                  //                  //
 /////////////////////////////////////////////////////////////////////////////////////////////
 #define MOD CS     //  Edit this to which game you are going to be using (from the above)  //
 /////////////////////////////////////////////////////////////////////////////////////////////
 #define SAVE SAVE_NVAULT  //     Edit this to which saving type you want (from the above)  //
 ////////////////////////////////////////////////////////////////////////////////
 
 //reading in the xp_table.ini will not go above this
 #define MAX_LEVELS         100

 //max pokemon you can register with pokemod
 #define TOTAL_POKES        275

 //max items you can register with pokemod
 #define MAX_ITEMS          200

 //how much of the original price is given back for selling items
 #define SELLFACTOR         0.5

 //max speed they can be running at (default 700.0)
 #define MAX_SPEED          700.0
 //speed change for slow status (default 150.0)
 #define SPEED_SLOW         -150.0
 //speed change for frozen status (default 150.0)
 #define SPEED_FROZEN       -150.0

 //max amount of money someone can have
 #define MAX_MONEY          16000

 //didn't know what to call this, but heres how it works
	// 0 - when someone "catches" their pokemon, they lose theirs. when someone gives their pokemon away, they lose it.
	// 1 - when someone "catches" their pokemon, they keep theirs. when someone gives their pokemon away, they lose it.
	// 2 - when someone "catches" their pokemon, they keep theirs. when someone gives their pokemon away, they keep it.
	// For example using POKE_POPULATION 1:
	//  Player A has Gyarados and Player B catches A's Gyarados, there are now 2 Gyarados on the map.
	//  Player B then gives Player C his Gyarados. There are still only 2 Gyarados on the map.
 #define POKE_POPULATION    1

 //should players receive the same ammount of xp as the one they catch
	// 1 - when someone catches a level 32 pidgey, they will receive a level 32 pidgey
	// 0 - when someone catches a level 32 pidgey, they will receive a level 1 pidgey
 #define POKEMON_CATCH_XP   0

 //if a pokemon loses experience to below when it evolved, does it un-evolve
 #define POKEMON_UNEVOLVE   0

 //what level bots pokemon start at
 #define BOT_START_LEVEL    5

 //are special pokemon only able to be used by admins? 1=admin only 0=anyone can use
 #define ADMIN_SPECIAL      0

 //teleport back to spawn when you catch a pokemon?
	// 0 - never teleport
	// 1 - teleport only when you pick up a pokeball
	// 2 - always teleport when you catch something (pick up pokeball or use pokeball to catch someone)
 #define CATCH_TELEPORT     0

 //max level a pokemon can be caught at (if set to 50, 51+ can't be caught, set to 0 to disable)
 #define CATCH_MAX_LEVEL    49

 //do players need to have a silphscope item to catch ghost pokemon (1==yes;0==no)
 #define CATCH_GHOST        0

 //allow players to go /wild and /trainer? (1==yes ; 0==no)
 #define ALLOW_WILD         0

 //max amount of items a player can buy at one time (default:20) set to 0 to ignore
 #define MAX_AMOUNT_TO_BUY  5

 //max number of a single item that someone can have
 #define MAX_ITEM_AMOUNT    2

 //max number of items that someone can have in total
 #define MAX_ITEM_TOTAL     10

 //if users have more HP than this, no pokeskill will kill them immediately
 #define HP_KILL_ALLOW      50

 //should items be used automatically
	// 0 - no items are used automatically
	// 1 - only for bots
	// 2 - items are used automatically for everyone
 #define AUTO_ITEMS         2

 //do players awaken if they are damaged through weapons in-game
 #define DAMAGE_AWAKEN      0
 //do players awaken if they are damaged through pokemon damage
 #define POKEDAMAGE_AWAKEN  0

 //use the pokerank system? (1=yes 0=no)
 #define POKERANK_SYSTEM    1
 //how many ranks will you have? (note that the top player will be PokeMaster, and the next top 4 will be the Elite Four
 #define MAX_RANKS          11
 //Note that if someone levels a pokemon from 1 to 99, they will get 4950 points
 stock const PokeRanks[MAX_RANKS]={	
	0,
	350,
	3250,
	7000,
	10000,
	30000,
	60000,
	90000,
	100000,
	200000,
	500000
 }
 stock const PokeRankNames[MAX_RANKS][]={
	"Pokemon Fanatic",
	"Pokemon Trainer",
	"Advanced Trainer",
	"Extreme Trainer",
	"Master Trainer",
	"Legendary Trainer",
	"Pokemon Guru",
	"Advanced Guru",
	"Extreme Guru",
	"Master Guru",
	"Legendary Guru"
 }

 //whether people can change pokemon when they're alive
	// 0 - they can always change pokemon
	// -1 - bots can only change once (this prevents occasional switch spam from bots)
	// 1 and higher - everyone can only change this many times
 #define CHANGE_POKES       -1

 //delay given to pokeskills after being spawned
 #define SPAWN_DELAY        5.0
 //delay given to pokeskills when changing pokemon
 #define CHANGE_DELAY       5.0

 //how many seconds pass before a pokemon is released for sure
 #define RELEASE_TIME       60.0
 //allow people to say /release and get a menu to release? (1==yes 0==no)
 #define RELEASE_MENU       1

 //should admin commands be prefixed with pm_
	//1 - pm_
	//0 - amx_ - doesn't filter commands in pm_help
 #define PM_ADMIN_COMMANDS  1

 //should pokemod cvars be prefixed with pm_
	//1 - pm_
	//0 - amx_
 #define PM_CVAR_PREFIX     1

 //change the game name from Counter-Strike, Natural Selection, etc.
 #define CHANGE_GAME_NAME   1
 //what to change the game name to
 #define GAME_NAME          "PokeMod: 2nd Season"

 // Armor Takes 80% of the damage (0.2 in the SDK)
 #define ARMOR_RATIO        0.2
 // Each Point of Armor is worth 1/x points of health (0.5 in the SDK)
 #define ARMOR_BONUS        0.5

 //if a special pokemon is going to be in a pokeball on the ground,
	//this is the chance out of 100 that it will stay a special pokemon.
	//set to 0 for no special pokemon on ground
 #define POKEBALL_SPECIAL   80

 //how many pokeballs to spawn at the spawns
 #define POKEBALL_NUMBER    5

 //should pokeballs be respawned when someone picks up a pokeball? (1=yes, 0=no)
 #define POKEBALL_RESPAWN   1

 //block player chat messages dealing with pokemod? (1=yes, 0=no)
 #define CHAT_SPAM          1

 // How rare candy items work -
	//	1 - add a level to the current pokemon (does not save to pokemon)
	//	2 - give to a pokemon and they level up
	//  3 - both
 #define RARE_CANDY_MODE    2

 // Random items on death
	//	0 - no random items
	//	1 - for a random item
	//	2 - for a random item based on price. items with lower prices are more likely (items with no price have a chance as if it costed the most)
 #define RANDOM_ITEMS       2

 //chance out of 100 that players will drop a random item (set to negative to only allow items that can be bought)
 #define RANDOM_CHANCE      50

 //do touch skills effect teammates (1==yes;0==no)
 #define TOUCH_SKILL_TEAM   0

 //time that players need to wait inbetween using skills
 #define SKILL_SPAM         1

 //when someone releases the pokeskill button, should it "unspam" them (allow to use immediately again)[this can be abused if yes] (1==yes;0==no)
 #define SKILL_UNSPAM       0

 //should tempents (special effects) be sent to spectators? (1==yes;0==no)
 #define SPEC_MSGS          0

 //how many rounds should go by without saving
	//if set to 0, round saving will not occur
	//if set to a negative number, random players will save each round (the number is how many players will save)
 #define SAVE_ROUNDS        -3

 //time that players need to wait inbetween saving, this will prevent saving happening a lot and creating lag
 #define SAVE_SPAM          60.0

 //how to save data ( 0 - steamid ; 1 - nickname )
 #define SAVE_ID            0

 //only admins with rcon access can change other peoples pokemon
 #define RCON_POKEADMINS    0

 //have pokemod register with EAM (Extendable Admin Menu)
 #define EAM_MENU           0

 //if one skill with a delay time is used, stop calling other skills
 #define ONE_SKILL_USE      1

 //how many pokemon to display per page in the pokedex
 #define POKEDEX_PERPAGE    10

 //display pictures of pokemon in pokedex (1==yes;0==no)
 #define POKEDEX_IMAGES     1

 //does using heal at pokecenter reset skills?
	// 0 - no skills are reset
	// 1 - skills with flag 't' are reset (not really recommended)
	// 2 - skills with flag 't' or 'q' are reset
 #define PC_RESET_SKILLS    0

