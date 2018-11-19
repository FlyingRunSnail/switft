// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

/*
 *  Command Processor Table
 */

#include <command.h>
#include <console.h>

#define MAX_CMD_LINE (32)

static unsigned short cmd_index = 0;
cmd_tbl_t cmd_tbl[MAX_CMD_LINE] = {0};

cmd_tbl_t *cmd_entry_start(void)
{
	return &cmd_tbl[0];
}

int cmd_entry_count(void)
{
	return MAX_CMD_LINE;
}

int cmd_register(char *name, int maxargs, int repeatable, 
	int (*cmd)(struct cmd_tbl_s *, int, int, char * const []), char *usage, char *help)
{
	if (cmd_index >= MAX_CMD_LINE)
	{
		return CMD_REGISTER_FAIL;
	}
	
	cmd_tbl[cmd_index].name = name;
	cmd_tbl[cmd_index].maxargs = maxargs;
	cmd_tbl[cmd_index].repeatable = repeatable;
	cmd_tbl[cmd_index].cmd = cmd;
	cmd_tbl[cmd_index].usage = usage;
	cmd_tbl[cmd_index].help = help;
	cmd_tbl[cmd_index].complete = NULL;

	cmd_index++;
	return CMD_REGISTER_OK;
}

/*
 * Use puts() instead of printf() to avoid printf buffer overflow
 * for long help messages
 */

int _do_help(cmd_tbl_t *cmd_start, int cmd_items, cmd_tbl_t *cmdtp, int flag,
	     int argc, char * const argv[])
{
	int i, swaps;
	int rcode = 0;
	cmd_tbl_t *tmp = NULL;
	char *usage = NULL;

	if (argc == 1) {	/* show list of commands */
		/* print short help (usage) */
		for (i = 0; i < cmd_items; i++) {
			tmp = &cmd_start[i];
			usage = tmp->usage;

			/* allow user abort */
			if (ctrlc())
				return 1;
			if (usage == NULL)
				continue;
			printf("%-*s- %s\r\n", CONFIG_SYS_HELP_CMD_WIDTH,
			       tmp->name, usage);
		}
		return 0;
	}
	/*
	 * command help (long version)
	 */
	for (i = 1; i < argc; ++i) {
		cmdtp = find_cmd_tbl(argv[i], cmd_start, cmd_items);
		if (cmdtp != NULL) {
			rcode |= cmd_usage(cmdtp);
		} else {
			printf("Unknown command '%s' - try 'help' without arguments for list of all known commands\n\n",
			       argv[i]);
			rcode = 1;
		}
	}
	return rcode;
}

/* find command table entry for a command */
cmd_tbl_t *find_cmd_tbl(const char *cmd, cmd_tbl_t *table, int table_len)
{
	cmd_tbl_t *cmdtp;
	cmd_tbl_t *cmdtp_temp = table;	/* Init value */
	const char *p;
	int len;
	int n_found = 0;

	if (!cmd)
		return NULL;

	len = strlen (cmd);

	for (cmdtp = table; cmdtp != table + table_len; cmdtp++) {
		if (strncmp(cmd, cmdtp->name, len) == 0) {
			if (len == strlen(cmdtp->name))
				return cmdtp;	/* full match */

			cmdtp_temp = cmdtp;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1) {			/* exactly one match */
		return cmdtp_temp;
	}

	return NULL;	/* not found or ambiguous command */
}

cmd_tbl_t *find_cmd(const char *cmd)
{
	return find_cmd_tbl(cmd, cmd_tbl, MAX_CMD_LINE);
}

int cmd_usage(const cmd_tbl_t *cmdtp)
{
	printf("%s - %s\r\n", cmdtp->name, cmdtp->usage);

	printf("Usage:\r\n%s ", cmdtp->name);

	if (!cmdtp->help) {
		puts ("- No additional help available.\r\n");
		return 1;
	}

	puts(cmdtp->help);
	puts("\r\n");

	return 1;
}

#if 0
static int complete_cmdv(int argc, char * const argv[], char last_char, int maxv, char *cmdv[])
{
	cmd_tbl_t *cmdtp = ll_entry_start(cmd_tbl_t, cmd);
	const int count = ll_entry_count(cmd_tbl_t, cmd);
	const cmd_tbl_t *cmdend = cmdtp + count;
	const char *p;
	int len, clen;
	int n_found = 0;
	const char *cmd;

	/* sanity? */
	if (maxv < 2)
		return -2;

	cmdv[0] = NULL;

	if (argc == 0) {
		/* output full list of commands */
		for (; cmdtp != cmdend; cmdtp++) {
			if (n_found >= maxv - 2) {
				cmdv[n_found++] = "...";
				break;
			}
			cmdv[n_found++] = cmdtp->name;
		}
		cmdv[n_found] = NULL;
		return n_found;
	}

	/* more than one arg or one but the start of the next */
	if (argc > 1 || last_char == '\0' || isblank(last_char)) {
		cmdtp = find_cmd(argv[0]);
		if (cmdtp == NULL || cmdtp->complete == NULL) {
			cmdv[0] = NULL;
			return 0;
		}
		return (*cmdtp->complete)(argc, argv, last_char, maxv, cmdv);
	}

	cmd = argv[0];
	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	p = (char *)strchr(cmd, '.');
	if (p == NULL)
		len = strlen(cmd);
	else
		len = p - cmd;

	/* return the partial matches */
	for (; cmdtp != cmdend; cmdtp++) {

		clen = strlen(cmdtp->name);
		if (clen < len)
			continue;

		if (memcmp(cmd, cmdtp->name, len) != 0)
			continue;

		/* too many! */
		if (n_found >= maxv - 2) {
			cmdv[n_found++] = "...";
			break;
		}

		cmdv[n_found++] = cmdtp->name;
	}

	cmdv[n_found] = NULL;
	return n_found;
}
#endif

static int make_argv(char *s, int argvsz, char *argv[])
{
	int argc = 0;

	/* split into argv */
	while (argc < argvsz - 1) {

		/* skip any white space */
		while (isblank(*s))
			++s;

		if (*s == '\0')	/* end of s, no more args	*/
			break;

		argv[argc++] = s;	/* begin of argument string	*/

		/* find end of string */
		while (*s && !isblank(*s))
			++s;

		if (*s == '\0')		/* end of s, no more args	*/
			break;

		*s++ = '\0';		/* terminate current arg	 */
	}
	argv[argc] = NULL;

	return argc;
}

static void print_argv(const char *banner, const char *leader, const char *sep, int linemax, char * const argv[])
{
	int ll = leader != NULL ? strlen(leader) : 0;
	int sl = sep != NULL ? strlen(sep) : 0;
	int len, i;

	if (banner) {
		puts("\n");
		puts(banner);
	}

	i = linemax;	/* force leader and newline */
	while (*argv != NULL) {
		len = strlen(*argv) + sl;
		if (i + len >= linemax) {
			puts("\n");
			if (leader)
				puts(leader);
			i = ll - sl;
		} else if (sep)
			puts(sep);
		puts(*argv++);
		i += len;
	}
	printf("\n");
}

static int find_common_prefix(char * const argv[])
{
	int i, len;
	char *anchor, *s, *t;

	if (*argv == NULL)
		return 0;

	/* begin with max */
	anchor = *argv++;
	len = strlen(anchor);
	while ((t = *argv++) != NULL) {
		s = anchor;
		for (i = 0; i < len; i++, t++, s++) {
			if (*t != *s)
				break;
		}
		len = s - anchor;
	}
	return len;
}

static char tmp_buf[SHELL_SYS_CBSIZE + 1];	/* copy of console I/O buffer */

int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp)
{
	int n = *np, col = *colp;
	char *argv[SHELL_SYS_MAXARGS + 1];		/* NULL terminated	*/
	char *cmdv[20];
	char *s, *t;
	const char *sep;
	int i, j, k, len, seplen, argc;
	int cnt;
	char last_char;

	if (strcmp(prompt, SHELL_SYS_PROMPT) != 0)
		return 0;	/* not in normal console */

	cnt = strlen(buf);
	if (cnt >= 1)
		last_char = buf[cnt - 1];
	else
		last_char = '\0';

	/* copy to secondary buffer which will be affected */
	strcpy(tmp_buf, buf);

	/* separate into argv */
	argc = make_argv(tmp_buf, sizeof(argv)/sizeof(argv[0]), argv);

	/* do the completion and return the possible completions */
	//i = complete_cmdv(argc, argv, last_char, sizeof(cmdv) / sizeof(cmdv[0]), cmdv);

	/* no match; bell and out */
	if (i == 0) {
		if (argc > 1)	/* allow tab for non command */
			return 0;
		putc('\a');
		return 1;
	}

	s = NULL;
	len = 0;
	sep = NULL;
	seplen = 0;
	if (i == 1) { /* one match; perfect */
		k = strlen(argv[argc - 1]);
		s = cmdv[0] + k;
		len = strlen(s);
		sep = " ";
		seplen = 1;
	} else if (i > 1 && (j = find_common_prefix(cmdv)) != 0) { /* more */
		k = strlen(argv[argc - 1]);
		j -= k;
		if (j > 0) {
			s = cmdv[0] + k;
			len = j;
		}
	}

	if (s != NULL) {
		k = len + seplen;
		/* make sure it fits */
		if (n + k >= SHELL_SYS_CBSIZE - 2) {
			putc('\a');
			return 1;
		}

		t = buf + cnt;
		for (i = 0; i < len; i++)
			*t++ = *s++;
		if (sep != NULL)
			for (i = 0; i < seplen; i++)
				*t++ = sep[i];
		*t = '\0';
		n += k;
		col += k;
		puts(t - k);
		if (sep == NULL)
			putc('\a');
		*np = n;
		*colp = col;
	} else {
		print_argv(NULL, "  ", " ", 78, cmdv);

		puts(prompt);
		puts(buf);
	}
	return 1;
}


/**
 * Call a command function. This should be the only route in U-Boot to call
 * a command, so that we can track whether we are waiting for input or
 * executing a command.
 *
 * @param cmdtp		Pointer to the command to execute
 * @param flag		Some flags normally 0 (see CMD_FLAG_.. above)
 * @param argc		Number of arguments (arg 0 must be the command text)
 * @param argv		Arguments
 * @return 0 if command succeeded, else non-zero (CMD_RET_...)
 */
static int cmd_call(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int result;

	result = (cmdtp->cmd)(cmdtp, flag, argc, argv);
	return result;
}

enum command_ret_t cmd_process(int flag, int argc, char * const argv[],
			       int *repeatable, unsigned int *ticks)
{
	enum command_ret_t rc = CMD_RET_SUCCESS;
	cmd_tbl_t *cmdtp;

	/* Look up command in command table */
	cmdtp = find_cmd(argv[0]);
	if (cmdtp == NULL) {
		printf("Unknown command '%s' - try 'help'\n", argv[0]);
		return 1;
	}

	/* found - check max args */
	if (argc > cmdtp->maxargs)
		rc = CMD_RET_USAGE;

	/* If OK so far, then do the command */
	if (!rc) {
		rc = cmd_call(cmdtp, flag, argc, argv);
		*repeatable &= cmdtp->repeatable;
	}
	if (rc == CMD_RET_USAGE)
		rc = cmd_usage(cmdtp);
	return rc;
}

int cmd_process_error(cmd_tbl_t *cmdtp, int err)
{
	if (err == CMD_RET_USAGE)
		return CMD_RET_USAGE;

	if (err) {
		printf("Command '%s' failed: Error %d\n", cmdtp->name, err);
		return CMD_RET_FAILURE;
	}

	return CMD_RET_SUCCESS;
}

