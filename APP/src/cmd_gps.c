#include <stdlib.h>
#include "command.h"
#include "cmd_gps.h"
#include "exports.h"
#include "hw_gps.h"


static int do_gps(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2) 
	{
		return -1;
	}

    if (!strcmp(argv[1], "on"))
    {
        HW_GPS_On();
    }
    else if (!strcmp(argv[1], "off"))
    {
        HW_GPS_Off();
    }
    else if (!strcmp(argv[1], "reset"))
    {
        HW_GPS_Rst();
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_gps_init(void)
{
	int ret;
	
	ret = cmd_register(
		"gps", SHELL_SYS_MAXARGS, 0, do_gps,
		"gps on/off/reset",
		"gps on/off/reset\r\n	 - control the gps on/off/reset\r\n"
	);

	return ret;
}


