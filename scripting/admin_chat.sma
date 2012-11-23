#include <amxmodx>
#include <colorchat>

#define VERSION "0.0.1"

new iMaxPlayers

public plugin_init() {
	register_plugin("admin_chat",VERSION,"Firippu")

	iMaxPlayers=get_maxplayers()

	register_concmd("say","say_hook")
}

public say_hook(id,level,cid) {
	new args[128]
	read_args(args,charsmax(args))
	remove_quotes(args)

	new szCommand[4]
	strbreak(args,szCommand,charsmax(szCommand),args,charsmax(args))

	if(equali(szCommand,"u@")) {
		if(strlen(args[0])!=0) {
			new szName[32]
			get_user_name(id,szName,31)

			for(new iAdmin=1; iAdmin<=iMaxPlayers; iAdmin++) {
				if(get_user_flags(iAdmin) & ADMIN_KICK) {
					ColorChat(iAdmin,NORMAL,"^4%s : ^4%s",szName,args[0])
				}
			}

			ColorChat(id,NORMAL,"^4Message Sent")
		} else {
			ColorChat(id,NORMAL,"^4Message Failed")
		}

		return PLUGIN_HANDLED
	}

	return PLUGIN_CONTINUE
}
