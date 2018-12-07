#include "app_shell.h"
#include "cli.h"
#include "cmd_help.h"
#include "cmd_ver.h"
#include "cmd_d.h"
#include "cmd_m.h"
#include "cmd_reset.h"
#include "cmd_buzzer.h"
#include "cmd_phy.h"
#include "cmd_power.h"
#include "cmd_gps.h"
#include "cmd_battery.h"
#include "cmd_ap.h"



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
	// init uart2 as debug com
	SWIFT_UART2_Init(115200);

	cmd_help_init();
	cmd_ver_init();
	cmd_d_init();
	cmd_m_init();
	cmd_reset_init();
	cmd_buzzer_init();
	cmd_phy_init();
	cmd_power_init();
	cmd_gps_init();
	cmd_battery_init();
	cmd_ap_init();

	cli_init();
	
	return SUCC;
}

