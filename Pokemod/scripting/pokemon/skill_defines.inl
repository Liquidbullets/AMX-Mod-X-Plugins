
// Please only edit if you know what you are doing
//    * Note the default values. If there is a decimal '.' in the value, 
//      there must be a decimal in your value
//         Examples:
//              LEECH_SEED default is 5
//              LEECH_SEED can be 5, 10, 20, etc.
//              LEECH_SEED can NOT be 5.0, 10.5, 20.0, etc.
//
//              LEECH_SEED_TIME default is 5.0
//              LEECH_SEED_TIME can be 5.0, 10.5, 20.0, etc.
//              LEECH_SEED_TIME can NOT be 5, 10, 20, etc.


#define DELAY_LEVEL_REDUCE          0.04    //how much time is taken away per level

#define AGILITY_SPEED_MIN           100     //speed change at minimum level
#define AGILITY_SPEED_MAX           150     //speed change at maximum level

#define AURORA_BEAM_RADIUS          1.0     // level * # = radius of beam
#define AURORA_BEAM_DAMAGE          0.7     // level * # = max damage of beam

#define BARRAGE_MIN1                2       //minimum amount of eggs someone can get when pokemon is NOT active
#define BARRAGE_MAX1                4       //maximum amount of eggs someone can get when pokemon is NOT active
#define BARRAGE_MIN2                3       //minimum amount of eggs someone can get when pokemon is active
#define BARRAGE_MAX2                5       //maximum amount of eggs someone can get when pokemon is active

#define BARRIER_LIMIT               0       //max amount of barriers that can be placed near each other (0-no limit)
#define BARRIER_MIN_HP              20.0    //min amount of hp a barrier will have
#define BARRIER_MAX_HP              100.0   //max amount of hp a barrier will have

#define BIDE_FACTOR                 10       //every # damage, bide damage will go up by 1

#define BITE                        1       //how much damage does Bite damage and heal for

#define BLIZZARD_TIME               5.0     //initial time of blizzard
#define BLIZZARD_RADIUS             10      //level * # = max radius of blizzard

#define BONE_VEC                    750     //how fast bones are thrown
#define BONE_DAMAGE                 50      //how much damage bone attacks do

#define BUBBLE_BEAM_DAMAGE          30.0    // max damage
#define BUBBLE_BEAM_NUM             20      // level + # = bubbles to start with

#define CONFUSION_DISTANCE          100     //how close players have to be to be confused

#define CONSTRICT_TIME              1.0     //how long someone is paralyzed from constrict

#define CONVERSION_ALPHA            30      //how invisible are players that are using conversion (0=completely invisible 255=completely visible)
#define CONVERSION_TIME             10      //this + 5 = max time someone can use conversion
#define CONVERSION_SPEED_MIN        -150    //speed change at minimum level
#define CONVERSION_SPEED_MAX        -50     //speed change at maximum level

#define COUNTER_FACTOR              10       //every # damage, counter will go up by 1
#define COUNTER_MAX                 10      //max counter you can have

#define CRAB_HAMMER                 5       //multiplier for knife damages

#define DEFENSE_CURL                3       //armor given = damage / #

#define DIG_KNIFE                   1       //can players dig with weapons

#define DISABLE_TIME                6.0    //how long someone cannot use pokeskills after being disabled

#define ZAP_CANNON_RADIUS            1.0     // level * # = radius of beam
#define ZAP_CANNON_DAMAGE            0.7     // level * # = max damage of beam

#define DIZZY_PUNCH                 20      //damage for dizzy punch
#define DIZZY_PUNCH_DIZZY           200     //how much shaking for getting punched

#define DREAM_EAT_DAMAGE            30      //how much damage does dream eat do?

#define DRILL_PECK                  2       //how much damage does drill peck do to entities

#define EARTHQUAKE_DAMAGE           40      //how much damage earthquake starts at
#define EARTHQUAKE_RADIUS           300     //how far earthquake reaches

#define EMBER_TIME                  3       //how many seconds ember lasts by default
#define EMBER_RANGE                 175     //range ember has to burn players

#define EXPLOSION_RADIUS            400     //radius of voltorb's explosion
#define EXPLOSION_DAMAGE            150     //max damage of voltorb's explosion

#define FIRE_SPIN_DAMAGE            2       //level * # = max damage
#define FIRE_SPIN_RADIUS            200     //level + # = radius of max damage

#define FISSURE                     10      //max chance for fissure to autokill someone

#define FLASH_TIME                  1.0     //how long a user is flashed
#define FLASH_ALPHA_MIN             100     //min screen alpha of flash
#define FLASH_ALPHA_MAX             255     //max screen alpha of flash

#define FLY                         200     //boost given every keydown
#define FLY_FLAP                    1       //do players have to press keydown repeatedly (1=yes, 0=no)
#define FLY_FLAPS                   25      //how many times can a player flap their wings (if holding skill, players flap 10 times per second)

#define FURY_ATTACK_FACTOR          3       //every # damage on ap, damage will go up by 1

#define FURY_SWIPES_FACTOR          5       //every # damage on hp, damage will go up by 1

#define FUTURE_SIGHT_DAMAGE         0      //max damage future sight can do 

#define GROWTH_REDUCE               0.3     //how much time is removed from delay time

#define GUST_FACTOR                 2       //level / # = damage
#define GUST_VELOCITY               5       //gust damage * # = final velocity of gust

#define HORN_VEC                    1000    //how fast horns shoot
#define HORN_ATTACK_DAMAGE          30      //how much damage horn attack does
#define HORN_DRILL_DAMAGE           70      //how much damage horn drill does

#define HYDRO_PUMP_DAMAGE           4.5     // max damage (don't forget there are two beams fired)
#define HYDRO_PUMP_NUM              80      // level + # = hydropumps to start with

#define HYPER_FANG                  4       //knife attacks will do (maxhp of player) / #

#define HYPNOSIS_MAX_DISTANCE       750    //max distance someone can be to be affected by hypnosis

#define ICE_BEAM_RADIUS             0.7     // level * # = radius of beam
#define ICE_BEAM_DAMAGE             0.7     // level * # = max damage of beam

#define ICY_WIND_TIME               5.0    //how long icy-wind freezes for

#define KARATE_CHOP                 30      //how much damage karate chop does

#define LEAF_STORM_RADIUS           1.0     // level * # = radius of beam
#define LEAF_STORM_DAMAGE           0.3     // level * # = max damage of beam
#define LEAF_STORM_TIME             3.0     //how long leaf storm poisons for

#define LEECH_SEED                  5       //how much damage and health is returned
#define LEECH_SEED_TIME             5.0     //how often (in seconds) leech seed occurs

#define LEECHLIFE                   1       //how much damage does leechlife damage and heal for

#define LOW_KICK                    20      //how much damage low kick does

#define MEGA_DRAIN_LOW              80     //percent of ammo player drains at lowest level
#define MEGA_DRAIN_HIGH             50      //percent of ammo player drains at highest level

#define MEGA_KICK                   400     //how much force is behind mega kick
#define MEGA_KICK_UP                400     //how high will mega kick kick them
#define MEGA_KICK_DAMAGE            40      //how much damage mega kick does

#define MEGA_PUNCH                  1500    //how much force is behind mega punch
#define MEGA_PUNCH_DAMAGE           40      //how much damage mega punch does

#define NIGHTSHADE_MAX_INVIS        200     //max visibility of someone with nightshade (should be higher than MIN_INVIS and range from 1-255)
#define NIGHTSHADE_MIN_INVIS        30       //min visibility of someone with nightshade (should be lower than MAX_INVIS and range from 0-254)

#define PECK                        1       //how much damage does peck do to entities

#define PIN_MISSLE_DAMAGE           50      //how much damage pin missle does
#define PIN_MISSLE_VEC              2000    //how fast pin missle shoots

#define PSYBEAM_SPEED               500     //how fast a psybeam moves
#define PSYBEAM_DAMAGE              35      //max damage psybeam can do

//don't forget this can be used 5 times
#define PSYCHIC_DAMAGE              10      //max damage psychic can do
#define PSYCHIC_RANGE               500     //starting range of psychic
#define PSYCHIC_DELAY               2.0     //skill delay forced on players

#define QUICK_ATTACK                150     //amount of boost quick attack gives
#define RAGE_FACTOR                 5       //every # damage, rage will go up by 1
#define RAGE_MAX                    20      //max rage you can have

#define RAZOR_LEAF_FACTOR           2       //this will be multiplied by the players level for razor leaf damage

#define REFLECT_ACTIVE              70      //additional chance that reflect will work if they have the pokemon active

#define REST                        1       //HP given every 1 seconds

#define ROCK_THROW_VEC              2000    //how hard rock throw and fire blast will go

#define ROCK_DAMAGE                 80     //max damage that boulders can do

#define ROCK_SLIDE_LEVEL            10      //every # of levels, one more boulder will fall

#define SACRED_RADIUS          1.0     // level * # = radius of beam
#define SACRED_DAMAGE          0.7     // level * # = max damage of beam

#define SCREECH_RADIUS              750     //level + # = radius of screech

#define SEISMIC_TOSS                1000    //how much force is behind seismic toss
#define SEISMIC_TOSS_DAMAGE         20      //how much damage seismic toss does

#define SING_RADIUS                 600     //level + # = radius of sing
#define SING_SLEEP                  3.5     //how long people fall asleep for

#define SHOCK_WAVE_DAMAGE           2       //level * # = max damage
#define SHOCK_WAVE_RADIUS           250     //level + # = radius of max damage

#define SKY_ATTACK_DAMAGE           2       //level * # = max damage
#define SKY_ATTACK_RADIUS           200     //level + # = radius of max damage

#define SONIC_BOOM_DAMAGE           2       //level * # = max damage
#define SONIC_BOOM_RADIUS           500     //level + # = radius of max damage

#define SPIDER_WEB_TIME             5.0    //how long spider web slows a user

#define SPIKE_DAMAGE                5       //how much damage each spike does for spike cannon 

#define SPORE_TIME                  5       //how many seconds spore lasts by default
#define SPORE_RANGE                 200     //range spore has to affect players
#define SPORE_SOLID                 0       //spore is solid and blocks players from going through it

#define STOMP_RADIUS                200     //level + # = radius of stomp

#define STRENGTH_DIST               150     //how close to an object someone has to be/how far an object will be moved from the player
#define STRENGTH_DAMAGE             1       //damage done to players being grabbed every tenth of a second (2 = 20 damage every second)
#define STRENGTH_NPC                0       //can players move NPCs (Prof. Oak)

#define STRING_SHOT_TIME            5.0    //how long string shot slows a user

#define SUBMISSION_TIME             4.0     //how long submission paralyzes for

#define SUBSTITUTE                  3       //HP has to be below maxhp/# in order to substitute

#define SUPER_FANG                  2       //knife attacks will do (maxhp of player) / #

#define SURF_DISTANCE               200     //how close players are effected by a surf wave
#define SURF_DUCK                   1       //players ducking are not affected by surf waves (1=not affected; 0=affected)

#define TAIL_WHIP                   250     //distance that tailwhip affects

#define TAKE_DOWN_TIME              7.0    //how long take-down paralyzes for

#define TELEPORT_TIME               2.0     //seconds to wait before teleporting

#define THRASH                      400     //how crazy thrash shakes screens
#define THRASH_DAMAGE               5       //damage for being thrashed

#define THUNDER_RADIUS              1000    // radius of sky thunders
#define THUNDER_DAMAGE			1	  // level / # = damage from large thunder
#define THUNDER_RADIUS2             500     // radius of rebound thunders
#define THUNDER_DAMAGE2			4		// level / # = damage from rebound thunder
#define THUNDER_KILL                1       // can thunder kill players?

#define THUNDER_PUNCH_TIME          5.0    //how long status punches are

#define THUNDER_WAVE_TIME           3.0     //how long thunder wave paralyzes for

#define THUNDERBOLT_DISTANCE        1000    //how close players have to be to get electrocuted

#define THUNDERSHOCKS               5      // level / # = how many times a thundershock activates

#define TRANSFORM_ALPHA             30      //transparency when not a human

#define TRIPLE_KICK                 600     //how much force is behind mega kick
#define TRIPLE_KICK_UP              600     //how high will mega kick kick them
#define TRIPLE_KICK_DAMAGE          35      //how much damage mega kick does

#define VICE_GRIP                   3.0     //how long players are paralyzed

#define VINE_WHIP_JUMP              300.0   //how high vine whip pushes you into the sky
#define VINE_WHIP_DAMAGE            25      //how much damage it does

#define WATER_GUN_DAMAGE            6.0     // max damage (don't forget that this can be fired 10 times per second)
#define WATER_GUN_NUM               40      // level + # = waterguns to start with

#define WATERFALL_TIME              5       //how many seconds waterfall lasts by default
#define WATERFALL_RANGE             200     //range of waterfall

#define WHIRLWIND_FACTOR            10      // level / # = damage
#define WHIRLWIND_VELOCITY          50      // whirlwind damage * # = final velocity of whirlwind

#define WITHDRAW                    2       // level * # = how much armor to repair

#define POISON_POWDERS              7       //how many poison powders are created
#define SLEEP_POWDERS               5       //how many sleep powders are created
#define COTTON_SPORES               7       //how many cotton spores are created
#define POWDER_TIME                 5      //how long a powder lasts
#define POWDER_RANGE                100     //how far does powder affect people?

#define SLAP                        3       //level * # = slap initial (before factoring in which skill)
#define DOUBLE_SLAP                 40      //slap initial / # = damage
#define BODY_SLAM                   35      //""    ""        ""        ""
#define SKULLBASH                   30      //""    ""        ""        ""
