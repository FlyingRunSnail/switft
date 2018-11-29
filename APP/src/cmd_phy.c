#include <stdlib.h>
#include "command.h"
#include "cmd_phy.h"
#include "exports.h"
#include "hw_buzzer.h"


static int do_phy(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2)
	{
		return -1;
	}

    if (!strcmp(argv[1], "reset"))
    {
	    BSP_GPIO_PinWriteLow(GPIOF, GPIO_Pin_9);
        BSP_Delay_us(2);
	    BSP_GPIO_PinWriteHigh(GPIOF, GPIO_Pin_9);
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_phy_init(void)
{
	int ret;
	
	ret = cmd_register(
		"phy", SHELL_SYS_MAXARGS, 0, do_phy,
		"phy reset",
		"phy reset\r\n	 - reset the phy\r\n"
	);

	return ret;
}


