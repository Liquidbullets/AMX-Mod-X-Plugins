
 // Things to interact with Extendable Admin Menu
 #if EAM_MENU == 1

 #tryinclude <eam>

 #if defined _eam_included && EAM_VERSION >= 2

 enum{
	pm_givepoke, pm_setpoke, pm_takepoke, pm_morph,
	pm_givexp, pm_setxp,
	#if POKERANK_SYSTEM==1
	pm_addrank, pm_setrank,
	#endif
	pm_setlevel,
	pm_giveitem,
	max_pm_eam
 }
 new eam_com[max_pm_eam];

 public eam_setup()
 {
	new category = eam_category_register( "PokeMod" );
	eam_category_access( category, CVAR, get_pokecvar_name(pm_admin) );

	new commands[max_pm_eam][NAME_LENGTH] = {
		"givepoke", "setpoke", "takepoke", "morph",
		"givexp", "setxp",
		#if POKERANK_SYSTEM==1
		"addrank", "setrank",
		#endif
		"setlevel",
		"giveitem"
	};
	new i;
	for( i=0; i<max_pm_eam; i++ ){

		#if PM_ADMIN_COMMANDS==1
		format(commands[i], NAME_LENGTH-1, "pm_%s",commands[i]);
		#else
		format(commands[i], NAME_LENGTH-1, "amx_%s",commands[i]);
		#endif

		eam_com[i] = eam_command_register( commands[i] );
		eam_command_category( eam_com[i], category);

		eam_command_arg( eam_com[i], 1, PLAYER_OR_TEAM, USERNAME );

		if(	i == pm_givexp || i == pm_setxp || i == pm_setlevel )
			eam_command_arg( eam_com[i], 2, OPTIONS, "\"@ALL\" \"@CURRENT\"" );
		else if( i == pm_giveitem )
			eam_command_arg( eam_com[i], 2, INPUT, "Say an Item" );
		#if POKERANK_SYSTEM==1
		else if( i == pm_addrank )
			eam_command_arg( eam_com[i], 2, RANGE, -1000000, 1000000 );
		else if( i == pm_setrank )
			eam_command_arg( eam_com[i], 2, RANGE, 0, 1000000 );
		#endif
		else
			eam_command_arg( eam_com[i], 2, INPUT, "Say a Pokemon" );

		if(	i == pm_morph )
			eam_command_arg( eam_com[i], 3, INPUT, "Say a Pokemon" );
		else if( i == pm_giveitem )
			eam_command_arg( eam_com[i], 3, RANGE, -1000, 1000 );
		else if( i == pm_givexp )
			eam_command_arg( eam_com[i], 3, RANGE, -1000000, 1000000 );
		else if( i == pm_setxp )
			eam_command_arg( eam_com[i], 3, RANGE, 0, 1000000 );
		else if( i == pm_givepoke || i == pm_setlevel )
			eam_command_arg( eam_com[i], 3, RANGE, 1, TotalLevels );

	}
 }
 public eam_input_check( const id, const input[], const command, const arg )
 {
	if( command == eam_com[pm_giveitem] && arg == 2 ){
		if( !ValidItem( NameToItem( input ) ) ) return INPUT_CHECK_FAIL;
		return INPUT_CHECK_PASS;
	}
	else if( (command == eam_com[pm_morph] && arg == 3)
	||	(arg == 2 && command != eam_com[pm_givexp] && command != eam_com[pm_setxp] && command != eam_com[pm_setlevel] && command != eam_com[pm_giveitem] && command != eam_com[pm_addrank] && command != eam_com[pm_setrank])
	){
		new poke = NameToPoke( input );
		if( !poke ) return INPUT_CHECK_FAIL;

		#if RCON_POKEADMINS == 1
		if( id && !(get_user_flags(id)&ADMIN_RCON) && PokeInfo[poke] & PI_SPECIAL ){
			poke_print(id, print_chat, "You cannot use admin commands with special pokemon.");
			return INPUT_CHECK_FAIL;
		}
		#endif

		return INPUT_CHECK_PASS;
	}
	return INPUT_CHECK_NULL
 }
 #endif

 #endif