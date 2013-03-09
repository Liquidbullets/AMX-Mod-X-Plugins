 #include "pokemon/skills/bug.inl"
 #include "pokemon/skills/dragon.inl"
 #include "pokemon/skills/electric.inl"
 #include "pokemon/skills/fighting.inl"
 #include "pokemon/skills/fire.inl"
 #include "pokemon/skills/flying.inl"
 #include "pokemon/skills/ghost.inl"
 #include "pokemon/skills/grass.inl"
 #include "pokemon/skills/ground.inl"
 #include "pokemon/skills/ice.inl"
 #include "pokemon/skills/normal.inl"
 #include "pokemon/skills/poison.inl"
 #include "pokemon/skills/psychic.inl"
 #include "pokemon/skills/rock.inl"
 #include "pokemon/skills/water.inl"

 ///////////////////////////////////////
 //  When someone presses +pokeskill  //
 ///////////////////////////////////////
 public pokeskill_down(id)
 {
	g_StopSkill[id] = false;
	if( !PMon() ){
		poke_printlang(id,"POKEMOD_DISABLED")
		return PLUGIN_HANDLED
	}
	else if( PM_disabled == PM_XP ){
		poke_printlang(id,"POKEMOD_SKILLSDISABLED")
		return PLUGIN_HANDLED
	}
	else if( !ValidPoke( PlayersPoke[id][0] ) ){
		StartMenu(id)
		return PLUGIN_HANDLED
	}

	if( g_isItemed[id] ){
		SetActionMenu( id, AC_GO );
		return PLUGIN_HANDLED;
	}

	if( !Pokemod_Alive(id) ){
		new parm[2];
		parm[0] = id;
		parm[1] = 0;
		set_task(0.1, "PokeMenu", 0, parm, 2);
		return PLUGIN_HANDLED;
	}

	new skill_num, command[32];
	read_argv(0,command,31);
	replace(command, 31, "+pokeskill", EMPTY);
	skill_num = str_to_num(command);

	new poke_skills[MAX_SKILLS], skill_count, i;
	for( i = 0; i < MAX_SKILLS; i++ )
		poke_skills[i] = -1;

	new pid = Pokemon[id], tempskill, tempcheck, bool:skill_found;
	for( i = -1; i<MAX_ACTIVE_POKES; i++ ){
		if( i != -1 ){
			pid = PlayersPoke[id][i];

			//if weve already added the skill in, we don't need to add it again
			if( i == PlayersActive[id] )
				continue;
		}
		while( pid && skill_count < MAX_SKILLS ){
			if( !PokeDisabled(pid) && (tempskill = PokeSkill[pid]) != -1 ){
				if( (i==-1 || PokeInfo[pid] & PI_GLOBAL) && SkillInfo[tempskill] & SI_BIND ){
					skill_found = false;
					for( tempcheck=0; tempcheck < skill_count; tempcheck++ )
						if( poke_skills[tempcheck] == tempskill )
							skill_found = true;
					if( !skill_found ){
						poke_skills[skill_count] = tempskill;
						skill_count++;
					}
				}
			}

			pid = AnyEvolveTo(pid);
		}
	}

	if( skill_count > 1 && !skill_num ){
		if( !ValidSkill( ActiveSkill(id) ) ){
			SkillMenu(id);
			return PLUGIN_HANDLED;
		}
		else
			tempskill = ActiveSkill(id);
	}
	else if( skill_num )
		SetActiveSkill(id, (tempskill = poke_skills[skill_num-1]) );
	else
		SetActiveSkill(id, (tempskill = poke_skills[0]) );

	//couldnt find any skill to use, lets just use their first skill
	if( !ValidSkill(tempskill) && !PokeDisabled( Pokemon[id] ) )
		SetActiveSkill(id, (tempskill = PokeSkill[Pokemon[id]]) );

	//stop, they dont have a valid skill
	if( !ValidSkill(tempskill) ){
		poke_printlang(id,"POKEMOD_NOSKILL" );
		return PLUGIN_HANDLED;
	}
	//stop, they dont have a bind skill
	if( !(SkillInfo[tempskill] & SI_BIND) ){
		poke_printlang(id,"POKEMOD_SKILLAUTO", SkillToName(tempskill) );
		return PLUGIN_HANDLED;
	}
	//stop, we arent allowing the skill to be used
	if( DisAllowSkill( id, tempskill, true ) )
		return PLUGIN_HANDLED;

	//check spam time. stop if they are spamming
	new Float:current_time = get_gametime();
	if(current_time-g_lastSkill[id] < SKILL_SPAM){
		poke_printlang(id,"POKEMOD_SKILLSPAM");
		return PLUGIN_HANDLED;
	}
	g_lastSkill[id] = current_time;

	//send into skill handler
	bind_skill_handler(id+POKESKILL_TASK);

	debugMessage(id,6,"Player", "keyed down with %s", SkillToName( ActiveSkill(id) ) );

	return PLUGIN_HANDLED;
 }

 ////////////////////////////////////////
 //  When someone releases +pokeskill  //
 ////////////////////////////////////////
 public pokeskill_up(id)
 {
	debugMessage(id,6,"Player", "keyed up with %s", SkillToName( ActiveSkill(id) ) );

	skills_handler(id, SI_RELEASE);

	if( ValidSkill( ActiveSkill(id) ) ){
		if( Action[id] == AC_SKILL )
			SkillMenu(id);
	}

	g_StopSkill[id] = true;

	#if SKILL_UNSPAM == 1
	g_lastSkill[id] = 0.0;
	#endif

	return PLUGIN_HANDLED;
 }
 
 public bind_skill_handler(id)
 {
	id -= POKESKILL_TASK;

	if( ValidPlayer( id ) ){
		if( !Pokemod_Alive(id) || DisAllowSkill( id, ActiveSkill(id), true ) || g_StopSkill[id] ){
			g_StopSkill[id] = false;
		}
		else if( ValidSkill( ActiveSkill(id) ) ){
			CallSkill(id, ActiveSkill(id), SI_BIND);
			set_task(0.1,"bind_skill_handler", POKESKILL_TASK+id)
		}
	}
 }
 skills_handler(id, call_prop)
 {
	if( PlayerEventForward > -1 )
	{
		new functionReturn
		ExecuteForward(PlayerEventForward, functionReturn, id, call_prop)
		if( functionReturn==PLUGIN_HANDLED )
			return;
	}

	new skill, subpoke, bool:auto_globals=false;

	if( call_prop & SI_RESET || call_prop & SI_ROUNDRESET || call_prop & SI_CONNECT || call_prop & SI_DISCONNECT )
		auto_globals = true;

	if( !PokeDisabled( Pokemon[id] ) ){
		skill = PokeSkill[Pokemon[id]];
		if( !DisAllowSkill(id, skill) && SkillInfo[ skill ] & call_prop )
			CallSkill(id, skill, call_prop)
	}

	//if their active poke is evolved, deal with the skills it had
	subpoke = AnyEvolveTo(Pokemon[id])
	while(subpoke && subpoke!=Pokemon[id]){
		if( !PokeDisabled( subpoke ) ){
			skill = PokeSkill[subpoke];
			if( !DisAllowSkill(id, skill) && SkillInfo[skill] & call_prop )
				CallSkill(id, skill, call_prop)
		}
		subpoke = AnyEvolveTo(subpoke)
	}

	//now on to the global pokes
	new m, pid_global;
	for(m=0; m<MAX_ACTIVE_POKES; m++){

		pid_global = PlayersPoke[id][m]
		if( Pokemon[id] == pid_global )
			continue;

		if( (auto_globals || PokeInfo[pid_global] & PI_GLOBAL) && !PokeDisabled(pid_global) ){
			skill = PokeSkill[pid_global];
			if( !DisAllowSkill(id, skill) && SkillInfo[skill] & call_prop )
				CallSkill(id, skill, call_prop)
		}

		//sub globals now
		subpoke = AnyEvolveTo(pid_global)
		while(subpoke && subpoke!=pid_global){
			if( (auto_globals || PokeInfo[subpoke] & PI_GLOBAL) && !PokeDisabled(subpoke) ){
				skill = PokeSkill[subpoke];
				if( !DisAllowSkill(id, skill) && SkillInfo[skill] & call_prop )
					CallSkill(id, skill, call_prop)
			}
			subpoke = AnyEvolveTo(subpoke)
		}
	}
 }
 bool:PokeDisabled( pokemon )
 {
	if( PokeInfo[pokemon]&PI_DISABLE || SafariLevel(pokemon)==-1000 )
		return true;
	return false;
 }
 bool:DisAllowSkill(id, skill, bool:bind=false)
 {
	if( !ValidSkill( skill ) || !Pokemod_Connected(id) )
		return true;
	if( PM_disabled == PM_XP ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLSDISABLED");
		return true;
	}

	else if( SkillInfo[skill] & SI_DISABLE ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLDIS");
		return true;
	}
	else if( SkillInfo[skill] & SI_CALL )
		return false;
	else if( !(SkillInfo[skill] & SI_IGNORE) && BindTime(id) ){
		if(bind)			sound_deny(id);
		return true;
	}
	else if( g_Status[id][STATUS_ASLEEP] ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLASLEEP");
		return true;
	}
	else if( !(SkillInfo[skill] & SI_DEAD) && !Pokemod_Alive(id) ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLDEAD");
		return true;
	}
	else if( SkillInfo[skill] & SI_ADMIN && !is_user_admin(id) ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLADMIN");
		return true;
	}
	else if( FreezeTime ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLFREEZE");
		return true;
	}
	#if MOD==CS
	else if( cs_get_user_vip(id) ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLVIP");
		return true;
	}
	#endif
	#if MOD==NS
	else if( ns_get_class(id) == CLASS_COMMANDER ){
		if(bind)			poke_printlang(id,"POKEMOD_SKILLCOM");
		return true;
	}
	#endif
	return false;
 }

 stock RemoveDelayCallSkill(id, skill)
	remove_task( SKILLCALL_TASK + (skill * MAX_PLAYERS) + id );
 stock DelayedCallSkill(Float:time, id, skill, prop)
 {
	new parm[3];
	parm[0] = id;
	parm[1] = skill;
	parm[2] = prop;
	set_task(time, "task_CallSkill", SKILLCALL_TASK + (skill * MAX_PLAYERS) + id, parm, 3);
 }
 public task_CallSkill( parm[] )
	CallSkill( parm[0], parm[1], parm[2] );

 CallSkill(id, skill, prop)
 {
	if( ValidPlayer(id) && ValidSkill(skill) ){
		debugMessage(id,7,"Player", "calling skill %s", SkillToName( skill ) );

		static temp_forward, temp_return;

		//skill is from another plugin
		if( SkillInfo[skill] & SI_PUBLIC )
			temp_forward = CreateMultiForward( SkillName[skill], ET_STOP, FP_CELL, FP_CELL, FP_CELL);
		//skill is in pokemod
		else
			temp_forward = CreateOneForward( plugin_id, SkillName[skill], FP_CELL, FP_CELL, FP_CELL );

		ExecuteForward( temp_forward, temp_return, id, skill, prop );
		DestroyForward( temp_forward );

		if( temp_return == SKILL_USED ){
			if( prop & SI_BIND )
				bind_used( id, skill );
			else{
				#if ONE_SKILL_USE == 0
					parm[0] = id;
					parm[1] = skill;
					set_task(0.0, "delayed_bind_used", 0, parm, 2);
				#else
					bind_used( id, skill );
				#endif
			}
			return true;
		}
	}
	return false;
 }

 /////////////////////////
 //  Resets all skills  //
 /////////////////////////
 ResetSkills(id, bool:Round_Skills)
 {
	debugMessage(id,4,"Player", "reset skills");

	//things to be reset only after death/round end
	if(Round_Skills){
		skills_handler(id, SI_ROUNDRESET);
	}

	skills_handler(id, SI_RESET);
	CheckRepUsed(id);

	SetActiveSkill(id);

	PlayerStat[id][STAT_ATTACK] = PlayerStat[id][STAT_DEFENSE] = PlayerStat[id][STAT_SPECIAL] = PlayersLevel(id);
	PlayerStat[id][STAT_SPEED] = 0;

	fm_set_rendering(id, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, 255);

	if(pev_valid(PlayerEnt[id])){
		new szClass[32];
		pev(PlayerEnt[id], pev_classname, szClass, 31);
		if(equali(szClass, "psy_beam"))
			psybeam_touch(PlayerEnt[id], 0);
	}
	PlayerEnt[id] = 0;
	g_Charge[id] = 0;

	screenflash(id);
	SetSpeed(id);
 }

 ActiveSkill(id)
	return g_isSkilled[id];
 SetActiveSkill(id, skill=-1)
 {
	g_isSkilled[id] = skill;
	debugMessage(id,6,"Player", "set active skill to %s(%d)",SkillToName(skill),skill);
 }
 
 /////////////////////////////
 //  Organized bind delays  //
 /////////////////////////////
 public delayed_bind_used( parm[] )
	bind_used( parm[0], parm[1] );
 bind_used(id, skill=-1, Float:over_ride=0.0)
 {
	new Float:gametime = get_gametime();
	new Float:newtime;
	if( !BindUsed[id] || BindUsed[id]==WAIT_TIME ) newtime = gametime;
	else newtime = BindUsed[id];

	if( over_ride != 0.0 )
		newtime += over_ride;
	else if( skill != -1 ){
		new Float:tasktime, Float:oldtime;
		tasktime = oldtime = SkillDelay[skill];

		tasktime -= (PlayersLevel(id)*DELAY_LEVEL_REDUCE);

		if(tasktime < oldtime/3)
			tasktime = oldtime/3;

		newtime += tasktime;
	}

	if( newtime > gametime ){
		BindUsed[id] = newtime;
		g_StopSkill[id] = true;
	}
 }
 
 /////////////////////////////////////////////////
 //  If we want to set it to exactly something  //
 /////////////////////////////////////////////////
 set_bind(id, Float:time=0.0)
	BindUsed[id] = time;
 
 ///////////////////////////////////////////////////////////////////
 //  If I need to stop them after they've used it a couple times  //
 ///////////////////////////////////////////////////////////////////
 end_rep_use(id, skill, Float:max)
 {
	SetActiveSkill(id, skill);
	if( ++g_Charge[id] >= max*10 ){
		g_Charge[id] = 0;
		if( Action[id] != AC_SKILL )
			SetActiveSkill(id);
		return true;
	}
	return false;
 }
 CheckRepUsed(id)
 {
	if( g_Charge[id] && ValidSkill( ActiveSkill(id) ) ){
		bind_used(id, ActiveSkill(id));
		g_Charge[id] = 0;
		SetActiveSkill(id);
		return true;
	}
	return false;
 }

 public create_sprite_spray(parm[])
 {
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(120)
	write_coord(parm[0])		//origin
	write_coord(parm[1])
	write_coord(parm[2])
	write_coord(parm[3])		//velocity
	write_coord(parm[4])
	write_coord(parm[5] + 5)
	write_short(parm[6])		//sprite
	write_byte(8)				//count
	write_byte(10)				//speed
	write_byte(100)				//noise
	write_byte(5)				//rendermode
	message_end()
 }
 public spray_check(parm[])
 {
	new id = parm[0]
	new flame = parm[1]	//this is 0 for smoke, 1 for fire
	new skill = parm[2]	//this is 0 for smoke, 1 for fire

	//direct damage first
	new ent, body
	get_user_aiming(id, ent, body, 600)
	
	if(Pokemod_Alive(ent))
	{
		if(flame==1)
			ApplyStatus( id, ent, STATUS_BURN, 0.0 )
		else if(flame==2)
			screenflash(ent,139,59,19,200)
		else
			screenflash(ent,255,255,255,255)
	}

	//now indirect damage
	new Players[32], iNum, target
	poke_get_players(Players, iNum, PLAYER_ALIVE|PLAYER_MORTAL)

	new Float:fOrigin[3], Float:fOrigin2[3]
	new temp[3], Float:fAim[3]
	new Float:fDistance = 600.0
	new iDistance

	for(new i = 0; i < iNum; i++)
	{
		target = Players[i]
		if( target == id )
			continue

		pev(id, pev_origin, fOrigin)
		pev(target, pev_origin, fOrigin2)

		get_user_origin(id, temp, 3)
		IVecFVec(temp, fAim)

		if(get_distance_f(fOrigin, fOrigin2) > fDistance)
			continue 

		iDistance = get_distance_to_line(fOrigin, fOrigin2, fAim)
		if( iDistance > 40 || iDistance < 0 || !fm_is_ent_visible(id, target) )
			continue 

		if(flame==1){
			skilldamage(target, id, 1, skill)
			if(Chance(id))
				ApplyStatus( id, target, STATUS_BURN, 0.0 )
		}
		else if(flame==2)
			screenflash(target,139,59,19,200)
		else
			screenflash(target,255,255,255,255)
	}
 }
 BlowUp(id)
 {
	get_user_origin(id,origin[id])

	poke_sound(id,CHAN_STATIC, SND_SUCK)

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte(TE_IMPLOSION)
	write_coord(origin[id][0])
	write_coord(origin[id][1])
	write_coord(origin[id][2])
	write_byte(100)
	write_byte(20)
	write_byte(5)
	message_end()

	// random explosions
	message_begin( MSG_PVS, SVC_TEMPENTITY, origin[id] )
	write_byte(TE_EXPLOSION) // This just makes a dynamic light now
	write_coord( origin[id][0] + random_num( -100, 100 ))
	write_coord( origin[id][1] + random_num( -100, 100 ))
	write_coord( origin[id][2] + random_num( -50, 50 ))
	write_short( SPRITE_INDEX[SPR_EEXPLO] )
	write_byte( random_num(0,20) + 20 )	// scale * 10
	write_byte( 12 )					// framerate
	write_byte(0)
	message_end()

	// lots of smoke
	message_begin( MSG_PVS, SVC_TEMPENTITY, origin[id] )
	write_byte(TE_SMOKE)
	write_coord( origin[id][0] + random_num( -100, 100 ))
	write_coord( origin[id][1] + random_num( -100, 100 ))
	write_coord( origin[id][2] + random_num( -50, 50 ))
	write_short( SPRITE_INDEX[SPR_STEAM1] )
	write_byte( 60 ) // scale * 10
	write_byte( 10 ) // framerate
	message_end()

	// blast circles
	message_begin( MSG_PAS, SVC_TEMPENTITY, origin[id] )
	write_byte(TE_BEAMCYLINDER)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16)
	write_coord( origin[id][0])
	write_coord( origin[id][1])
	write_coord( origin[id][2] - 16 + EXPLOSION_RADIUS)
	write_short( SPRITE_INDEX[SPR_SHOCKWAVE] )
	write_byte( 0 )		// startframe
	write_byte( 0 )		// framerate
	write_byte( 6 )		// life
	write_byte( 16 )	// width
	write_byte( 0 )		// noise
	write_byte( 188 )
	write_byte( 220 )
	write_byte( 255 )
	write_byte( 255 )	//brightness
	write_byte( 0 )		// speed
	message_end()

 }
 public make_powder(parm[])
 {
	new id = parm[0];
	if(!PMon() || !Pokemod_Alive(id))
		return

	new Float:fl_Origin[3], ptime = POWDER_TIME

	pev(id, pev_origin, fl_Origin)

	new newEnt = fm_create_entity("info_target")

	set_pev(newEnt, pev_classname, "pokemod_ent")
	fm_entity_set_model(newEnt, MODELS[MDL_POWDER])

	// Change height of entity origin to hands
	fl_Origin[2] += 6

	// Set entity postion
	fm_entity_set_origin(newEnt, fl_Origin)

	// Set properties of the entity
	set_pev( newEnt, pev_rendermode, 5)
	set_pev( newEnt, pev_renderamt, 255.0)
	set_pev( newEnt, pev_owner, id)

	set_pev( newEnt, pev_animtime, 1.0);
	set_pev( newEnt, pev_framerate, 1.0);
	set_pev( newEnt, pev_sequence, 0);

	new parm2[3];
	parm2[0] = newEnt;
	parm2[1] = parm[1];
	parm2[2] = parm[2];
	check_powder(parm2)
	set_task(0.5,"check_powder",0, parm2, 3 , "a", 2*ptime-1)
	set_task(ptime*1.0,"poke_remove_entity",newEnt+REMOVE_TASK)
 }
 public check_powder(parm[])
 {
	new spowder = parm[0];
	new check_poison = parm[1];
	new check_sleep = parm[2];

	if(!pev_valid(spowder))
		return

	new id = pev(spowder, pev_owner)

	new Float:poison_time = get_ppcvar_float(pm_poisontime)
	new Float:sleep_time = get_ppcvar_float(pm_sleeptime)
	new range = POWDER_RANGE
	AddLvl(id, range)

	new aid, players[MAX_PLAYERS], pnum
	poke_get_players(players,pnum,PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_NOTEAM,Pokemod_Team(id))

	for(new i = 0; i<pnum; i++)
	{
		aid = players[i]

		if( fm_entity_range(spowder,aid) < range ) {

			if( check_poison && !g_Status[aid][STATUS_POISON] ){
				ApplyStatus( id, aid, STATUS_POISON, poison_time )
			}

			if( check_sleep && !g_Status[aid][STATUS_ASLEEP] ){
				ApplyStatus( id, aid, STATUS_ASLEEP, sleep_time )
			}
		}
	}
 }
 public check_spore(parm[])
 {
	new ent = parm[0];
	parm[2]--;

	if( pev_valid(ent) ){
		new Float:health;
		pev(ent, pev_health, health);
		if( health-POKE_HP_OFFSET < 0 ){
			new Float:origfl[3], orig[3];
			pev( ent, pev_origin, origfl );
			FVecIVec(origfl, orig);

			new parmlol[7];
			parmlol[0] = orig[0];
			parmlol[1] = orig[1];
			parmlol[2] = orig[2]+100;
			parmlol[3] = 0;
			parmlol[4] = 0;
			parmlol[5] = 50;
			parmlol[6] = SPRITE_INDEX[SPR_STEAM1];
			create_sprite_spray( parmlol );

			set_task(1.0,"poke_remove_entity",ent+REMOVE_TASK);
			set_task(0.1, "check_spore", 0, parm, 4);

			return;
		}

		if( !parm[2] ){
			fm_remove_entity(ent);
			return;
		}
	}
	else
		return;

	new id = pev(ent, pev_owner);

	new range = SPORE_RANGE;
	AddLvl(id, range);

	new aid, players[MAX_PLAYERS], pnum
	poke_get_players(players,pnum,PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_NOTEAM,Pokemod_Team(id))
	for(new i = 0; i<pnum; i++)
	{
		aid = players[i]

		if( fm_entity_range(ent,aid) < range ) {

			if( parm[3] ){
				if( !g_Status[aid][STATUS_POISON] )
					ApplyStatus( id, aid, STATUS_POISON, 5.0 )
			}
			else
				set_pev( aid, pev_velocity, Float:{0.0,0.0,0.0} );
		}
	}

	new Float:origfl[3], orig[3];
	pev( ent, pev_origin, origfl );
	FVecIVec(origfl, orig);

	if( (parm[2] % 10) == 0 ){
		message_begin( MSG_PAS, SVC_TEMPENTITY, orig )
		write_byte(TE_BEAMCYLINDER)
		write_coord( orig[0])
		write_coord( orig[1])
		write_coord( orig[2] + 20 )
		write_coord( orig[0])
		write_coord( orig[1])
		write_coord( orig[2] + 20 + range)
		write_short( SPRITE_INDEX[SPR_GAS] )
		write_byte( 0 )		// startframe
		write_byte( 0 )		// framerate
		write_byte( 10 )		// life
		write_byte( 16 )	// width
		write_byte( 0 )		// noise
		write_byte( 88 )
		write_byte( 20 )
		write_byte( parm[3] ? 255 : 50 )
		write_byte( 255 )	//brightness
		write_byte( 0 )		// speed
		message_end()
	}

	if( (parm[2] % 5) == 0 ){
		new parmlol[7];
		parmlol[0] = orig[0];
		parmlol[1] = orig[1];
		parmlol[2] = orig[2]+100;
		parmlol[3] = 0;
		parmlol[4] = 0;
		parmlol[5] = -20;
		parmlol[6] = parm[3] ? SPRITE_INDEX[SPR_BIGSPIT] : SPRITE_INDEX[SPR_MUZZLEFLASH1];
		create_sprite_spray( parmlol );
	}

	set_task(0.1, "check_spore", 0, parm, 4);
 }
 public delayed_grenade(parm[])
	give_grenade(parm[0],parm[1],bool:parm[2]);
 give_grenade(id, how_many=1, bool:add_with=false)
 {
	if( !Pokemod_Alive(id) )
		return;
	new old_amount, new_amount
	#if MOD==CS
		give_item(id, "weapon_hegrenade")
		old_amount = cs_get_user_bpammo(id, CSW_HEGRENADE)
		new_amount = add_with ? (old_amount+how_many) : ( max(how_many,old_amount) )
		cs_set_user_bpammo(id, CSW_HEGRENADE, new_amount)
	#endif
	#if MOD==DOD
		new wpnid
		if(Pokemod_Team(id)==2){
			give_item(id, "weapon_stickgrenade")
			wpnid = DODW_STICKGRENADE
		}
		else{
			give_item(id, "weapon_handgrenade")
			wpnid = DODW_HANDGRENADE
		}
		old_amount = dod_get_user_ammo(id,wpnid)
		new_amount = add_with ? (old_amount+how_many) : ( max(how_many,old_amount) )
		dod_set_user_ammo(id, wpnid, new_amount)
	#endif
	#if MOD==NS
		if(Pokemod_Team(id)==1){
			new wpn[32], bpammo
			get_user_weapon(id, old_amount, bpammo)
			get_weaponname(WEAPON_GRENADE,wpn,31)

			ns_give_item(id, "weapon_grenade")
			new_amount = add_with ? (old_amount+how_many) : ( max(how_many,old_amount) )
			new iWPNidx = -1
			while ((iWPNidx = fm_find_ent_by_class(iWPNidx, wpn)) != 0) {
				if (id == pev(iWPNidx, pev_owner)) {
					ns_set_weap_clip(iWPNidx, new_amount)
				}
			}
		}
	#endif
 }
 Gas(id, bool:smog=false, bool:poison=false)
 {
	if( !Pokemod_Alive(id) || ( !smog && !poison ) )
		return;

	get_user_origin(id, origin[id])
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
	write_byte(TE_SPRITE)
	write_coord(origin[id][0]) // start position
	write_coord(origin[id][1])
	write_coord(origin[id][2])
	write_short(SPRITE_INDEX[SPR_GAS]) // sprite index
	write_byte(10) // scale in 0.1's
	write_byte(100) // brightness
	message_end()

	new aid, playersa[MAX_PLAYERS], pnuma
	poke_get_players(playersa,pnuma,PLAYER_ALIVE|PLAYER_MORTAL|PLAYER_NOTEAM,Pokemod_Team(id))

	new Float:poison_time = get_ppcvar_float(pm_poisontime)
	new distance
	new maxdist = 300
	AddLvl(id, maxdist)

	for(new ick = 0; ick<pnuma; ick++)
	{
		aid = playersa[ick]

		get_user_origin(aid, origin[aid])

		distance = get_distance(origin[aid], origin[id])
		if(distance <= maxdist){
			if(smog){
				poke_print(aid,print_center,LANG,aid,"POKEMOD_SMOG")
				screenflash(aid, 139, 59, 19, 222, 1.0)
			}
			if(poison && !g_Status[aid][STATUS_POISON]){
				ApplyStatus( id, aid, STATUS_POISON, poison_time )
			}
		}
	}
 }
 make_snow(id)
 {
	get_user_origin(id,origin[id]);

	//lets put one on each side of them
	new sides[4][2];
	sides[0][0] = sides[0][1] = sides[2][0] = sides[3][1] = 100;
	sides[1][0] = sides[1][1] = sides[2][1] = sides[3][0] = -100;

	for(new i=0; i<4; i++){
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
		write_byte( TE_SPRITE ) // additive sprite, plays 1 cycle
		write_coord(origin[id][0] + sides[i][0] + random_num(-10,10))
		write_coord(origin[id][1] + sides[i][1] + random_num(-10,10))
		write_coord(origin[id][2])
		write_short( SPRITE_INDEX[SPR_SNOW] )
		write_byte( 14 ) // byte (scale in 0.1's)
		write_byte( 255 ) //  byte (brightness)
		message_end()
	}

	return PLUGIN_HANDLED
 }
 PokeSkillTarget(id, &damage=-1, &weapon=-1)
 {
	new victim;
	// If they were damaged
	if( PlayerDamageInfo[id][PD_DAMAGE] ){
		damage = PlayerDamageInfo[id][PD_DAMAGE];
		weapon = PlayerDamageInfo[id][PD_WEAPON];
		return PlayerDamageInfo[id][PD_ATTACKER];
	}
	// If they damaged someone else
	for( victim=0; victim<MAX_PLAYERS; victim++ )
		if( PlayerDamageInfo[victim][PD_ATTACKER] == id ){
			damage = PlayerDamageInfo[victim][PD_DAMAGE];
			weapon = PlayerDamageInfo[victim][PD_WEAPON];
			return victim;
		}
	// If they touched someone
	if( PlayerDamageInfo[id][PD_ATTACKER] ){
		damage = weapon = 0;
		return PlayerDamageInfo[id][PD_ATTACKER];
	}
	// Get who they are looking at
	get_user_aiming(id, victim, weapon);
	if( Pokemod_Alive(victim) ){
		weapon = get_user_weapon(id);
		damage = 0;
		return victim;
	}
	return 0;
 }
 
 SkillLevel(id, skill)
 {
	if( ValidPlayer(id) && ValidSkill(skill) ){
		static pid, i;
		pid = Pokemon[id];
		for( i = -1; i<MAX_ACTIVE_POKES; i++ ){
			if( i != -1 ){
				//if weve already added the skill in, we don't need to add it again
				if( i == PlayersActive[id] )
					continue;
				else
					pid = PlayersPoke[id][i];
			}
			while( ValidPoke(pid) ){
				if( !PokeDisabled(pid) && PokeSkill[pid] == skill ){
					if( i==-1 || PokeInfo[pid] & PI_GLOBAL ){
						return PlayersLevel(id, i);
					}
				}

				pid = AnyEvolveTo(pid);
			}
		}
	}
	return 0;
 } 
