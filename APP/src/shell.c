#include "app_shell.h"
#include "cli.h"
#include "cmd_help.h"
#include "cmd_ver.h"
#include "cmd_d.h"
#include "cmd_m.h"
#include "cmd_reset.h"



/***********************************************************
**name:	APP_Shell_Init
**describe: shell init functions
**input:	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_Shell_Init(void)
{
	// init uart1 as debug com
	SWIFT_UART1_Init(115200);

	cmd_help_init();
	cmd_ver_init();
	cmd_d_init();
	cmd_m_init();
	cmd_reset_init();

	cli_init();
	
	return SUCC;
}

