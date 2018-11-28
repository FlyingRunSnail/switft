#include <stdlib.h>
#include "command.h"
#include "cmd_help.h"
#include "exports.h"


static int do_help(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *start = cmd_entry_start();
	int len = cmd_entry_count();
	
	return _do_help(start, len, cmdtp, flag, argc, argv);
}

int cmd_help_init(void)
{
	int ret;
	
	ret = cmd_register(
		"help",	SHELL_SYS_MAXARGS,	0,	do_help,
		"print command description/usage",
		"\n"
		"	- print brief description of all commands\n"
		"help command ...\n"
		"	- print detailed usage of 'command'"
	);

	ret = cmd_register(
		"?",	SHELL_SYS_MAXARGS,	0,	do_help,
		"alias for 'help'",
		""
	);

	return ret;
}
