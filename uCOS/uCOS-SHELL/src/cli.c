// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
 */

#include <cli.h>
//#include <cli_hush.h>
#include <console.h>


/*
 * Run a command using the selected parser, and check if it is repeatable.
 *
 * @param cmd	Command to run
 * @param flag	Execution flags (CMD_FLAG_...)
 * @return 0 (not repeatable) or 1 (repeatable) on success, -1 on error.
 */
int run_command_repeatable(const char *cmd, int flag)
{
#if 1
	return cli_simple_run_command(cmd, flag);
#else
	/*
	 * parse_string_outer() returns 1 for failure, so clean up
	 * its result.
	 */
	if (parse_string_outer(cmd,0x3))
		return -1;

	return 0;
#endif
}

void cli_loop(void)
{
	cli_simple_loop();
}

void cli_init(void)
{
	;
}

