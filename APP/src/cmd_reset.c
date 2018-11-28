#include <stdlib.h>
#include "bsp.h"
#include "command.h"
#include "cmd_reset.h"
#include "exports.h"


int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("Resetting the board...\n");

	BSP_SoftReset();

	/*NOTREACHED*/
	return 0;
}

int cmd_reset_init(void)
{
	int ret;
	
	ret = cmd_register(
		"reset", 1, 0,	do_reset,
		"Perform RESET of the CPU",
		""
	);
	
	return ret;
}


