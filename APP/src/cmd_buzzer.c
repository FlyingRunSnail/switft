#include <stdlib.h>
#include "command.h"
#include "cmd_buzzer.h"
#include "exports.h"
#include "hw_buzzer.h"


static int do_buzzer(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2)
	{
		return -1;
	}

    if (!strcmp(argv[1], "on"))
    {
        HW_BUZZER_On();
    }
    else
    {
        HW_BUZZER_Off();
    }
	
	return 0;
}

int cmd_buzzer_init(void)
{
	int ret;
	
	ret = cmd_register(
		"buzzer", SHELL_SYS_MAXARGS, 0,	show_memory,
		"start or stop buzzer",
		"buzzer on\r\n	 - start buzzer work\r\n"
		"buzzer off\r\n  - stop buzzer work\r\n"
	);

	return ret;
}


