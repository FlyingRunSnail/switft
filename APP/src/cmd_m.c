#include <stdlib.h>
#include "command.h"
#include "cmd_m.h"


static int modify_memory(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int val;
	unsigned int *addr;
	
	if (argc != 3)
	{
		return -1;
	}
	
	val = strtoul(argv[1], NULL, 16);
	addr = (unsigned int *)val;
	val = strtoul(argv[2], NULL, 16);
	*addr = val;

	return 0;
}

int cmd_m_init(void)
{
	int ret;
	
	ret = cmd_register(
		"m", SHELL_SYS_MAXARGS, 0,	modify_memory,
		"start application at address 'addr'",
		"addr [arg ...]\n	 - start application at address 'addr'\n"
		"	   passing 'arg' as arguments"
	);

	return ret;
}


