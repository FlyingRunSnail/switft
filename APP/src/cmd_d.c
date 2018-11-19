#include <stdlib.h>
#include "command.h"
#include "cmd_d.h"


static int show_memory(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int val;
	unsigned int *addr;
	
	if (argc != 2)
	{
		return -1;
	}
	
	val = strtoul(argv[1], NULL, 16);
	addr = (unsigned int *)val;
#if 1
	printf("0x%p 0x%08x\r\n", addr, *addr);
	printf("\r\n");
#else
	printf("0x%p 0x%08x\n", addr, *addr);
#endif
	return 0;
}

int cmd_d_init(void)
{
	int ret;
	
	ret = cmd_register(
		"d", SHELL_SYS_MAXARGS, 0,	show_memory,
		"start application at address 'addr'",
		"addr [arg ...]\n	 - start application at address 'addr'\n"
		"	   passing 'arg' as arguments"
	);

	return ret;
}


