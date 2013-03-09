 #if defined _pokemod_defines_included
    #endinput
 #endif
 #define _pokemod_defines_included
 
 stock const PLUGNAME[] =    "PokeMod: Johto"
 stock const PREFIX[] =      "[PokeMod Season 2]"
 #define AUTHOR              "hib"
 stock const POKETEAM[] =    "Emp`/ hib"
 stock const VERSION[] =     "hib"

 //MODELS
 enum{
    MDL_ROCK,
    MDL_SKULL,
    MDL_POWDER,
    MDL_HAIR,
    MDL_ROCKET,
    MDL_FUNGUS,

    MDL_POKEBALL,
    MDL_SURF,
    MDL_OAK,
    MDL_BONE,
    MDL_DRKBALL,
    MAX_MDL
 }
 enum{
    MDL_T_HAIR,
    MDL_T_OAK,
    MAX_T_MDL
 }
 stock const MODELS[MAX_MDL][] = {
    "models/big_rock.mdl",
    "models/bleachbones.mdl",
    "models/teleporter_blue_sprites.mdl",
    "models/hair.mdl",
    "models/rpgrocket.mdl",
    "models/fungus.mdl",

    //Custom Pokemon Models
    "models/pokemon/w_pokeball.mdl",
    "models/pokemon/wave.mdl",
    "models/pokemon/oak.mdl",
    "models/pokemon/bone.mdl",
    "models/pokemon/drkball.mdl"
}
 stock const MODELS_T[MAX_T_MDL][] = {
    "models/hairt.mdl",
    "models/pokemon/oakT.mdl"
 }

 //SOUNDS
 enum{
	SND_THEME,

    SND_HIT,
    SND_TU_PING,
    SND_TU_FIRE,
    SND_BODYDROP,
    SND_SUCK,
    SND_BURN,
    SND_FLAME,
    SND_AG_DIE,
    SND_SPARK,
    SND_BELL,
    SND_WADE,
    SND_SWIM,
    SND_JUMPLAND,
    SND_PAIN,
    SND_STEP,
    SND_BIGGUN,
    SND_DENY,

    SND_POKECENTER,
    SND_EXM2,
    SND_CURE,
    SND_CAST,
    SND_SING,
    MAX_SND
 }
 stock const SOUNDS[MAX_SND][] = {
    "pokemon/theme.wav",

    "hornet/ag_hornethit1.wav",
    "turret/tu_ping.wav",
    "turret/tu_fire1.wav",
    "common/bodydrop1.wav",
    "ambience/particle_suck1.wav",
    "ambience/burning1.wav",
    "ambience/flameburst1.wav",
    "agrunt/ag_die3.wav",
    "buttons/spark6.wav",
    "buttons/bell1.wav",
    "player/pl_wade3.wav",
    "player/pl_swim3.wav",
    "bullchicken/bc_idle2.wav",
    "player/pl_pain7.wav",
    "player/pl_step4.wav",
    "ambience/biggun3.wav",
    "common/wpn_denyselect.wav",

    //Custom Pokemon Sounds
    "pokemon/pokecenter.wav",
    "pokemon/a_exm2.wav",
    "pokemon/cure.wav",
    "pokemon/cast.wav",
    "pokemon/jigglypuff.wav"
 }

 //SPRITES
 enum{
    SPR_ROPE,
    SPR_BUBBLE,
    SPR_SHOCKWAVE,
    SPR_STEAM1,
    SPR_EXPLODE1,
    SPR_XFIRE,
    SPR_BALLSMOKE,
    SPR_ZBEAM6,
    SPR_EEXPLO,
    SPR_FIRE,
    SPR_LGTNING,
    SPR_MUZZLEFLASH1,
    SPR_BIGSPIT,

    SPR_XPLODE,
    SPR_ARBOK,
    SPR_TRAIL,
    SPR_GAS,
    SPR_SNOW,
    SPR_AEROBLAST,
    MAX_SPR
 }
 stock const SPRITES[MAX_SPR][] = {
    "sprites/rope.spr",
    "sprites/bubble.spr",
    "sprites/shockwave.spr",
    "sprites/steam1.spr",
    "sprites/explode1.spr",
    "sprites/xfire.spr",
    "sprites/ballsmoke.spr",
    "sprites/zbeam6.spr",
    "sprites/eexplo.spr",
    "sprites/fire.spr",
    "sprites/lgtning.spr",
    "sprites/muzzleflash1.spr",
    "sprites/cnt1.spr",

    "sprites/pokemon/zerogxplode2.spr",
    "sprites/pokemon/arbok.spr",
    "sprites/pokemon/psybeam.spr",
    "sprites/pokemon/splash.spr",
    "sprites/pokemon/snow.spr",
    "sprites/pokemon/aeroblast.spr"
}
 stock SPRITE_INDEX[MAX_SPR];

 #define MAX_PLAYERS             32                   //max players there can be
 #define MAX_SKILLS              4                    //max skills that a pokemon can have
 #define TOTAL_SKILLS            200                //max skills that pokemod can handle

 #define MAX_ACTIVE_POKES        6                   //max pokes that someone can have in their party
 #define MAX_PC_POKES            50                //max pokes that someone can have in PC
 #define MAX_POKES MAX_ACTIVE_POKES+MAX_PC_POKES      //max pokes that someone can have total


 #define MAX_ITYPE               11                   //how many different item types there are
 #define MAX_OAKS                20                   //maximum number of oaks
 #define MAX_OAKPROP             3                    //how many different properties there are for oaks
 #define MAX_EVOLVES             5                    //max times something has the chance to evolve
 #define MAX_POKE_ENTS           200

 #define NAME_LENGTH             32
 #define DESC_LENGTH             51
 
 stock const EMPTY[] =           ""
 stock const SPACE[] =           " "
 stock const PERIOD[] =          "."
 stock const UNDERSCORE[] =      "_"
 stock const DASH[] =            "-"
 stock const PLUS[] =            "+"
 stock const COLON[] =           ":"
 stock const AT[] =              "@"
 stock const STAR[] =            "*"
 stock const NEWLINE[] =         "\n"
 stock const LEFT_BRACKET[] =    "["
 stock const RIGHT_BRACKET[] =   "]"
 stock const LEFT_PARENT[] =     "("
 stock const RIGHT_PARENT[] =    ")"
 stock const LEFT_CBRACKET[] =   "{"
 stock const RIGHT_CBRACKET[] =  "}"
 stock const LANG[] =            "%L"

 //Players Settings
 enum
 {
    SET_HUD_Y = 0,
    SET_HUD_PARTY,
    MAX_SETS
 }


 #define P_FLAG_A        (1<<0)        /* flag "a" */
 #define P_FLAG_B        (1<<1)        /* flag "b" */
 #define P_FLAG_C        (1<<2)        /* flag "c" */
 #define P_FLAG_D        (1<<3)        /* flag "d" */
 #define P_FLAG_E        (1<<4)        /* flag "e" */
 #define P_FLAG_F        (1<<5)        /* flag "f" */
 #define P_FLAG_G        (1<<6)        /* flag "g" */
 #define P_FLAG_H        (1<<7)        /* flag "h" */
 #define P_FLAG_I        (1<<8)        /* flag "i" */
 #define P_FLAG_J        (1<<9)        /* flag "j" */
 #define P_FLAG_K        (1<<10)       /* flag "k" */
 #define P_FLAG_L        (1<<11)       /* flag "l" */
 #define P_FLAG_M        (1<<12)       /* flag "m" */
 #define P_FLAG_N        (1<<13)       /* flag "n" */
 #define P_FLAG_O        (1<<14)       /* flag "o" */
 #define P_FLAG_P        (1<<15)       /* flag "p" */
 #define P_FLAG_Q        (1<<16)       /* flag "q" */
 #define P_FLAG_R        (1<<17)       /* flag "r" */
 #define P_FLAG_S        (1<<18)       /* flag "s" */
 #define P_FLAG_T        (1<<19)       /* flag "t" */
 #define P_FLAG_U        (1<<20)       /* flag "u" */
 #define P_FLAG_V        (1<<21)       /* flag "v" */
 #define P_FLAG_W        (1<<22)       /* flag "w" */
 #define P_FLAG_X        (1<<23)       /* flag "x" */
 #define P_FLAG_Y        (1<<24)       /* flag "y" */
 #define P_FLAG_Z        (1<<25)       /* flag "z" */

 enum PLAYER_SELECT
 {
    PLAYER_NONE =       0,
    PLAYER_ALIVE =      P_FLAG_A,
    PLAYER_DEAD =       P_FLAG_B,
    PLAYER_REAL =       P_FLAG_C,
    PLAYER_BOT =        P_FLAG_D,
    PLAYER_INTEAM =     P_FLAG_E,
    PLAYER_GOD =        P_FLAG_F,
    PLAYER_MORTAL =     P_FLAG_G,
    PLAYER_NOHLTV =     P_FLAG_H,
    PLAYER_NOTEAM =     P_FLAG_I
 }
 
 enum POKE_TYPE
 {
    TYPE_NONE =        0,
    TYPE_NORMAL =      P_FLAG_A,
    TYPE_FIRE =        P_FLAG_B,
    TYPE_WATER =       P_FLAG_C,
    TYPE_ELECTRIC =    P_FLAG_D,
    TYPE_GRASS =       P_FLAG_E,
    TYPE_ICE =         P_FLAG_F,
    TYPE_FIGHTING =    P_FLAG_G,
    TYPE_POISON =      P_FLAG_H,
    TYPE_GROUND =      P_FLAG_I,
    TYPE_FLYING =      P_FLAG_J,
    TYPE_PSYCHIC =     P_FLAG_K,
    TYPE_BUG =         P_FLAG_L,
    TYPE_ROCK =        P_FLAG_M,
    TYPE_GHOST =       P_FLAG_N,
    TYPE_DRAGON =      P_FLAG_O,
    TYPE_DARK =        P_FLAG_P,
    TYPE_STEEL =       P_FLAG_Q,
//    TYPE_ =            P_FLAG_R,      // For the next type
    MAX_TYPES =        P_FLAG_R-1
 }

 //lets define some tasks (so we can remove the tasks if we need to)
 #define POKESKILL_TASK     24567
 #define FREEZE_TASK        26567
 #define BUYTIME_TASK       27567
 #define REMOVE_TASK        28567
 #define BLIZZARD_TASK      29567
 #define SOLID_TASK         30567
 #define RELEASE_TASK       31567
 #define RELEASE2_TASK      32567
// #define _TASK      33567
 #define SPAWN_TASK         34567
 #define SKILLCALL_TASK     35000
// SKILLCALL_TASK extends for multiple tasks, give it 10000 different numbers
// #define _TASK            46567

 //lets define some statuses for g_Status[id][STATUS]
 enum PM_STATUS
 {
    STATUS_BURN = 0,
    STATUS_FROZEN,
    STATUS_PARALYZE,
    STATUS_ASLEEP,
    STATUS_OAK_STOP,
    STATUS_SLOW,
    STATUS_POISON,
    STATUS_BIKE,
    STATUS_CONFUSE,
    MAX_STATUS
 }

 //for player stats
 enum
 {
    STAT_MAXHP,
    STAT_MAXAP,
    STAT_ATTACK,
    STAT_DEFENSE,
    STAT_SPEED,
    STAT_SPECIAL,
    MAX_STAT
 }
 //for pokemon stats
 enum
 {
    PSTAT_MAXHP,
    PSTAT_ATTACK,
    PSTAT_DEFENSE,
    PSTAT_SPEED,
    PSTAT_SPECIAL,
    PSTAT_HAPPINESS,
    MAX_PSTAT
 }

 //for menu handler 
 enum
 {
    AC_NONE=0,          //no menu
    AC_START,           //start menu
    AC_MAIN,            //main menu
    AC_MART,            //pokemart menu
    AC_GO,              //go menu
    AC_RELEASE,         //release pokemon menu
    AC_PC_GIVE,         //giving pc a poke
    AC_PC_TAKE,         //taking poke from pc
    AC_ITEM,            //item use menu
    AC_HELP,            //help menu
    AC_SKILL,           //skill menu
    AC_GIVE_ITEM,       //give pokemon item menu
    AC_SAFARI,          //safari menu
    AC_SELECT,          //selection menu
    AC_PC               //pc menu
 }
 
 //for complicated menus
 enum
 {
    SEL_EVOLVE,		// original poke, evolved poke
    MAX_SEL
 }

 //for screenflashes
 #define SECONDS_TO_SCREENFADE_UNITS(%1) floatround(float((1<<12)) * (%1))

 stock POKENAMES[TOTAL_POKES+1][NAME_LENGTH];
 
 //These are for unstucking people
 #define UNSTUCK_START_DISTANCE      32             // The first search distance for finding a free location in the map
 #define UNSTUCK_MAX_ATTEMPTS        128            // How many times to search in an area for a free space

 //These are numbers for within pokemod and should never be changed
 #define TRADE_EVOLVE               -999
 #define AUTO_KILL                  99999
 #define FIND_POKECENTER            -100
 #define WAIT_TIME                  -100.0       //if someone is waiting for a pokeskill, their BindUsed == this
 #define MART_CAT_OFFSET            1000         //offset for pokemart categories
 #define MART_SELL_OFFSET           2000         //offset for pokemart selling
 #define MART_POKE_OFFSET           MAX_ITEMS    //offset for pokemart pokemon
 #define POKE_MENU_EXIT             -333         //exit buttons for pokemenus
 #define POKE_HP_OFFSET             1000

 enum
 {
    SKILL_READY = 0,
    SKILL_USED
 }

 enum CHECK_POKES
 {
    CHECK_PC = -1,
    CHECK_ACTIVE = 0,
    CHECK_ALL = 1
 }

 enum PMOD_STATUS
 {
    PM_XP = -1,
    PM_ON = 0,
    PM_OFF = 1
 }

 enum BOT_SAYS
 {
    BOT_BOUGHT_ITEM = 0,
    BOT_SELECT_POKE
 }

 #if MOD==CS
 stock const spawnEntString[2][] = {"info_player_start","info_player_deathmatch"}
 #endif
 #if MOD==DOD
 stock const spawnEntString[2][] = {"info_player_axis","info_player_allies"}
 #endif
 //For NS we just use the stock provided (ns_get_spawn)

 //Poke Infos
 #define PI_BASE        P_FLAG_A      /* flag "a" */
 #define PI_EVOLVE      P_FLAG_B      /* flag "b" */
 #define PI_NOFIND      P_FLAG_C      /* flag "c" */
 #define PI_START       P_FLAG_D      /* flag "d" */
 #define PI_NOCATCH     P_FLAG_E      /* flag "e" */
 #define PI_GLOBAL      P_FLAG_G      /* flag "g" */
 #define PI_HEAVY       P_FLAG_H      /* flag "h" */
 #define PI_SPECIAL     P_FLAG_Y      /* flag "y" */
 #define PI_DISABLE     P_FLAG_Z      /* flag "z" */

 //Item Infos
 #define II_HEAL        (1<<0)        /* flag "a" */
 #define II_CATCH       (1<<1)        /* flag "b" */
 #define II_REMEDY      (1<<2)        /* flag "c" */
 #define II_BURN        (1<<3)        /* flag "d" */
 #define II_FROZEN      (1<<4)        /* flag "e" */
 #define II_PARALYZE    (1<<5)        /* flag "f" */
 #define II_ASLEEP      (1<<6)        /* flag "g" */
 #define II_SLOW        (1<<7)        /* flag "h" */
 #define II_POISON      (1<<8)        /* flag "i" */
 #define II_ESCAPE      (1<<9)        /* flag "j" */
 #define II_GIVE        (1<<10)        /* flag "k" */
 #define II_PDAMAGE     (1<<11)        /* flag "l" */
 #define II_DAMAGE      (1<<12)        /* flag "m" */
 #define II_ARMOR       (1<<13)        /* flag "n" */
 #define II_SPEED       (1<<14)        /* flag "o" */
 #define II_MAXHP       (1<<15)        /* flag "p" */
 #define II_SPECIAL     (1<<16)        /* flag "q" */
 #define II_FOSSIL      (1<<17)        /* flag "r" */
 #define II_LEVEL       (1<<18)        /* flag "s" */
 #define II_BIKEV       (1<<19)        /* flag "t" */
 #define II_BIKE        (1<<20)        /* flag "u" */
 #define II_SCOPE       (1<<21)        /* flag "v" */

 #define II_NODROP      (1<<23)        /* flag "x" */
 #define II_MAX         (1<<24)        /* flag "y" */
 #define II_DISABLE     (1<<25)        /* flag "z" */

 //Skill Infos
 #define SI_ATTACK      P_FLAG_A        /* flag "a" */    //    When someone attacks someone else
 #define SI_BIND        P_FLAG_B        /* flag "b" */    //    When someone presses a bind key to activate
 #define SI_TOUCH       P_FLAG_C        /* flag "c" */    //    When someone touches someone else
 #define SI_DAMAGED     P_FLAG_D        /* flag "d" */    //    When someone is damaged
 #define SI_LOOP        P_FLAG_E        /* flag "e" */    //    Called every second
 #define SI_SPAWN       P_FLAG_F        /* flag "f" */    //    Called during spawn
 #define SI_DEAD        P_FLAG_G        /* flag "g" */    //    When someone dies
 #define SI_CALL        P_FLAG_H        /* flag "h" */    //    Call no matter what
 #define SI_IGNORE      P_FLAG_I        /* flag "i" */    //    Ignore delay times
 #define SI_PATTACK     P_FLAG_J        /* flag "j" */    //    When someone attacks someone else with a pokemon skill
 #define SI_PDAMAGED    P_FLAG_K        /* flag "k" */    //    When someone is damaged with a pokemon skill
 #define SI_LOOK        P_FLAG_L        /* flag "l" */    //    When someone looks at someone else
 #define SI_SPEED       P_FLAG_M        /* flag "m" */    //    When someone changes speed
// #define SI_      P_FLAG_N        /* flag "n" */    //    
// #define SI_      P_FLAG_O        /* flag "o" */    //    
// #define SI_      P_FLAG_P        /* flag "p" */    //    
 #define SI_ROUNDRESET  P_FLAG_Q        /* flag "q" */    //    Called at the start of a new round
 #define SI_RELEASE     P_FLAG_R        /* flag "r" */    //    When someone releases a bind key
 #define SI_STATUS      P_FLAG_S        /* flag "s" */    //    When someone changes status
 #define SI_RESET       P_FLAG_T        /* flag "t" */    //    A skill is being reset
 #define SI_SKILLCALL   P_FLAG_U        /* flag "u" */    //    A skill is calling itself
 #define SI_CONNECT     P_FLAG_V        /* flag "v" */    //    When someone connects to pokemod
 #define SI_DISCONNECT  P_FLAG_W        /* flag "w" */    //    When someone disconnects to pokemod
 #define SI_PUBLIC      P_FLAG_X        /* flag "x" */    //    Skill is in another plugin
 #define SI_ADMIN       P_FLAG_Y        /* flag "y" */    //    Only by admins
 #define SI_DISABLE     P_FLAG_Z        /* flag "z" */

 // Item Events
 #define IE_BUY         P_FLAG_A
 #define IE_PICKUP      P_FLAG_B
 #define IE_SELL        P_FLAG_C
 #define IE_DROP        P_FLAG_D
 #define IE_USE         P_FLAG_E

 //defines for player informations
 enum
 {
    PL_CONNECTED,    //if someone has loaded xp yet (set to 1 if they had xp, set to -1 if they dont have any xp yet)
    PL_ALIVE,
    PL_TEAM,
    MAX_PLINFO
 }

 //defines for different files
 enum PokeFiles
 {
    F_PokeDir,
    F_VaultFile,
    F_Skill,
    F_ModConfig,
    F_Pokemon,
    F_Items,
    F_Safari,
    F_PokeMaps,
    F_XPs,
    F_Type,
    F_MAX
 }

 //--Thing im using so i dont have to use get_user_health each time--//
 #define Health(%1) get_user_health(%1)

 enum AOE_FLAGS
 {
    AOE_NORMAL = 0,             //normal radial aoe attack
    AOE_IGNORE = P_FLAG_A,      //ignore attacker
    AOE_MAXDAM = P_FLAG_B,      //if within radius, do max damage
    AOE_ATEAM = P_FLAG_C,       //only deal with the attackers teammates
    AOE_VTEAM = P_FLAG_D        //deal with enemies of the attacker only
 }

 enum
 {
    SQL_NONE,
    SQL_MYSQL,
    SQL_SQLITE
 }
 enum
 {
    SQLITE_SYNC_OFF,
    SQLITE_SYNC_NORMAL,
    SQLITE_SYNC_FULL
 }

 enum
 {
    pm_pokemod, pm_admin, pm_save, pm_xpsavedays, pm_safari_mode, pm_debug, pm_debug_key

 #if ALLOW_WILD==1
    ,pm_wild, pm_wild_level
 #endif
    ,pm_maxhp, pm_maxarmor, pm_botsays, pm_pokecenter, pm_oak, pm_oakhp, pm_pokeballs, pm_buytime, pm_itemtime, pm_pokemart_open, pm_ffa

    ,pm_burntime, pm_burndamage, pm_waterheal, pm_poisontime, pm_poisondamage, pm_sleeptime

 #if SAVE==SAVE_MYSQL
    ,pm_mysql_host, pm_mysql_user, pm_mysql_pass, pm_mysql_db, pm_mysql_persistent
 #endif

    ,MAX_CVARS
 }
 stock pcvars[MAX_CVARS];
 stock const cvars[MAX_CVARS][] = {
    "pokemod","admin","save","xpsavedays","safari_mode","debug","debug_key"

 #if ALLOW_WILD==1
    ,"wild","wild_level"
 #endif

    ,"maxhp","maxarmor","botsays","pokecenter","oak","oakhp","pokeballs","buytime","itemtime","pokemart_open","ffa"

    ,"burntime","burndamage","waterheal","poisondamage","poisontime","sleeptime"

 #if SAVE==SAVE_MYSQL
    ,"mysql_host", "mysql_user", "mysql_pass", "mysql_db", "mysql_persistent"
 #endif

 }
 stock const cvar_default[MAX_CVARS][] = {
    "1","m","1","30","0","0",""

 #if ALLOW_WILD==1
    ,"0","33"
 #endif

    ,"255","255","1","200","ab","100","1","0.0","30.0","1","0"

    ,"10","5","3","5","10","3"

 #if SAVE==SAVE_MYSQL
    ,"localhost", "root", "", "pokemod", "1"
 #endif
 }

 enum
 {
    PD_ATTACKER,
    PD_DAMAGE,
    PD_WEAPON,
    MAX_PD
 }
 
 enum
 {
    SP_CHANCE,
    SP_RADIUS,
    SP_DAMAGE,
    SP_VEC,
    SP_TIME,
    SP_AMOUNT,
    SP_MAXAMOUNT,
    SP_USES,
    SP_PP,
    SP_WEAPON,
    MAX_SP
 }

 enum
 {
    AS_DUPLICATE = -2,
    AS_NOROOM,
    AS_NOGOOD,
    AS_OK
 }

#if !defined SECONDS_IN_DAY
    #define SECONDS_IN_DAY    86400
#endif

stock const sound_directory[] = "sound/";

#define poke_sound(%1,%2,%3)    emit_sound(%1,%2,SOUNDS[%3],1.0,ATTN_NORM,0,PITCH_NORM)

#define sound_deny(%1)            client_cmd(%1,"spk %s",SOUNDS[SND_DENY])
/* stock sounddeny(id)
    client_cmd(id,"spk %s", SOUNDS[SND_DENY]);*/

#define sound_cast(%1)            poke_sound(%1,CHAN_VOICE,SND_CAST)
/* stock soundcast(id)
    emit_sound(id, CHAN_VOICE, SOUNDS[SND_CAST], 1.0, ATTN_NORM, 0, PITCH_NORM);*/

#define sound_cure(%1)            poke_sound(%1,CHAN_VOICE,SND_CURE)
/* stock soundcure(id)
    emit_sound(id, CHAN_VOICE, SOUNDS[SND_CURE], 1.0, ATTN_NORM, 0, PITCH_NORM);*/

#define sound_hit(%1)            poke_sound(%1,CHAN_VOICE,SND_HIT)
/* stock soundhit(id)
    emit_sound(id, CHAN_AUTO, SOUNDS[SND_HIT], VOL_NORM, ATTN_NORM, 0, PITCH_NORM);*/


