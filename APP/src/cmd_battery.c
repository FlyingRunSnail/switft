#include <stdlib.h>
#include "command.h"
#include "cmd_battery.h"
#include "exports.h"
#include "hw_battery.h"


static int do_battery(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    INT8U buf[34] = {0};
    INT32U len = 0;

	if (argc != 2) 
	{
		return -1;
	}

    if (!strcmp(argv[1], "info"))
    {
        HW_BATTERY_Read(buf, &len);
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_battery_init(void)
{
	int ret;
	
	ret = cmd_register(
		"battery", SHELL_SYS_MAXARGS, 0, do_battery,
		"battery info",
		"battery info\r\n	 - display battery info\r\n"
	);

	return ret;
}


