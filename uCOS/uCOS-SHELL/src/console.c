// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000
 * Paolo Scaffardi, AIRVENT SAM s.p.a - RIMINI(ITALY), arsenio@tin.it
 */

#include <stdarg.h>
#include <stdio.h>
#include "console.h"
#include "command.h"
//#include "hw_uart1.h"
#include "hw_uart2.h"

int cgetc(void)
{
	return SWIFT_USART2_GETC();
}

int ctstc(void)
{
	return SWIFT_USART2_TSTC();
}

void cputc(char c)
{
	SWIFT_UART2_SendBuff(&c, 1);
}

void cputs(char *s)
{
	/* Send directly to the handler */
	SWIFT_UART2_SendBuff(s, strlen(s));
}

#ifdef __GNUC__
int printf(const char *fmt, ...)
{
    va_list args;
    uint i;
    char printbuffer[SHELL_SYS_PBSIZE];

    va_start(args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = vsnprintf(printbuffer, sizeof(printbuffer), fmt, args);
    va_end(args);

    /* Print the string */
    cputs(printbuffer);
    return i;
}
#endif

/* test if ctrl-c was pressed */
static int ctrlc_disabled = 0;	/* see disable_ctrl() */
static int ctrlc_was_pressed = 0;
int ctrlc(void)
{
	if (!ctrlc_disabled) {
		if (ctstc()) {
			switch (cgetc()) {
			case 0x03:		/* ^C - Control C */
				ctrlc_was_pressed = 1;
				return 1;
			default:
				break;
			}
		}
	}

	return 0;
}

/* Reads user's confirmation.
   Returns 1 if user's input is "y", "Y", "yes" or "YES"
*/
int confirm_yesno(void)
{
	int i;
	char str_input[5];

	/* Flush input */
	while (ctstc())
		cgetc();
	i = 0;
	while (i < sizeof(str_input)) {
		str_input[i] = cgetc();
		cputc(str_input[i]);
		if (str_input[i] == '\r')
			break;
		i++;
	}
	cputc('\n');
	if (strncmp(str_input, "y\r", 2) == 0 ||
	    strncmp(str_input, "Y\r", 2) == 0 ||
	    strncmp(str_input, "yes\r", 4) == 0 ||
	    strncmp(str_input, "YES\r", 4) == 0)
		return 1;
	return 0;
}

/* pass 1 to disable ctrlc() checking, 0 to enable.
 * returns previous state
 */
int disable_ctrlc(int disable)
{
	int prev = ctrlc_disabled;	/* save previous state */

	ctrlc_disabled = disable;
	return prev;
}

int had_ctrlc (void)
{
	return ctrlc_was_pressed;
}

void clear_ctrlc(void)
{
	ctrlc_was_pressed = 0;
}

