#pragma ctrlchar '\'

#include <amxmodx>
#include <pokemod>

new rankpokes[][] = {
	"Moltres",
	"Zapdos",
	"Articuno",
	"Mewtwo",
	"Mew"
}
new rankpoints[] = {
	25000,
	50000,
	75000,
	100000,
	200000
}

#define MAX_PLAYERS 32
new oldrank[MAX_PLAYERS+1];

public plugin_init()
{
	register_plugin("PokeMod Rank Pokes","0.1","Emp`")
}
public poke_player_event(id, event)
{
	if( event & SI_SPAWN ){
		static i, newrank; newrank = Poke_PlayerRank(id);
		for( i=0; i<sizeof rankpoints; i++ ){
			if( oldrank[id] < rankpoints[i] && newrank >= rankpoints[i] ){
				if( Poke_Give_Poke(id, Poke_NameToPoke( rankpokes[i] ) ) ){
					new name[32]
					get_user_name(id, name, 31)
					client_print(0, print_chat, "[PokeMod] Congratulations %s! You have received a free %s for attaining a PokeRank of %d!", name, rankpokes[i], rankpoints[i] );
					oldrank[id] = newrank;
				}
				else {
					client_print(id, print_chat, "[PokeMod] Please make room in your party for your free %s!", rankpokes[i] );
				}
			}
			else
				oldrank[id] = newrank;
		}
	}
}