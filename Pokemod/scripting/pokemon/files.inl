 /////////////////////////
 //--Read Config Files--//
 /////////////////////////
 SetupConfig()
 {
	//Set Up Config Files
	new PokeDir[128];
	Poke_FileName( F_MAX, PokeDir );		//refresh the static config dir in Poke_FileName
	Poke_FileName( F_PokeDir, PokeDir );
	if( !dir_exists(PokeDir) )
		mkdir(PokeDir);
 }
 Poke_FileName( PokeFiles:filenum, _File[], len=127 )
 {
	static PokeDir[128];
	switch( filenum )
	{
		case F_MAX:{
							get_configsdir(PokeDir,127);
							add(PokeDir,127,"/pokemon");
		}
		case F_PokeDir:		formatex(_File,len,	"%s/"				,PokeDir);
		case F_Skill:		formatex(_File,len,	"%s/skill.ini"		,PokeDir);
		case F_ModConfig:	formatex(_File,len,	"%s/pokemod.cfg"	,PokeDir);
		case F_Pokemon:		formatex(_File,len,	"%s/pokemon.ini"	,PokeDir);
		case F_Items:		formatex(_File,len,	"%s/items.ini"		,PokeDir);
		case F_Safari:		formatex(_File,len,	"%s/safari.ini"		,PokeDir);
		case F_PokeMaps:	formatex(_File,len,	"%s/maps.ini"		,PokeDir);
		case F_XPs:			formatex(_File,len,	"%s/xp_table.ini"	,PokeDir);
		case F_Type:		formatex(_File,len,	"%s/types.txt"	,PokeDir);
	}
	return true;
 }
 public LoadConfig()
 {
	debugMessage( 0,7,"Config", "Looking for Config Files." );

	get_localinfo("amxx_vault",g_VaultFile,127);

	SetupConfig()

	new PokeFile[128];

	//Load Config Files
	Poke_FileName( F_ModConfig, PokeFile);
	if( file_exists(PokeFile) ) {
		server_cmd("exec %s",PokeFile);
		SearchCvars();
	}
	else {
		poke_error("**WARNING** PokeMod Config File not found, making default configs");
		ResetConfig();
	}

	Poke_FileName( F_Skill, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod Skills File not found, making default");
		ResetSkillInfo();
	}
	Poke_FileName( F_Pokemon, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod Pokemon Properties File not found, making default");
		ResetPokemon();
	}
	Poke_FileName( F_Items, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod Items File not found, making default");
		ResetPokemart();
	}
	Poke_FileName( F_Safari, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod Safari file not found, making default");
		ResetSafari();
	}
	Poke_FileName( F_Type, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod Type file not found, making default");
		ResetTypeFile();
	}
	Poke_FileName( F_XPs, PokeFile);
	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod XP table file not found, making default");
		ResetXPs();
	}
	debugMessage( 0,7,"Config", "Files Found/Reset." );
 }
 public ResetConfig()
 {
	new PokeFile[128];
	Poke_FileName( F_ModConfig, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile);

	write_file(PokeFile,	"//CVARS for PokeMod, these are loaded every map change",-1);
	write_file(PokeFile,	EMPTY, -1);

	for(new i=0; i<MAX_CVARS; i++)
		set_ppcvar_line(i, cvar_default[i]);

	write_file(PokeFile,	EMPTY, -1);
	write_file(PokeFile,	"echo \"PokeMod - Successfully Loaded PokeMod Config File\" ",-1);
 }

 public ResetPokemart()
 {
	new PokeFile[128];
	Poke_FileName( F_Items, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile);

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - Do not edit here unless you know what your doing, otherwise use pm_pokemart\n" );
	fputs(f,	"// Price = 0 (No one can buy) Price = -# (Only Admin can buy)\n" );
	fputs(f,	"// To make an item Group, place the group name in {} before the items\n" );
	fputs(f,	"// Put items then it's price, then it's properties, then it's description in quotes.\n" );
	fputs(f,	"// Put pokemon name with a dash \"-\" in front then it's price.\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"// Item Properties:\n" );
	fputs(f,	"//  \"a\" - heal item - followed by how much to heal. use -1 for full health\n" );
	fputs(f,	"//  \"b\" - catch item - followed by chance to catch\n" );
	fputs(f,	"//  \"c\" - removes all statuses\n" );
	fputs(f,	"//  \"d\" - removes burn status\n" );
	fputs(f,	"//  \"e\" - removes frozen status\n" );
	fputs(f,	"//  \"f\" - removes paralyze status\n" );
	fputs(f,	"//  \"g\" - removes asleep status\n" );
	fputs(f,	"//  \"h\" - removes slow status\n" );
	fputs(f,	"//  \"i\" - removes poison status\n" );
	fputs(f,	"//  \"j\" - teleports you to spawn\n" );
	fputs(f,	"//  \"k\" - give item to a pokemon (for evolution)\n" );
	fputs(f,	"//  \"l\" - extra pokedamage - followed by additional damage\n" );
	fputs(f,	"//  \"m\" - extra damage - followed by additional damage\n" );
	fputs(f,	"//  \"n\" - extra armor - followed by additional armor\n" );
	fputs(f,	"//  \"o\" - extra speed - followed by additional speed\n" );
	fputs(f,	"//  \"p\" - extra maxhp - followed by additional maxhp\n" );
	fputs(f,	"//  \"q\" - can't be bought\n" );
	fputs(f,	"//  \"r\" - evolution - followed by pokemon to evolve into\n" );
	fputs(f,	"//  \"s\" - levels a pokemon\n" );
	fputs(f,	"//  \"t\" - item trader - followed by item to recieve\n" );
	fputs(f,	"//  \"u\" - bicycle - followed by additional speed\n" );
	fputs(f,	"//  \"v\" - silph scope - allows you to see ghost pokemon\n" );
	fputs(f,	NEWLINE );
/*	fputs(f,	"//  \"w\" - skill - followed by the skill it will teach\n" );*/
	fputs(f,	"//  \"x\" - no drop - players don't drop this item when they die\n" );
	fputs(f,	"//  \"y\" - max amount - followed by max amount of this item players can have\n" );
	fputs(f,	"//  \"z\" - disable default use - disables default pokemod use of an item\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"// Examples:\n" );
	fputs(f,	"//  PokeBall 200 b 20 \"Catches low level pokemon\"\n" );
	fputs(f,	"//  Master_Ball -16000 b 1000 x y 1 \"Automatically catches pokemon\"\n" );
	fputs(f,	"//  Potion 300 a 20 \"Heals 20 health\"\n" );
	fputs(f,	"//  Antidote 100 i \"Heals poison status\"\n" );
	fputs(f,	"//  Full_Restore 3000 a -1 c \"Heals all status and health\"\n" );
	fputs(f,	"//  Calcium 9800 l 10 \"Extra skill damage\"\n" );
	fputs(f,	"//  Fire_Stone 2100 k \"Evolves certain fire pokemon\"\n" );
	fputs(f,	"//  Nugget 10000 q \"Sell for $5000\"\n" );
	fputs(f,	"//  -Bulbasaur 16000\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"{Pokeballs}\n" );
	fputs(f,	"PokeBall 200 b 20 \"Catches low level pokemon\"\n" );
	fputs(f,	"Great_Ball 600 b 40 \"Catches medium level pokemon\"\n" );
	fputs(f,	"Ultra_Ball 1200 b 80 \"Catches high level pokemon\"\n" );
	fputs(f,	"Master_Ball -16000 b 1000 x y 1 \"Automatically catches pokemon\"\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"{Items}\n" );
	fputs(f,	"Potion 300 a 20 \"Heals 20 health\"\n" );
	fputs(f,	"Antidote 100 i \"Heals poison status\"\n" );
	fputs(f,	"Awakening 200 g \"Heals sleep status\"\n" );
	fputs(f,	"Burn_Heal 200 d \"Heals burn status\"\n" );
	fputs(f,	"Ice_Heal 250 e \"Heals frozen status\"\n" );
	fputs(f,	"Paralyze_Heal 250 f \"Heals paralysis and slow status\"\n" );
	fputs(f,	"Escape_Rope 550 j \"Teleports to spawn\"\n" );
	fputs(f,	"Full_Heal 600 c \"Heals all status\"\n" );
	fputs(f,	"Super_Potion 700 a 70 \"Heals 70 health\"\n" );
	fputs(f,	"Full_Restore 3000 a -1 c \"Heals all status and health\"\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"{Boosts}\n" );
	fputs(f,	"Calcium 9800 l 10 \"Extra skill damage\"\n" );
	fputs(f,	"Carbos 9800 o 50 \"Extra speed\"\n" );
	fputs(f,	"HP_Up 9800 p 25 \"Extra max health\"\n" );
	fputs(f,	"Iron 9800 n 50 \"Extra armor\"\n" );
	fputs(f,	"Protein 9800 m 5 \"Extra damage\"\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"{Stones}\n" );
	fputs(f,	"Fire_Stone 2100 k \"Evolves certain fire pokemon\"\n" );
	fputs(f,	"Leaf_Stone 2100 k \"Evolves certain grass pokemon\"\n" );
	fputs(f,	"Moon_Stone 2100 k \"Evolves certain psychic pokemon\"\n" );
	fputs(f,	"Thunder_Stone 2100 k \"Evolves certain electric pokemon\"\n" );
	fputs(f,	"Water_Stone 2100 k \"Evolves certain water pokemon\"\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"{Other}\n" );
	fputs(f,	"Nugget 10000 q \"Sell for $5000\"\n" );
	fputs(f,	"Old_Amber 5000 q r aerodactyl \"Fossilized aerodactyl\"\n" );
	fputs(f,	"Rare_Candy 16000 k q s \"Levels up pokemon\"\n" );
	fputs(f,	"Bicycle 0 q u 200 \"Run extra fast\"\n" );
	fputs(f,	"Bike_Voucher 5000 q t Bicycle \"Exchange for a free bike\"\n" );
	fputs(f,	"Silph_Scope 5000 v \"See ghost pokemon easier\"\n" );
	fputs(f,	NEWLINE );
/*
	fputs(f,	"{Technical Machines}\n" );
	fputs(f,	"TM_01 0 q w Mega_Punch\n" );
	fputs(f,	"TM_02 0 q w Razor_Wind\n" );
	fputs(f,	"TM_03 0 q w Swords_Dance\n" );
	fputs(f,	"TM_04 0 q w Whirlwind\n" );
	fputs(f,	"TM_05 0 q w Mega_Kick\n" );
	fputs(f,	"TM_06 0 q w Toxic\n" );
	fputs(f,	"TM_07 0 q w Horn_Drill\n" );
	fputs(f,	"TM_08 0 q w Body_Slam\n" );
	fputs(f,	"TM_09 0 q w Take_Down\n" );
	fputs(f,	"TM_10 0 q w Double_Edge\n" );
	fputs(f,	"TM_11 0 q w BubbleBeam\n" );
	fputs(f,	"TM_12 0 q w Water_Gun\n" );
	fputs(f,	"TM_13 0 q w Ice_Beam\n" );
	fputs(f,	"TM_14 0 q w Blizzard\n" );
	fputs(f,	"TM_15 0 q w Hyper_Beam\n" );
	fputs(f,	"TM_16 0 q w Pay_Day\n" );
	fputs(f,	"TM_17 0 q w Submission\n" );
	fputs(f,	"TM_18 0 q w Counter\n" );
	fputs(f,	"TM_19 0 q w Seismic_Toss\n" );
	fputs(f,	"TM_20 0 q w Rage\n" );
	fputs(f,	"TM_21 0 q w Mega_Drain\n" );
	fputs(f,	"TM_22 0 q w Solarbeam\n" );
	fputs(f,	"TM_23 0 q w Dragon_Rage\n" );
	fputs(f,	"TM_24 0 q w Thunderbolt\n" );
	fputs(f,	"TM_25 0 q w Thunder\n" );
	fputs(f,	"TM_26 0 q w Earthquake\n" );
	fputs(f,	"TM_27 0 q w Fissure\n" );
	fputs(f,	"TM_28 0 q w Dig\n" );
	fputs(f,	"TM_29 0 q w Psychic\n" );
	fputs(f,	"TM_30 0 q w Teleport\n" );
	fputs(f,	"TM_31 0 q w Mimic\n" );
	fputs(f,	"TM_32 0 q w Double_Team\n" );
	fputs(f,	"TM_33 0 q w Reflect\n" );
	fputs(f,	"TM_34 0 q w Bide\n" );
	fputs(f,	"TM_35 0 q w Metronome\n" );
	fputs(f,	"TM_36 0 q w Self_Destruct\n" );
	fputs(f,	"TM_37 0 q w Egg_Bomb\n" );
	fputs(f,	"TM_38 0 q w Fire_Blast\n" );
	fputs(f,	"TM_39 0 q w Swift\n" );
	fputs(f,	"TM_40 0 q w Skull_Bash\n" );
	fputs(f,	"TM_41 0 q w Softboiled\n" );
	fputs(f,	"TM_42 0 q w Dream_Eater\n" );
	fputs(f,	"TM_43 0 q w Sky_Attack\n" );
	fputs(f,	"TM_44 0 q w Rest\n" );
	fputs(f,	"TM_45 0 q w Thunder_Wave\n" );
	fputs(f,	"TM_46 0 q w Psywave\n" );
	fputs(f,	"TM_47 0 q w Explosion\n" );
	fputs(f,	"TM_48 0 q w Rock_Slide\n" );
	fputs(f,	"TM_49 0 q w Tri_Attack\n" );
	fputs(f,	"TM_50 0 q w Substitute\n" );
	fputs(f,	"{Hidden Machines}\n" );
	fputs(f,	"HM_01 0 q w Cut x\n" );
	fputs(f,	"HM_02 0 q w Fly x\n" );
	fputs(f,	"HM_03 0 q w Surf x\n" );
	fputs(f,	"HM_04 0 q w Strength x\n" );
	fputs(f,	"HM_05 0 q w Flash x\n" );
	fputs(f,	NEWLINE );
*/
	fputs(f,	"-Bulbasaur 16000\n" );
	fputs(f,	"-Charmander 16000\n" );
	fputs(f,	"-Squirtle 16000\n" );
	fputs(f,	"-Pikachu 16000\n" );

	fclose(f);
 }
 public ResetTypeFile()
 {
	new PokeFile[128];
	Poke_FileName( F_Type, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile)

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - There is nothing to edit in this file\n" );
	fputs(f,	"// Note2 - These are the type flags for pokemon and skills\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"//  \"a\" - Normal\n" );
	fputs(f,	"//  \"b\" - Fire\n" );
	fputs(f,	"//  \"c\" - Water\n" );
	fputs(f,	"//  \"d\" - Electric\n" );
	fputs(f,	"//  \"e\" - Grass\n" );
	fputs(f,	"//  \"f\" - Ice\n" );
	fputs(f,	"//  \"g\" - Fighting\n" );
	fputs(f,	"//  \"h\" - Poison\n" );
	fputs(f,	"//  \"i\" - Ground\n" );
	fputs(f,	"//  \"j\" - Flying\n" );
	fputs(f,	"//  \"k\" - Psychic\n" );
	fputs(f,	"//  \"l\" - Bug\n" );
	fputs(f,	"//  \"m\" - Rock\n" );
	fputs(f,	"//  \"n\" - Ghost\n" );
	fputs(f,	"//  \"o\" - Dragon\n" );

	fclose(f);
 }
 public ResetPokemon()
 {
	new PokeFile[128];
	Poke_FileName( F_Pokemon, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile)

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - Do not edit here unless you know what your doing\n" );
	fputs(f,	"// Put pokemon name, then its number, then its skill in quotes, then it's properties, then evolutions.\n" );
	fputs(f,	"// For types, check types.txt\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"// Pokemon Properties:\n" );

	fputs(f,	"//  \"a\" - isn't an evolved form of another pokemon\n" );
	fputs(f,	"//  \"b\" - evolves - followed by level number, TRADE, or item name\n" );
	fputs(f,	"//                  - then followed by the pokemon it should evolve into\n" );
	fputs(f,	"//  \"c\" - cannot be found in pokeballs on the ground\n" );
	fputs(f,	"//  \"d\" - can be selected as a starting pokemon\n" );
	fputs(f,	"//  \"e\" - cannot be caught by pokeballs\n" );
	fputs(f,	"//  \"g\" - global - doesn't have to be active to use skill\n" );
	fputs(f,	"//  \"h\" - heavy - some skills will not work against heavy pokemon\n" );
	fputs(f,	"//  \"y\" - special - pokemon follows special flags for capturing\n" );
	fputs(f,	"//  \"z\" - disable use\n" );

	fputs(f,	NEWLINE );
	fputs(f,	"// Example:\n" );
	fputs(f,	"//#   Name Type_Flags \"Skill_Name\" Pokemon_Properties\n" );
	fputs(f,	"//1   Bulbasaur eh \"Leech_Seed\" abcd 16 Ivysaur\n" );
	//"Number name type attack props evolution"

	fputs(f,	NEWLINE );
	fputs(f,	"1   Bulbasaur eh \"Leech_Seed\" abcd 16 Ivysaur\n" );
	fputs(f,	"2   Ivysaur eh \"Growth\" bc 32 Venusaur\n" );
	fputs(f,	"3   Venusaur eh \"Solar_Beam\" ch\n" );
	fputs(f,	"4   Charmander b \"Rage\" abcd 16 Charmeleon\n" );
	fputs(f,	"5   Charmeleon b \"Counter\" bc 36 Charizard\n" );
	fputs(f,	"6   Charizard bj \"Fire_Blast\" c\n" );
	fputs(f,	"7   Squirtle c \"Bubblebeam\" abcd 16 Wartortle\n" );
	fputs(f,	"8   Wartortle c \"Water_Gun\" bc 36 Blastoise\n" );
	fputs(f,	"9   Blastoise c \"Hydro_Pump\" ch\n" );
	fputs(f,	"10  Caterpie l \"String_Shot\" ab 7 Metapod\n" );
	fputs(f,	"11  Metapod l \"Harden\" bc 10 Butterfree\n" );
	fputs(f,	"12  Butterfree lj \"Substitute\" c\n" );
	fputs(f,	"13  Weedle lh \"Poison_Sting\" ab 7 Kakuna\n" );
	fputs(f,	"14  Kakuna lh \"Harden\" bc 10 Beedrill\n" );
	fputs(f,	"15  Beedrill lh \"Pin_Missile\" c\n" );
	fputs(f,	"16  Pidgey aj \"Gust\" ab 18 Pidgeotto\n" );
	fputs(f,	"17  Pidgeotto aj \"Whirlwind\" bc 36 Pidgeot\n" );
	fputs(f,	"18  Pidgeot aj \"Fly\" c\n" );
	fputs(f,	"19  Rattata a \"Hyper_Fang\" ab 20 Raticate\n" );
	fputs(f,	"20  Raticate a \"Super_Fang\" c\n" );
	fputs(f,	"21  Spearow j \"Peck\" ab 20 Fearow\n" );
	fputs(f,	"22  Fearow aj \"Drill_Peck\" c\n" );
	fputs(f,	"23  Ekans h \"Wrap\" ab 22 Arbok\n" );
	fputs(f,	"24  Arbok h \"Glare\" c\n" );
	fputs(f,	"25  Pikachu d \"Quick_Attack\" abcd Thunder_Stone Raichu\n" );
	fputs(f,	"26  Raichu d \"Thunderbolt\" c\n" );
	fputs(f,	"27  Sandshrew i \"Sand_Attack\" ab 22 Sandslash\n" );
	fputs(f,	"28  Sandslash i \"Swift\" c\n" );
	fputs(f,	"29  NidoranF h \"Tackle\" ab 16 Nidorina\n" );
	fputs(f,	"30  Nidorina h \"Double_Kick\" bc Moon_Stone Nidoqueen\n" );
	fputs(f,	"31  Nidoqueen hi \"Body_Slam\" ch\n" );
	fputs(f,	"32  NidoranM h \"Horn_Attack\" ab 16 Nidorino\n" );
	fputs(f,	"33  Nidorino h \"Horn_Drill\" bc Moon_Stone Nidoking\n" );
	fputs(f,	"34  Nidoking hi \"Strength\" ch\n" );
	fputs(f,	"35  Clefairy a \"Submission\" ab Moon_Stone Clefable\n" );
	fputs(f,	"36  Clefable a \"Take_Down\" c\n" );
	fputs(f,	"37  Vulpix b \"Tail_Whip\" ab Fire_Stone Ninetales\n" );
	fputs(f,	"38  Ninetales b \"Ember\" c\n" );
	fputs(f,	"39  Jigglypuff a \"Sing\" ab Moon_Stone Wigglytuff\n" );
	fputs(f,	"40  Wigglytuff a \"Sing\" c\n" );
	fputs(f,	"41  Zubat hj \"Leech_Life\" ab 22 Golbat\n" );
	fputs(f,	"42  Golbat hj \"Bite\" c\n" );
	fputs(f,	"43  Oddish eh \"Absorb\" ab 21 Gloom\n" );
	fputs(f,	"44  Gloom eh \"Stun_Spore\" bc Leaf_Stone Vileplume\n" );
	fputs(f,	"45  Vileplume eh \"Mega_Drain\" c\n" );
	fputs(f,	"46  Paras le \"Spore\" ab 24 Parasect\n" );
	fputs(f,	"47  Parasect le \"Growth\" c\n" );
	fputs(f,	"48  Venonat lh \"Poison_Powder\" ab 30 Venomoth\n" );
	fputs(f,	"49  Venomoth lh \"Sleep_Powder\" c\n" );
	fputs(f,	"50  Diglett i \"Dig\" ab 28 Dugtrio\n" );
	fputs(f,	"51  Dugtrio i \"Earthquake\" c\n" );
	fputs(f,	"52  Meowth a \"Pay_Day\" abd 28 Persian\n" );
	fputs(f,	"53  Persian a \"Scratch\" c\n" );
	fputs(f,	"54  Psyduck c \"Confusion\" ab 33 Golduck\n" );
	fputs(f,	"55  Golduck c \"Fury_Swipes\" c\n" );
	fputs(f,	"56  Mankey g \"Low_Kick\" ab 26 Primeape\n" );
	fputs(f,	"57  Primeape g \"Thrash\" c\n" );
	fputs(f,	"58  Growlithe b \"Roar\" ab Fire_Stone Arcanine\n" );
	fputs(f,	"59  Arcanine b \"Flamethrower\" c\n" );
	fputs(f,	"60  Poliwag c \"Double_Slap\" ab 25 Poliwhirl\n" );
	fputs(f,	"61  Poliwhirl c \"Body_Slam\" bc Water_Stone Poliwrath\n" );
	fputs(f,	"62  Poliwrath cg \"Skullbash\" ch\n" );
	fputs(f,	"63  Abra k \"Teleport\" ab 16 Kadabra\n" );
	fputs(f,	"64  Kadabra k \"Psybeam\" bc TRADE Alakazam\n" );
	fputs(f,	"65  Alakazam k \"Reflect\" c\n" );
	fputs(f,	"66  Machop g \"Karate_Chop\" ab 28 Machoke\n" );
	fputs(f,	"67  Machoke g \"Low_Kick\" bc TRADE Machamp\n" );
	fputs(f,	"68  Machamp g \"Seismic_Toss\" c\n" );
	fputs(f,	"69  Bellsprout eh \"Vine_Whip\" ab 21 Weepinbell\n" );
	fputs(f,	"70  Weepinbell eh \"Acid\" bc Leaf_Stone Victreebel\n" );
	fputs(f,	"71  Victreebel eh \"Razor_Leaf\" c\n" );
	fputs(f,	"72  Tentacool ch \"Acid\" ab 30 Tentacruel\n" );
	fputs(f,	"73  Tentacruel ch \"Constrict\" c\n" );
	fputs(f,	"74  Geodude mi \"Defense_Curl\" ab 25 Graveler\n" );
	fputs(f,	"75  Graveler mi \"Harden\" bc TRADE Golem\n" );
	fputs(f,	"76  Golem mi \"Rock_Slide\" ch\n" );
	fputs(f,	"77  Ponyta b \"Stomp\" ab 40 Rapidash\n" );
	fputs(f,	"78  Rapidash b \"Agility\" c\n" );
	fputs(f,	"79  Slowpoke ck \"Confusion\" ab 37 Slowbro\n" );
	fputs(f,	"80  Slowbro ck \"Amnesia\" c\n" );
	fputs(f,	"81  Magnemite d \"Sonic_Boom\" ab 30 Magneton\n" );
	fputs(f,	"82  Magneton d \"Thunder_Wave\" c\n" );
	fputs(f,	"83  Farfetchd aj \"Leer\" a\n" );
	fputs(f,	"84  Doduo aj \"Double_Edge\" ab 31 Dodrio\n" );
	fputs(f,	"85  Dodrio aj \"Tri_Attack\" c\n" );
	fputs(f,	"86  Seel c \"Aurora_Beam\" ab 37 Dewgong\n" );
	fputs(f,	"87  Dewgong cf \"Ice_Beam\" c\n" );
	fputs(f,	"88  Grimer h \"Poison_Gas\" ab 35 Muk\n" );
	fputs(f,	"89  Muk h \"Acid_Armor\" c\n" );
	fputs(f,	"90  Shellder c \"Withdraw\" ab Water_Stone Cloyster\n" );
	fputs(f,	"91  Cloyster cf \"Spike_Cannon\" c\n" );
	fputs(f,	"92  Gastly nh \"Night_Shade\" ab 25 Haunter\n" );
	fputs(f,	"93  Haunter nh \"Hypnosis\" bc TRADE Gengar\n" );
	fputs(f,	"94  Gengar nh \"Dream_Eater\" c\n" );
	fputs(f,	"95  Onix mi \"Rock_Throw\" a\n" );
	fputs(f,	"96  Drowzee k \"Disable\" ab 28 Hypno\n" );
	fputs(f,	"97  Hypno k \"Confusion\" c\n" );
	fputs(f,	"98  Krabby c \"Vice_Grip\" ab 28 Kingler\n" );
	fputs(f,	"99  Kingler c \"Crabhammer\" c\n" );
	fputs(f,	"100 Voltorb d \"Self_Destruct\" ab 33 Electrode\n" );
	fputs(f,	"101 Electrode d \"Explosion\" c\n" );
	fputs(f,	"102 Exeggcute ek \"Barrage\" ab Leaf_Stone Exeggutor\n" );
	fputs(f,	"103 Exeggutor ek \"Solar_Beam\" c\n" );
	fputs(f,	"104 Cubone i \"Bone_Club\" ab 28 Marowak\n" );
	fputs(f,	"105 Marowak i \"Bonemerang\" c\n" );
	fputs(f,	"106 Hitmonlee g \"Mega_Kick\" a\n" );
	fputs(f,	"107 Hitmonchan g \"Mega_Punch\" a\n" );
	fputs(f,	"108 Lickitung a \"Screech\" a\n" );
	fputs(f,	"109 Koffing h \"Smog\" ab 35 Weezing\n" );
	fputs(f,	"110 Weezing h \"Toxic\" c\n" );
	fputs(f,	"111 Rhyhorn im \"Fury_Attack\" ab 42 Rhydon\n" );
	fputs(f,	"112 Rhydon im \"Fissure\" c\n" );
	fputs(f,	"113 Chansey a \"Egg_Bomb\" a\n" );
	fputs(f,	"114 Tangela e \"Absorb\" a\n" );
	fputs(f,	"115 Kangaskhan a \"Dizzy_Punch\" a\n" );
	fputs(f,	"116 Horsea c \"Bubble\" ab 30 Seadra\n" );
	fputs(f,	"117 Seadra c \"Leer\" c\n" );
	fputs(f,	"118 Goldeen c \"Supersonic\" ab 33 Seaking\n" );
	fputs(f,	"119 Seaking c \"Waterfall\" c\n" );
	fputs(f,	"120 Staryu c \"Recover\" ab Water_Stone Starmie\n" );
	fputs(f,	"121 Starmie ck \"Light_Screen\" c\n" );
	fputs(f,	"122 MrMime k \"Barrier\" a\n" );
	fputs(f,	"123 Scyther lj \"Double_Team\" a\n" );
	fputs(f,	"124 Jynx fk \"Blizzard\" a\n" );
	fputs(f,	"125 Electabuzz d \"Flash\" a\n" );
	fputs(f,	"126 Magmar b \"Smokescreen\" a\n" );
	fputs(f,	"127 Pinsir l \"Guillotine\" a\n" );
	fputs(f,	"128 Tauros a \"Stomp\" a\n" );
	fputs(f,	"129 Magikarp c \"Splash\" ab 20 Gyarados\n" );
	fputs(f,	"130 Gyarados cj \"Surf\" c\n" );
	fputs(f,	"131 Lapras cf \"Confuse_Ray\" a\n" );
	fputs(f,	"132 Ditto a \"Transform\" a\n" );
	fputs(f,	"133 Eevee a \"Tackle\" ab Water_Stone Vaporeon Thunder_Stone Jolteon Fire_Stone Flareon\n" );
	fputs(f,	"134 Vaporeon c \"Haze\" c\n" );
	fputs(f,	"135 Jolteon d \"Thundershock\" c\n" );
	fputs(f,	"136 Flareon b \"Fire_Spin\" c\n" );
	fputs(f,	"137 Porygon a \"Conversion\" a\n" );
	fputs(f,	"138 Omanyte mc \"Withdraw\" abc 40 Omastar\n" );
	fputs(f,	"139 Omastar mc \"Bide\" c\n" );
	fputs(f,	"140 Kabuto mc \"Scratch\" abc 40 Kabutops\n" );
	fputs(f,	"141 Kabutops mc \"Slash\" c\n" );
	fputs(f,	"142 Aerodactyl mj \"Agility\" ac\n" );
	fputs(f,	"143 Snorlax a \"Rest\" ah\n" );
	fputs(f,	"144 Articuno fj \"Mist\" acy\n" );
	fputs(f,	"145 Zapdos dj \"Thunder\" acy\n" );
	fputs(f,	"146 Moltres bj \"Sky_Attack\" acy\n" );
	fputs(f,	"147 Dratini o \"Thunder_Wave\" ab 30 Dragonair\n" );
	fputs(f,	"148 Dragonair o \"Dragon_Rage\" bc 55 Dragonite\n" );
	fputs(f,	"149 Dragonite o \"Hyper_Beam\" ch\n" );
	fputs(f,	"150 Mewtwo k \"Psywave\" acy\n" );
	fputs(f,	"151 Mew k \"Psychic\" acy\n" );

	fclose(f);
 }
 public ResetSkillInfo()
 {
	new PokeFile[128];
	Poke_FileName( F_Skill, PokeFile);

	if( file_exists(PokeFile) )
		delete_file(PokeFile);

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - Do not edit here unless you know what your doing\n" );
	fputs(f,	"// Put skill name then it's type, then delay time, then properties.\n" );
	fputs(f,	"// For types, check types.txt\n" );
	fputs(f,	"// If no time specified, 0.0 will be used.\n" );

	fputs(f,	NEWLINE );
	fputs(f,	"//  Properties:\n" );
	fputs(f,	"//  \"a\" - skill called when someone attacks someone else\n" );
	fputs(f,	"//  \"b\" - skill called when someone presses a bind key\n" );
	fputs(f,	"//  \"c\" - skill called when someone touches something\n" );
	fputs(f,	"//  \"d\" - skill called when someone is damaged\n" );
	fputs(f,	"//  \"e\" - skill called every second\n" );
	fputs(f,	"//  \"f\" - skill called during spawn\n" );
	fputs(f,	"//  \"g\" - skill called on death\n" );
	fputs(f,	"//  \"h\" - skill called no matter what\n" );
	fputs(f,	"//  \"i\" - skill ignores delay time\n" );
	fputs(f,	"//  \"j\" - skill called when someone attacks someone else with a pokeskill\n" );
	fputs(f,	"//  \"k\" - skill called when someone is damaged with a pokeskill\n" );
	fputs(f,	"//  \"l\" - skill called when someone looks at someone else\n" );
	fputs(f,	"//  \"m\" - skill called when someone changes speed\n" );
	fputs(f,	"//  \"q\" - skill called/reset at start of a new round\n" );
	fputs(f,	"//  \"r\" - skill called when player releases bind key\n" );
	fputs(f,	"//  \"s\" - skill called when player status changes\n" );
	fputs(f,	"//  \"t\" - skill needs to be reset\n" );
	fputs(f,	"//  \"u\" - DO NOT USE - for skill internal use only!\n" );
	fputs(f,	"//  \"v\" - skill called when player connects\n" );
	fputs(f,	"//  \"w\" - skill called when player disconnects\n" );
	fputs(f,	"//  \"x\" - skill is in another plugin (not pokemod)\n" );
	fputs(f,	"//  \"y\" - skill can only be used by admins\n" );
	fputs(f,	"//  \"z\" - disable default skill\n" );

	fputs(f,	NEWLINE );
	fputs(f,	"// Example:\n" );
	fputs(f,	"//   Leech_Seed e 10 bl\n" );
	//"Function type delay props"

//32 unmade skills
	fputs(f,	NEWLINE );
	fputs(f,	"Absorb e 0 a \"steal armor from enemies\"\n" );
	fputs(f,	"Acid h 0 a \"poison enemies\"\n" );
	fputs(f,	"Acid_Armor h 0 s \"protects against poison\"\n" );
	fputs(f,	"Agility k 0 m \"speed boost\"\n" );
	fputs(f,	"Amnesia k 15 b \"confuse nearby enemies\"\n" );
	fputs(f,	"Aurora_Beam f 15 b \"shoot a colorful beam that damages\"\n" );
	fputs(f,	"Barrage a 0 fi \"recieve extra grenades\"\n" );
	fputs(f,	"Barrier k 10 b \"place barriers that take damage\"\n" );
	fputs(f,	"Bide a 0 a \"deal extra damage based on your health\"\n" );
//	fputs(f,	"Bind a \"\"\n" );
	fputs(f,	"Bite a 0.5 c \"steal health from enemies\"\n" );
	fputs(f,	"Blizzard f 15 bt \"freeze enemies around you\"\n" );
	fputs(f,	"Body_Slam a 1 c \"damage enemies and toss them away\"\n" );
	fputs(f,	"Bone_Club i 0.5 b \"throw a bone\"\n" );
	fputs(f,	"Bonemerang i 0.5 b \"throw a bone that comes back to you\"\n" );
	fputs(f,	"Bubble c 0 e \"recieve extra oxygen in water\"\n" );
	fputs(f,	"Bubble_Beam c 1 bq \"shoot a beam of bubbles\"\n" );
//	fputs(f,	"Clamp c \"\"\n" );
//	fputs(f,	"Comet_Punch a \"\"\n" );
	fputs(f,	"Confuse_Ray n 10 b \"shoot a ray of light to confuse enemies\"\n" );
	fputs(f,	"Confusion k 15 b \"confuse nearby enemies\"\n" );
	fputs(f,	"Constrict a 0 a \"paralyze enemies\"\n" );
	fputs(f,	"Conversion a 10 bmr \"move slowly and harder to see\"\n" );
	fputs(f,	"Counter g 0 adq \"damage enemies with after being damaged\"\n" );
	fputs(f,	"Crabhammer c 5 a \"extra knife damage\"\n" );
//	fputs(f,	"Cut a \"\"\n" );
	fputs(f,	"Defense_Curl a 0 d \"gain armor\"\n" );
	fputs(f,	"Dig i 0.5 bm \"dig into the ground and walls\"\n" );
	fputs(f,	"Disable a 5 b \"delay enemies from using skills\"\n" );
	fputs(f,	"Dizzy_Punch a 5 c \"damage and shake enemies screens\"\n" );
	fputs(f,	"Double_Edge a 0 a \"gain extra backpack ammo\"\n" );
	fputs(f,	"Double_Kick g 0 b \"jump extremely high\"\n" );
	fputs(f,	"Double_Slap a 0.5 c \"damage enemies\"\n" );
	fputs(f,	"Double_Team a 15 b \"teleport to a damaged teammate\"\n" );
	fputs(f,	"Dragon_Rage o 5 a \"always deal at least 40 damage\"\n" );
	fputs(f,	"Dream_Eater k 1 bl \"steal health from sleeping enemies\"\n" );
	fputs(f,	"Drill_Peck j 0.5 c \"great damage\"\n" );
	fputs(f,	"Earthquake i 20 b \"shake the earth and damage enemies\"\n" );
	fputs(f,	"Egg_Bomb a 10 b \"change a grenade into a bomb you can detonate\"\n" );
	fputs(f,	"Ember b 10 b \"place rocks that emit fire\"\n" );
	fputs(f,	"Explosion a 0 g \"explode with heavy damage\"\n" );
	fputs(f,	"Fire_Blast b 5 b \"shoot a rock on fire\"\n" );
//	fputs(f,	"Fire_Punch b \"\"\n" );
	fputs(f,	"Fire_Spin b 9 b \"catch nearby enemies on fire\"\n" );
	fputs(f,	"Fissure i 20 a \"small chance to instant kill enemies\"\n" );
	fputs(f,	"Flamethrower b 15 b \"shoot flames that catch enemies on fire\"\n" );
	fputs(f,	"Flash a 0 a \"blind enemies\"\n" );
	fputs(f,	"Fly a 10 b \"get a boost into the air\"\n" );
//	fputs(f,	"Focus_Energy a \"\"\n" );
	fputs(f,	"Fury_Attack a 5 a \"extra knife damage based on armor\"\n" );
	fputs(f,	"Fury_Swipes a 5 a \"extra knife damage based on health\"\n" );
	fputs(f,	"Glare a 0 bl \"paralyze enemies\"\n" );
//	fputs(f,	"Growl a \"\"\n" );
	fputs(f,	"Growth a 0 ei \"decrease time between skill use\"\n" );
	fputs(f,	"Guillotine a 0 a \"chance to auto kill with knife\"\n" );
	fputs(f,	"Gust a 10 b \"damage and throw enemies in the air\"\n" );
	fputs(f,	"Harden a 5 b \"swap health for armor\"\n" );
	fputs(f,	"Haze f 5 s \"protects against status effects\"\n" );
//	fputs(f,	"Head_Butt a \"\"\n" );
//	fputs(f,	"Hi-Jump_Kick g \"\"\n" );
	fputs(f,	"Horn_Attack a 1 b \"shoot a horn\"\n" );
	fputs(f,	"Horn_Drill a 5 b \"shoot a powerful horn\"\n" );
	fputs(f,	"Hydro_Pump c 0 bq \"shoot two beams of water\"\n" );
	fputs(f,	"Hyper_Beam a 25 b \"shoot a powerful beam while paralyzed\"\n" );
	fputs(f,	"Hyper_Fang a 0 a \"extra knife damage\"\n" );
	fputs(f,	"Hypnosis k 15 b \"make enemies fall asleep\"\n" );
	fputs(f,	"Ice_Beam f 10 b \"shoot a beam of ice that freezes enemies\"\n" );
//	fputs(f,	"Ice_Punch f \"\"\n" );
//	fputs(f,	"Jump_Kick g \"\"\n" );
	fputs(f,	"Karate_Chop a 5 c \"damage and black out enemies\"\n" );
//	fputs(f,	"Kinesis k \"\"\n" );
	fputs(f,	"Leech_Life l 1 c \"steal health from enemies\"\n" );
	fputs(f,	"Leech_Seed e 10 bflt \"plant a seed that steals health from enemies\"\n" );
	fputs(f,	"Leer a 5 a \"remove all armor from enemies\"\n" );
//	fputs(f,	"Lick n \"\"\n" );
	fputs(f,	"Light_Screen k 0 k \"heal 1/2 damage\"\n" );
//	fputs(f,	"Lovely_Kiss a \"\"\n" );
	fputs(f,	"Low_Kick g 3 c \"damage enemies and make them flinch\"\n" );
//	fputs(f,	"Meditate k \"\"\n" );
	fputs(f,	"Mega_Drain e 5 a \"steal ammo from enemies\"\n" );
	fputs(f,	"Mega_Kick a 5 c \"kick enemies into the air\"\n" );
	fputs(f,	"Mega_Punch a 5 c \"punch enemies away from you\"\n" );
//	fputs(f,	"Metronome a \"\"\n" );
//	fputs(f,	"Mimic a \"\"\n" );
//	fputs(f,	"Minimize a \"\"\n" );
//	fputs(f,	"Mirror_Move a \"\"\n" );
	fputs(f,	"Mist f 10 b \"emit mist that heals you\"\n" );
	fputs(f,	"Night_Shade n 0 ei \"\"\n" );
	fputs(f,	"Pay_Day a 0 aj \"recieve extra money\"\n" );
	fputs(f,	"Peck j 1 c \"damage\"\n" );
//	fputs(f,	"Petal_Dance e \"\"\n" );
	fputs(f,	"Pin_Missile l 5 b \"shoot a missile that pushes enemies\"\n" );
	fputs(f,	"Poison_Gas h 0 e \"emit gas that poisons enemies\"\n" );
	fputs(f,	"Poison_Powder h 10 b \"spread powder that poisons enemies\"\n" );
	fputs(f,	"Poison_Sting h 0 d \"poison enemies\"\n" );
//	fputs(f,	"Pound a \"\"\n" );
	fputs(f,	"Psybeam k 10 b \"shoot and guide a powerful beam\"\n" );
	fputs(f,	"Psychic k 15 b \"damage and reveal nearby enemies\"\n" );
	fputs(f,	"Psywave k 0 a \"extra high damage\"\n" );
	fputs(f,	"Quick_Attack a 15 b \"boost of speed\"\n" );
	fputs(f,	"Rage a 0 adq \"deal extra damage that builds up\"\n" );
	fputs(f,	"Razor_Leaf e 10 b \"shoot leafs\"\n" );
//	fputs(f,	"Razor_Wind a \"\"\n" );
	fputs(f,	"Recover a 15 b \"regain health\"\n" );
	fputs(f,	"Reflect k 0 d \"reflect grenade damage away from you\"\n" );
	fputs(f,	"Rest k 0 b \"fall asleep to regain health\"\n" );
	fputs(f,	"Roar a 10 b \"make nearby enemies run out of ammo\"\n" );
	fputs(f,	"Rock_Slide m 15 b \"make boulders fall\"\n" );
	fputs(f,	"Rock_Throw m 5 b \"shoot a boulder\"\n" );
//	fputs(f,	"Rolling_Kick g \"\"\n" );
	fputs(f,	"Sand_Attack a 5 b \"shoot sand that blinds enemies\"\n" );
	fputs(f,	"Scratch a 0 d \"paralyze enemies\"\n" );
	fputs(f,	"Screech a 10 b \"delay nearby enemies\"\n" );
	fputs(f,	"Seismic_Toss g 5 c \"throw enemies great distances\"\n" );
	fputs(f,	"Self_Destruct a 0 b \"explode\"\n" );
//	fputs(f,	"Sharpen a \"\"\n" );
	fputs(f,	"Sing a 30 b \"put nearby enemies to sleep\"\n" );
	fputs(f,	"Skullbash a 0.5 c \"damage enemies and knock them away\"\n" );
	fputs(f,	"Sky_Attack j 15 b \"teleport into the air and then into an enemy\"\n" );
//	fputs(f,	"Slam a \"\"\n" );
	fputs(f,	"Slash a 3 a \"damage nearby enemies\"\n" );
	fputs(f,	"Sleep_Powder e 10 b \"spread powder that puts enemies to sleep\"\n" );
//	fputs(f,	"Sludge h \"\"\n" );
	fputs(f,	"Smog h 0 e \"emit gas that darkens enemies screens\"\n" );
	fputs(f,	"Smokescreen a 15 b \"shoot smoke that blinds enemies\"\n" );
//	fputs(f,	"Softboiled a \"\"\n" );
	fputs(f,	"Solar_Beam e 15 b \"shoot a powerful beam outside\"\n" );
	fputs(f,	"Sonic_Boom a 10 b \"damage nearby enemies\"\n" );
	fputs(f,	"Spike_Cannon a 0 brt \"shoot small spikes\"\n" );
	fputs(f,	"Splash a 1 b \"play in water\"\n" );
	fputs(f,	"Spore e 10 b \"place a spore that poisons enemies\"\n" );
	fputs(f,	"Stomp a 9 b \"paralyze nearby enemies\"\n" );
	fputs(f,	"Strength a 0 br \"grab and move things\"\n" );
	fputs(f,	"String_Shot l 0 a \"slow down enemies\"\n" );
//	fputs(f,	"Struggle a \"\"\n" );
	fputs(f,	"Stun_Spore e 7.5 b \"place a spore that stuns enemies\"\n" );
	fputs(f,	"Submission g 5 c \"paralyze enemies\"\n" );
	fputs(f,	"Substitute a 5 b \"teleport back to a spawn point\"\n" );
	fputs(f,	"Super_Fang a 0 a \"extra knife damage\"\n" );
	fputs(f,	"Supersonic a 10 b \"confuse nearby enemies\"\n" );
	fputs(f,	"Surf c 17.5 b \"shoot a wave of water\"\n" );
	fputs(f,	"Swift a 10 b \"teleport into enemies\"\n" );
//	fputs(f,	"Swords_Dance a \"\"\n" );
	fputs(f,	"Tackle a 0 b \"jump boost\"\n" );
	fputs(f,	"Tail_Whip a 5 b \"push nearby enemies away\"\n" );
	fputs(f,	"Take_Down a 5 c \"paralyze enemies\"\n" );
	fputs(f,	"Teleport k 10 b \"teleport to where you are aiming\"\n" );
	fputs(f,	"Thrash a 0 c \"damage and shake enemies\"\n" );
	fputs(f,	"Thunder d 15 b \"shock nearby enemies\"\n" );
	fputs(f,	"Thunder_Wave d 0 d \"paralyze enemies\"\n" );
	fputs(f,	"Thunderbolt d 15 b \"shock enemies you can see\"\n" );
//	fputs(f,	"Thunderpunch d \"\"\n" );
	fputs(f,	"Thundershock d 20 b \"shock an area with electricity\"\n" );
	fputs(f,	"Toxic h 0 e \"emit gas that poisons enemies\"\n" );
	fputs(f,	"Transform a 5 bt \"look like an object\"\n" );
	fputs(f,	"Tri_Attack a 0 a \"recieve extra ammo\"\n" );
//	fputs(f,	"Twin_Needle l \"\"\n" );
	fputs(f,	"Vice_Grip a 0 a \"paralyze enemies\"\n" );
	fputs(f,	"Vine_Whip e 5 bl \"throw enemies into the air\"\n" );
	fputs(f,	"Water_Gun c 0 bq \"shoot a beam of water\"\n" );
	fputs(f,	"Waterfall c 15 b \"create a waterfall that heals teammates\"\n" );
	fputs(f,	"Whirlwind a 10 b \"damage and throw enemies in the air\"\n" );
//	fputs(f,	"Wing_Attack j \"\"\n" );
	fputs(f,	"Withdraw c 0 d \"recieve extra armor\"\n" );
	fputs(f,	"Wrap j 0 d \"paralyze enemies\"\n" );

	fclose(f);
 }
 public ResetSafari()
 {
	new PokeFile[128];
	Poke_FileName( F_Safari, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile)

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - Do not edit here unless you know what your doing, otherwise use pm_safari\n" );
	fputs(f,	"// Place pokemon name then their level.\n" );
	fputs(f,	"// Level = 0 (No one can use) Level = -# (Only Admin can use)\n" );
	fputs(f,	"// Level = -1000 (Disabled entirely)\n" );

	fputs(f,	"Bulbasaur 1\n" );
	fputs(f,	"Charmander 1\n" );
	fputs(f,	"Squirtle 1\n" );
	fputs(f,	"Pikachu 1\n" );

	fputs(f,	"Articuno 1\n" );
	fputs(f,	"Zapdos 1\n" );
	fputs(f,	"Moltres 1\n" );
	fputs(f,	"Mew 1\n" );
	fputs(f,	"Mewtwo 1\n" );

	fclose(f);
 }

 public ResetMapsConfig()
 {
	new PokeFile[128];
	Poke_FileName( F_PokeMaps, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile)

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Note - Only add things here if you know what you are doing.\n" );
	fputs(f,	"// When adding map specific configs, do the following:\n" );
	fputs(f,	"//  1. list the map name in brakets\n" );
	fputs(f,	"//  2. list things you want under it\n" );
	fputs(f,	"//  3. do not put comments in the front of the line ( // or ; )\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"// Examples:\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"//[de_dust]\n" );
	fputs(f,	"//-disabled\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"//[as_*]\n" );
	fputs(f,	"//-skills_off\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"//[dod_*]\n" );
	fputs(f,	"//pm_pokemart potion 1\n" );

	fclose(f);
 }
 
 public ResetXPs()
 {
	new PokeFile[128];
	Poke_FileName( F_XPs, PokeFile);

	if(file_exists(PokeFile))
		delete_file(PokeFile)

	new f = fopen(PokeFile, "at");

	fputs(f,	"// Just place the level XPs how you want\n" );
	fputs(f,	"// Note - Even if you don't set it, level 1 will always be 0 xp.\n" );
	fputs(f,	NEWLINE );
	fputs(f,	"0	10	20	30	50	70	100	130	170	210	//1-10\n" );
	fputs(f,	"260	310	370	430	500	570	650	730	820	910	//11-20\n" );
	fputs(f,	"1010	1110	1220	1330	1450	1570	1700	1830	1970	2110	//21-30\n" );
	fputs(f,	"2260	2410	2570	2730	2900	3070	3250	3430	3620	3810	//31-40\n" );
	fputs(f,	"4010	4210	4420	4630	4850	5070	5300	5530	5770	6010	//41-50\n" );
	fputs(f,	"6260	6510	6770	7030	7300	7570	7850	8130	8420	8710	//51-60\n" );
	fputs(f,	"9010	9310	9620	9930	10250	10570	10900	11230	11570	11910	//61-70\n" );
	fputs(f,	"12260	12610	12970	13330	13700	14070	14450	14830	15220	15610	//71-80\n" );
	fputs(f,	"16010	16410	16820	17230	17650	18070	18500	18930	19370	19810	//81-90\n" );
	fputs(f,	"20260	20710	21170	21630	22100	22570	23050	23530	24020		//91-99\n" );

	fclose(f);
 }

 public ReadSkills()
 {
	new PokeFile[128];
	Poke_FileName( F_Skill, PokeFile);
	new Data[256]
	new Left[DESC_LENGTH], Right[151]
	new skill

	SkillsCreated = 0;
	debugMessage(_,2,"Config", "Begining to read skills file.");

	new f = fopen(PokeFile, "rt");
	while( !feof(f) )
	{
		if( skill == TOTAL_SKILLS )
			break;

		fgets(f, Data, 255);

		if( ValidData( Data )<2 )
			continue;

		//"Function type delay props"

		//"Function" "type delay props"
		strbreak(Data, Left, NAME_LENGTH-1, Right, 150);
		remove_quotes(Left);

		if( funcidx( Left ) == -1 ){
			poke_error("Invalid Skill '%s'", Left);
			continue;
		}
		copy( SkillName[skill], NAME_LENGTH-1, Left );

		//should be
		//"type" "delay props"
		//but people are stupid, so were going to do a while loop and look randomly
		strbreak(Right, Left, DESC_LENGTH-1, Right, 150);

		while( ValidData(Left) ){
			if( is_str_float(Left) ){
				if( !SkillDelay[skill] )		SkillDelay[skill] = str_to_float(Left);
			}
			else{
				if( !SkillType[skill] )			SkillType[skill] = POKE_TYPE:read_flags(Left);
				else if( !SkillInfo[skill] )	SkillInfo[skill] = read_flags(Left);
				else							copy(SkillDesc[skill], DESC_LENGTH-1, Left);
			}

			strbreak(Right, Left, DESC_LENGTH-1, Right, 150);
		}

		if( !SkillInfo[skill] )
			debugMessage(_,2,"Config", "Skill(%s) does not have any call flags", SkillName[skill]);

		SkillsCreated++;
		skill++;
	}
	fclose(f);

	debugMessage(_,2,"Config", "Skills loaded (%d total)", SkillsCreated);
 }
 public ReadPokemon( read_data )
 {
	new PokeFile[128];
	Poke_FileName( F_Pokemon, PokeFile);
	new Data[256]
	new Left[NAME_LENGTH], Right[151]
	new poke, evolve_num

	if( read_data ) debugMessage(_,2,"Config", "Begining to read pokemon file");

	poke = 1;
	new f = fopen(PokeFile, "rt");
	while( !feof(f) )
	{
		fgets(f, Data, 255);

		if( !ValidData( Data )  )
			continue;

		//"Number Name type attack props evolution"

		//"Number" "Name type attack props evolution"
		strbreak(Data, Left, NAME_LENGTH-1, Right, 150);
		poke = str_to_num(Left);

		if( poke < 1 || poke > TOTAL_POKES )
			continue;
		if( poke > PokesCreated )
			PokesCreated = poke;

		//"Name" "type attack props evolution"
		strbreak(Right, Left, NAME_LENGTH-1, Right, 150);
		remove_quotes(Left);
		copy( POKENAMES[poke], NAME_LENGTH-1, Left );

		if( !read_data )
			continue;

		//"type" "attack props evolution"
		strbreak(Right, Left, NAME_LENGTH-1, Right, 150);
		remove_quotes(Left);
		PokeType[poke] = POKE_TYPE:read_flags(Left);

		if( containi(Right, "\"") != -1 ){
			strbreak(Right, Left, NAME_LENGTH-1, Right, 150);
			remove_quotes(Left);
			PokeSkill[poke] = NameToSkill(Left);

			if( PokeSkill[poke] == -1 )
				poke_error("%s does not have a valid skill (%s)", PokeToName(poke), Left);
		}
		else{
			PokeSkill[poke] = -1;
			poke_error("%s does not have any skill specified.", PokeToName(poke));
		}

		strbreak(Right, Left, NAME_LENGTH-1, Right, 150);
		remove_quotes(Left);
		PokeInfo[poke] = read_flags(Left);

		if( PokeInfo[poke] & PI_EVOLVE )
		{
			evolve_num = 0;
			while( ValidData( Right ) ){
				strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
				remove_quotes(Left)
				PokeEvolve[evolve_num][poke] = str_to_num(Left)
				if( !PokeEvolve[evolve_num][poke] ){
					if(equali(Left, "TRADE"))
						PokeEvolve[evolve_num][poke] = TRADE_EVOLVE
					else
						PokeEvolve[evolve_num][poke] = -1*NameToItem(Left,false) // negative so we know it isn't a level
				}
				if( PokeEvolve[evolve_num][poke] ){
					strbreak(Right, Left, NAME_LENGTH-1, Right, 150)
					remove_quotes(Left)
					PokeEvolveTo[evolve_num][poke] = NameToPoke(Left)
					if( PokeEvolveTo[evolve_num][poke] )
						evolve_num++
					else
						PokeEvolve[evolve_num][poke] = 0
				}
			}
		}
	}
	fclose(f);

	copy( POKENAMES[0], NAME_LENGTH-1, "None" );
	PokeSkill[0] = -1;

	if( read_data ) debugMessage(_,2,"Config", "%d Pokemon loaded.", PokesCreated);
 }

 public ReadXPs()
 {
	new PokeFile[128];
	Poke_FileName( F_XPs, PokeFile);

	if( !file_exists(PokeFile) ){
		poke_error("**WARNING** PokeMod XP Table not found, making default")
		ResetXPs();
	}

	new Data[256];
	new Left[11];
	new last_level;

	new f = fopen(PokeFile, "rt");
	while( !feof(f) )
	{
		fgets(f, Data, 255);
		strbreak(Data, Left, 10, Data, 255);

		while( ValidData(Left) )
		{
			if( last_level >= MAX_LEVELS ){
				debugMessage( _, 1, "Config", "More than %d levels in xp table file", MAX_LEVELS);
				//we're going to end the while loop, but we'll need to end the other while loop also
				fseek(f, 0, SEEK_END);
				break;
			}

			PokeLevels[last_level] = str_to_num(Left);
			debugMessage( _, 8, "Config", "Level %d = %d xp", last_level, PokeLevels[last_level] );

			if(last_level && PokeLevels[last_level-1] >= PokeLevels[last_level]){

				poke_error( "ERROR in XP File: level %d(%d) >= level %d(%d)",last_level-1,PokeLevels[last_level-1],last_level,PokeLevels[last_level] )

				//we're going to end the while loop, but we'll need to end the other while loop also
				fseek(f, 0, SEEK_END);
				break;

			}
			else
				last_level++

			strbreak(Data, Left, 10, Data, 255);
		}
	}
	fclose(f);

	//set first level to 0 xp
	PokeLevels[0] = 0;
	TotalLevels = last_level;

	debugMessage( _, 1, "Config", "Loaded %d levels", last_level);
 }
 
 SearchCvars()
 {
	for(new i=0; i<MAX_CVARS; i++)
		if(get_ppcvar_line(i) == -1)
			set_ppcvar_line(i, cvar_default[i]);
 }
 