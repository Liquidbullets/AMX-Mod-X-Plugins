
 #if defined _pokemod_stocks_included
	#endinput
 #endif
 #define _pokemod_stocks_included


 //--if someone is outside--//
 stock bool:is_user_outside(id)
 {
	new Float:origin[3];
	pev( id, pev_origin, origin );
	while( engfunc(EngFunc_PointContents, origin) == -1 )
		origin[2] += 5.0;
	return (engfunc(EngFunc_PointContents, origin) == -6 );
 }
 stock bool:entity_in_water(id)
	return (pev(id, pev_waterlevel) == 3);
 stock bool:entity_on_ground(id)
	return (pev(id, pev_flags)&FL_ONGROUND)?true:false;
 stock bool:entity_in_air(id)
	return !entity_on_ground(id);

 /* Sets indexes of players.
 * Flags:
 *  PLAYER_ALIVE  - players are alive
 *  PLAYER_DEAD   - players are dead
 *  PLAYER_REAL   - players are not bots
 *  PLAYER_BOT    - player are bots
 *  PLAYER_INTEAM - players are in team specified
 *  PLAYER_GOD    - players have godmode
 *  PLAYER_MORTAL - players don't have godmode
 *  PLAYER_NOHLTV - players are not HLTV
 *  PLAYER_NOTEAM - players are not in team specified
 * Example: Get all alive on team 2: poke_get_players(players,num,PLAYER_ALIVE|PLAYER_INTEAM,2) */
 stock poke_get_players(players[MAX_PLAYERS], &pnum, const PLAYER_SELECT:flags=PLAYER_NONE, team=-1)
 {
	static total, i;
	total = 0;
	for(i=1; i<=MAX_PLAYERS; i++)
	{
		if( !Pokemod_Connected(i) )
			continue;

		if( flags != PLAYER_NONE ){

			if( Pokemod_Alive(i) ? (flags & PLAYER_DEAD) : (flags & PLAYER_ALIVE) )
				continue;

			if( (flags & PLAYER_REAL) && is_user_bot(i) )
				continue;
			else if( (flags & PLAYER_BOT) && !is_user_bot(i) )
				continue

			if( (flags & PLAYER_MORTAL) && get_user_godmode(i) )
				continue;
			else if( (flags & PLAYER_GOD) && !get_user_godmode(i) )
				continue

			if( (flags & PLAYER_NOHLTV) && is_user_hltv(i) )
				continue;

			if( team != -1 ){
				if( Pokemod_Team(i)==team ? (flags & PLAYER_NOTEAM) : (flags & PLAYER_INTEAM) )
					continue;
			}

		}
		players[total] = i;
		total++;
	}
	pnum = total;

	return true;
 }

 stock get_distance_to_line(Float:pos_start[3], Float:pos_end[3], Float:pos_object[3])
 {
	new Float:vec_start_end[3], Float:vec_start_object[3], Float:vec_end_object[3], Float:vec_end_start[3]
	xs_vec_sub(pos_end, pos_start, vec_start_end) 		// vector from start to end 
	xs_vec_sub(pos_object, pos_start, vec_start_object) 	// vector from end to object 
	xs_vec_sub(pos_start, pos_end, vec_end_start) 		// vector from end to start 
	xs_vec_sub(pos_end, pos_object, vec_end_object) 		// vector object to end 

	new Float:len_start_object = vector_length(vec_start_object) 
	new Float:angle_start = floatacos(xs_vec_dot(vec_start_end, vec_start_object) / (vector_length(vec_start_end) * len_start_object), degrees)  
	new Float:angle_end = floatacos(xs_vec_dot(vec_end_start, vec_end_object) / (vector_length(vec_end_start) * vector_length(vec_end_object)), degrees)  

	if(angle_start <= 90.0 && angle_end <= 90.0) 
		return floatround(len_start_object * floatsin(angle_start, degrees)) 
	return -1
 }
 
 //--Finds free place to put escape rope users and npcs--//
 stock find_free_spawn(iTeamNumber, Float:spawnOrigin[3], Float:spawnAngle[3])
 {
	#if MOD==NS
		spawnAngle[0] = 1.0*ns_get_spawn(iTeamNumber, 0, spawnOrigin);
		ns_get_spawn(iTeamNumber, random_num(1, floatround(spawnAngle[0]) ), spawnOrigin);
		return 1;
	#else

	new iSpawn;
	if(iTeamNumber == 2)
		iSpawn=0;
	else
		iSpawn=1;

	const maxSpawns = 128;
	new spawnPoints[maxSpawns], bool:spawnChecked[maxSpawns], spawnpoint, spawnnum;
	new ent = -1, spawnsFound = 0;

	while( ( ent = fm_find_ent_by_class(ent,spawnEntString[iSpawn]) ) && spawnsFound < maxSpawns )
		spawnPoints[spawnsFound++] = ent;

	new Float:vicinity = 100.0;		//(32x32x96)
	new i, entList[1];

	for( i = 0; i<maxSpawns; i++)
		spawnChecked[i] = false;

	// Loop through all the spawn points
	i = 0;
	while( i++ < spawnsFound*10 ){
		spawnnum = random(spawnsFound);
		spawnpoint = spawnPoints[ spawnnum ];

		if( spawnpoint && !spawnChecked[spawnnum] ){
			//don't check this spawn point again
			spawnChecked[spawnnum] = true;

			// Get the origin of the spawn point
			pev(spawnpoint, pev_origin, spawnOrigin)

			// Determine if something is in this near the spawn point
			if(	!fm_find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)
			&&	!fm_find_sphere_class(0, "pokemon", vicinity, entList, 1, spawnOrigin)
			&&	!fm_find_sphere_class(0, "npc_pokecenter", vicinity, entList, 1, spawnOrigin)
			){
				// If there is nothing, return the angles and spawnpoint
				pev(spawnpoint, pev_angles, spawnAngle);
				return spawnpoint;
			}
		}
	}

	return 0;
	#endif
 }
 //--Flashes the screen a color--//
 stock screenflash(id, r=0, g=0, b=0, alpha=0, Float:time=0.5, fade=12, bool:spec_msg=false)
 {
	if( Pokemod_Alive(id) ){
		if( !is_user_bot(id) && !spec_msg ){
			//SECONDS_TO_SCREENFADE_UNITS(%1) for duration and hold time
			if( spec_msg )
				message_begin(MSG_SPEC, gmsgScreenFade, _, id) //message begin
			else
				message_begin(MSG_ONE_UNRELIABLE, gmsgScreenFade, _, id)
			write_short( SECONDS_TO_SCREENFADE_UNITS(time) ) //duration
			write_short( SECONDS_TO_SCREENFADE_UNITS(time) ) //holdtime
			write_short( (1<<fade) ) //fade in/out
			write_byte( r )
			write_byte( g )
			write_byte( b )
			write_byte( alpha )
			message_end()
		}

		#if SPEC_MSGS == 1
		if(!spec_msg)	screenflash(id,r,g,b,alpha,time,fade,true);
		#endif
	}
 }
 //--Flashes the screen a color--//
 stock screenshake(id, amplitude, Float:duration, frequency, bool:spec_msg=false)
 {
	if( Pokemod_Alive(id) ){
		if( !is_user_bot(id) && !spec_msg ){
			//SECONDS_TO_SCREENFADE_UNITS(%1) for duration
			if( spec_msg )
				message_begin(MSG_SPEC, gmsgScreenShake, _, id) //message begin
			else
				message_begin(MSG_ONE_UNRELIABLE, gmsgScreenShake, _, id)
			write_short( clamp(floatround(amplitude * float(1<<12)), 0, 0xFFFF) )// shake amount
			write_short( clamp(floatround(duration * float(1<<12)), 0, 0xFFFF) )// shake lasts this long
			write_short( clamp(floatround(frequency * float(1<<8)), 0, 0xFFFF) )// shake noise frequency
			message_end()
		}

		#if SPEC_MSGS == 1
		if(!spec_msg)	screenshake(id,amplitude,duration,frequency,true);
		#endif
	}
 }
 
 //makes a beam between two ents
 stock MakeLine_Ent(id,start,end,sprite,frame,rate,life,width,noise,r,g,b,a,scroll,bool:specmsg=false)
 {
	if( !is_user_bot(id) && !specmsg ){
		if( specmsg )
			message_begin(MSG_SPEC, SVC_TEMPENTITY, _, id) //message begin
		else if( Pokemod_Alive(id) )//if they are dead, what are we showing them??
			message_begin(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, _, id) //message begin
		else
			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
		write_byte(TE_BEAMENTS)
		write_short(start) // start entity
		write_short(end) // end entity
		write_short(sprite) // sprite index
		write_byte(frame) // starting frame
		write_byte(rate) // frame rate in 0.1's
		write_byte(life) // life in 0.1's
		write_byte(width) // line width in 0.1's
		write_byte(noise) // noise amplitude in 0.01's
		write_byte(r) // Red
		write_byte(g) // Green
		write_byte(b) // Blue
		write_byte(a) // brightness
		write_byte(scroll) // scroll speed in 0.1's
		message_end()
	}

	#if SPEC_MSGS == 1
	if(!spec_msg)	MakeLine_Ent(id,start,end,sprite,frame,rate,life,width,noise,r,g,b,a,scroll,true);
	#endif
 }
 //makes a beam between the two origins
 stock MakeLine(id,Start[3],End[3],sprite,frame,rate,life,width,noise,r,g,b,a,scroll,bool:specmsg=false)
 {
	if( !is_user_bot(id) && !specmsg ){
		if( specmsg )
			message_begin(MSG_SPEC, SVC_TEMPENTITY, _, id) //message begin
		else if( Pokemod_Alive(id) )//if they are dead, what are we showing them??
			message_begin(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, _, id) //message begin
		else
			message_begin(MSG_BROADCAST, SVC_TEMPENTITY) //message begin
		write_byte(TE_BEAMPOINTS)
		write_coord(Start[0]) // start position
		write_coord(Start[1])
		write_coord(Start[2])
		write_coord(End[0]) // end position
		write_coord(End[1])
		write_coord(End[2])
		write_short(sprite) // sprite index
		write_byte(frame) // starting frame
		write_byte(rate) // frame rate in 0.1's
		write_byte(life) // life in 0.1's
		write_byte(width) // line width in 0.1's
		write_byte(noise) // noise amplitude in 0.01's
		write_byte(r) // Red
		write_byte(g) // Green
		write_byte(b) // Blue
		write_byte(a) // brightness
		write_byte(scroll) // scroll speed in 0.1's
		message_end()
	}

	#if SPEC_MSGS == 1
	if(!spec_msg)	MakeLine_Ent(id,Start,End,sprite,frame,rate,life,width,noise,r,g,b,a,scroll,true);
	#endif
 }
 
 
 //Thank you from AMXX NS unstuck plugin
 stock poke_UnStuck(id, bool:kill=true)
 {
	#if defined _pokemod_plugin_included
	new pokecenter = EntIsPC(id);
	if(!Pokemod_Alive(id) && !pokecenter)
		return true;

	#else
	if(!Pokemod_Alive(id))
		return true;
	#endif

	new hullsize = poke_getHullSize(id);
	if(!hullsize) 
		return true;

	new Float:origin[3], Float:new_origin[3], distance;
	pev(id, pev_origin, origin);
	distance = UNSTUCK_START_DISTANCE;

	while( distance < 1000 ) {	// 1000 is just incase, should never get anywhere near that
		for (new i = 0; i < UNSTUCK_MAX_ATTEMPTS; ++i) {
			new_origin[0] = random_float(origin[0]-distance,origin[0]+distance);
			new_origin[1] = random_float(origin[1]-distance,origin[1]+distance);
			new_origin[2] = random_float(origin[2]-distance,origin[2]+distance);

			if ( fm_trace_hull(new_origin, hullsize, id) == 0 ) {
				fm_entity_set_origin(id, new_origin);
				return true;
			}
		}
		distance += UNSTUCK_START_DISTANCE;
	}

	#if defined _pokemod_plugin_included
	if( pokecenter )
		return false;
	#endif

	if( kill ){
		user_kill(id);
		poke_print(id, print_chat, "Couldn't find a free spot to move you too.");
	}
	return false;
 }
 
 stock poke_getHullSize(id)
 {
	#if defined _pokemod_plugin_included
		if( EntIsPC(id) )
			return HULL_HUMAN;
	#endif
	#if MOD==NS
		switch (ns_get_class(id))
		{
			case CLASS_SKULK,CLASS_GORGE,CLASS_LERK:
				return HULL_HEAD;
			case CLASS_FADE,CLASS_MARINE,CLASS_JETPACK,CLASS_HEAVY:
				return (pev(id, pev_flags) & FL_DUCKING) ? HULL_HEAD : HULL_HUMAN;
			case CLASS_ONOS:
				return (pev(id, pev_flags) & FL_DUCKING) ? HULL_HUMAN : HULL_LARGE;
			default: {
				return false;
			}
		}
		return false;
	#else
		return (pev(id, pev_flags) & FL_DUCKING) ? HULL_HEAD : HULL_HUMAN;
	#endif
 } 
 // Thank you SH mod
 stock strbrkqt(const text[], Left[], leftLen, Right[], rightLen)
 {
	//Breaks text[] into two parts, Left[], and Right[]
	// Left[] will contain the first parameter (either quoted or non-quoted)
	// Right[] contain the rest of the string after Left[], not including the space
	new bool:in_quotes = false
	new bool:done_flag = false
	new i, left_pos = 0

	for ( i = 0; i < strlen(text); i++) {

		if (equali(text[i], "\"", 1) && !done_flag) {
			if (in_quotes) {
				done_flag = true
				in_quotes = false
			}
			else in_quotes = true
		}
		else if ( isspace(text[i]) && !in_quotes ) {
			if (left_pos > 0) {
				done_flag = true
			}
		}
		else if (!done_flag && left_pos <= leftLen) {
			setc(Left[left_pos], 1, text[i])
			left_pos++
		}
		else if (done_flag) break
	}

	Left[left_pos] = 0
	copy(Right,rightLen,text[i])

	return true
 }
 
 stock is_num_odd( num )
	return ( num & 1 );

 stock fm_find_sphere_class(ent, const _classname[], Float:radius, entlist[], maxents, Float:origin[3]={0.0,0.0,0.0})
 {
	if( pev_valid(ent) )
		pev( ent, pev_origin, origin );
	new tempent, tempclass[32], entsfound;
	while( (tempent = fm_find_ent_in_sphere(tempent, origin, radius) ) && entsfound < maxents )
	{
		if( pev_valid(tempent) ){
			pev(tempent, pev_classname, tempclass, 31);
			if( equal( _classname, tempclass ) )
				entlist[entsfound++] = tempent;
		}
	}
	return entsfound;
 }
 
 stock entity_set_follow(entity, target, Float:speed)
 {
	if ( !pev_valid(entity) || !pev_valid(target) ) return 0

	new Float:fl_Origin[3], Float:fl_EntOrigin[3]
	pev(target, pev_origin, fl_Origin)
	pev(entity, pev_origin, fl_EntOrigin)

	new Float:fl_InvTime = (speed / vector_distance(fl_Origin, fl_EntOrigin))

	new Float:fl_Velocity[3]
	fl_Velocity[0] = (fl_Origin[0] - fl_EntOrigin[0]) * fl_InvTime
	fl_Velocity[1] = (fl_Origin[1] - fl_EntOrigin[1]) * fl_InvTime
	fl_Velocity[2] = (fl_Origin[2] - fl_EntOrigin[2]) * fl_InvTime

	set_pev(entity, pev_velocity, fl_Velocity)

	new Float:fl_NewAngle[3]
	vector_to_angle(fl_Velocity, fl_NewAngle)
	set_pev(entity, pev_angles, fl_NewAngle)

	return 1
 }
 



