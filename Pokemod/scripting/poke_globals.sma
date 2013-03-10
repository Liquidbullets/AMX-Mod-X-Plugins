#include <amxmodx>

public plugin_init() {
	register_plugin("Poke_globals","0.1","Firippu")

	register_clcmd("say /globals","show_globals")
}

public show_globals(id) {
	show_motd(id,"http://starwolf.net/misc/poke_globals.html")

	return PLUGIN_HANDLED
}
