#include <stdlib.h>
#include "command.h"
#include "cmd_power.h"
#include "exports.h"
#include "hw_power.h"


static int do_power(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 3)
	{
		return -1;
	}

    if (!strcmp(argv[1], "12v"))
    {
        if (!strcmp(argv[2], "on"))
        {
            HW_12V_On();
        }
        else if (!strcmp(argv[2], "off"))
        {
            HW_12V_Off();
        }
        else
        {
            return -1;
        }
    }
    else if (!strcmp(argv[1], "bat"))
    {
        if (!strcmp(argv[2], "on"))
        {
            HW_BAT_On();
        }
        else if (!strcmp(argv[2], "off"))
        {
            HW_BAT_Off();
        }
        else
        {
            return -1;
        }
    }
    else if (!strcmp(argv[1], "ap"))
    {
        if (!strcmp(argv[2], "on"))
        {
            HW_AP_On();
        }
        else if (!strcmp(argv[2], "off"))
        {
            HW_AP_Off();
        }
        else
        {
            return -1;
        }
    }
    else if (!strcmp(argv[1], "4g"))
    {
        if (!strcmp(argv[2], "on"))
        {
            HW_4G_On();
        }
        else if (!strcmp(argv[2], "off"))
        {
            HW_4G_Off();
        }
        else
        {
            return -1;
        }
    }
    else if (!strcmp(argv[1], "mesh"))
    {
        if (!strcmp(argv[2], "on"))
        {
            HW_MESH_On();
        }
        else if (!strcmp(argv[2], "off"))
        {
            HW_MESH_Off();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

	return 0;
}

int cmd_power_init(void)
{
	int ret;
	
	ret = cmd_register(
		"power", SHELL_SYS_MAXARGS, 0, do_power,
		"power 12v/bat/ap/4g/mesh on/off",
		"power 12v/bat/ap/4g/mesh on/off\r\n	 - control the power on or off\r\n"
	);

	return ret;
}


