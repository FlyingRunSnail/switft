#include "command.h"
#include "cmd_ver.h"


static int show_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if 1
	printf("Hardware version:  %s\r\n", "\tVer.A");
	printf("Software version: %s\r\n", "\t1.0.0.0");
	printf("build time:\t\t%s %s\r\n", __DATE__, __TIME__);
	printf("\r\n");
#else
	printf("Hardware version:  %s\n", "\tVer.A");
	printf("Software version: %s\n", "\t1.0.0.0");
	printf("build time:\t\t%s %s\n", __DATE__, __TIME__);
#endif
	return 0;
}

int cmd_ver_init(void)
{
	int ret;
	
	ret = cmd_register(
		"ver", SHELL_SYS_MAXARGS, 0,	show_version,
		"start application at address 'addr'",
		"addr [arg ...]\n	 - start application at address 'addr'\n"
		"	   passing 'arg' as arguments"
	);

	return ret;
}


