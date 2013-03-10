#include <amxmodx>

public plugin_init() {
	register_plugin("Poke_buy","0.1","Firippu")

	register_clcmd("say /buypokes","show_buy")
}

public show_buy(id) {
	show_motd(id,"http://starwolf.net/misc/poke_buy.html")

	return PLUGIN_HANDLED
}
