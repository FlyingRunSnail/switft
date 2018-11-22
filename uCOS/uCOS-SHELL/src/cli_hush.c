// SPDX-License-Identifier: GPL-2.0+
/*
 * sh.c -- a prototype Bourne shell grammar parser
 *      Intended to follow the original Thompson and Ritchie
 *      "small and simple is beautiful" philosophy, which
 *      incidentally is a good match to today's BusyBox.
 *
 * Copyright (C) 2000,2001  Larry Doolittle  <larry@doolittle.boa.org>
 *
 * Credits:
 *      The parser routines proper are all original material, first
 *      written Dec 2000 and Jan 2001 by Larry Doolittle.
 *      The execution engine, the builtins, and much of the underlying
 *      support has been adapted from busybox-0.49pre's lash,
 *      which is Copyright (C) 2000 by Lineo, Inc., and
 *      written by Erik Andersen <andersen@lineo.com>, <andersee@debian.org>.
 *      That, in turn, is based in part on ladsh.c, by Michael K. Johnson and
 *      Erik W. Troan, which they placed in the public domain.  I don't know
 *      how much of the Johnson/Troan code has survived the repeated rewrites.
 * Other credits:
 *      b_addchr() derived from similar w_addchar function in glibc-2.2
 *      setup_redirect(), redirect_opt_num(), and big chunks of main()
 *        and many builtins derived from contributions by Erik Andersen
 *      miscellaneous bugfixes from Matt Kraai
 *
 * There are two big (and related) architecture differences between
 * this parser and the lash parser.  One is that this version is
 * actually designed from the ground up to understand nearly all
 * of the Bourne grammar.  The second, consequential change is that
 * the parser and input reader have been turned inside out.  Now,
 * the parser is in control, and asks for input as needed.  The old
 * way had the input reader in control, and it asked for parsing to
 * take place as needed.  The new way makes it much easier to properly
 * handle the recursion implicit in the various substitutions, especially
 * across continuation lines.
 *
 * Bash grammar not implemented: (how many of these were in original sh?)
 *      $@ (those sure look like weird quoting rules)
 *      $_
 *      ! negation operator for pipes
 *      &> and >& redirection of stdout+stderr
 *      Brace Expansion
 *      Tilde Expansion
 *      fancy forms of Parameter Expansion
 *      aliases
 *      Arithmetic Expansion
 *      <(list) and >(list) Process Substitution
 *      reserved words: case, esac, select, function
 *      Here Documents ( << word )
 *      Functions
 * Major bugs:
 *      job handling woefully incomplete and buggy
 *      reserved word execution woefully incomplete and buggy
 * to-do:
 *      port selected bugfixes from post-0.49 busybox lash - done?
 *      finish implementing reserved words: for, while, until, do, done
 *      change { and } from special chars to reserved words
 *      builtins: break, continue, eval, return, set, trap, ulimit
 *      test magic exec
 *      handle children going into background
 *      clean up recognition of null pipes
 *      check setting of global_argc and global_argv
 *      control-C handling, probably with longjmp
 *      follow IFS rules more precisely, including update semantics
 *      figure out what to do with backslash-newline
 *      explain why we use signal instead of sigaction
 *      propagate syntax errors, die on resource errors?
 *      continuation lines, both explicit and implicit - done?
 *      memory leak finding and plugging - done?
 *      more testing, especially quoting rules and redirection
 *      document how quoting rules not precisely followed for variable assignments
 *      maybe change map[] to use 2-bit entries
 *      (eventually) remove all the printf's
 */


//#include <common.h>        /* readline */
#include <console.h>
#include <cli.h>
#include <cli_hush.h>
#include <command.h>        /* find_cmd */

#define SPECIAL_VAR_SYMBOL 03
#define SUBSTED_VAR_SYMBOL 04

#define EXIT_SUCCESS 0
#define EOF -1
#define syntax() syntax_err()
#define xmalloc malloc
#define error_msg printf
typedef unsigned int size_t; 

#define max(a,b) ((a > b) ? a : b)

static void *xrealloc(void *ptr, size_t size)
{
	void *p = NULL;

	p = (void *)malloc(size);
	if (!p)
	{
		return NULL;
	}

	memcpy(p, ptr, size);
	free(ptr);

	return p;
}

char * xstrdup(const char *s)
{
	char *new;

	if ((s == NULL)	||	((new = (char *)malloc (strlen(s) + 1)) == NULL) ) 
	{
		return NULL;
	}

	strcpy (new, s);
	return new;
}

typedef enum {
	PIPE_SEQ = 1,
	PIPE_AND = 2,
	PIPE_OR  = 3,
	PIPE_BG  = 4,
} pipe_style;

/* might eventually control execution */
typedef enum {
	RES_NONE  = 0,
	RES_IF    = 1,
	RES_THEN  = 2,
	RES_ELIF  = 3,
	RES_ELSE  = 4,
	RES_FI    = 5,
	RES_FOR   = 6,
	RES_WHILE = 7,
	RES_UNTIL = 8,
	RES_DO    = 9,
	RES_DONE  = 10,
	RES_XXXX  = 11,
	RES_IN    = 12,
	RES_SNTX  = 13
} reserved_style;
#define FLAG_END   (1<<RES_NONE)
#define FLAG_IF    (1<<RES_IF)
#define FLAG_THEN  (1<<RES_THEN)
#define FLAG_ELIF  (1<<RES_ELIF)
#define FLAG_ELSE  (1<<RES_ELSE)
#define FLAG_FI    (1<<RES_FI)
#define FLAG_FOR   (1<<RES_FOR)
#define FLAG_WHILE (1<<RES_WHILE)
#define FLAG_UNTIL (1<<RES_UNTIL)
#define FLAG_DO    (1<<RES_DO)
#define FLAG_DONE  (1<<RES_DONE)
#define FLAG_IN    (1<<RES_IN)
#define FLAG_START (1<<RES_XXXX)

/* This holds pointers to the various results of parsing */
struct p_context {
	struct child_prog *child;
	struct pipe *list_head;
	struct pipe *pipe;
	reserved_style w;
	int old_flag;				/* for figuring out valid reserved words */
	struct p_context *stack;
	int type;			/* define type of parser : ";$" common or special symbol */
	/* How about quoting status? */
};

struct child_prog {
	char **argv;				/* program name and arguments */
	/* was quoted when parsed; copy of struct o_string.nonnull field */
	int *argv_nonnull;
	int    argc;                            /* number of program arguments */
	struct pipe *group;			/* if non-NULL, first in group or subshell */
	int sp;				/* number of SPECIAL_VAR_SYMBOL */
	int type;
};

struct pipe {
	int num_progs;				/* total number of programs in job */
	struct child_prog *progs;	/* array of commands in pipe */
	struct pipe *next;			/* to track background commands */
	pipe_style followup;		/* PIPE_BG, PIPE_SEQ, PIPE_OR, PIPE_AND */
	reserved_style r_mode;		/* supports if, for, while, until */
};

struct variables {
	char *name;
	char *value;
	int flg_export;
	int flg_read_only;
	struct variables *next;
};

static unsigned int last_return_code;

/* "globals" within this file */
static char map[256];

static int flag_repeat = 0;
static int do_repeat = 0;
static struct variables *top_vars = NULL ;

#define B_CHUNK (100)
#define B_NOSPAC 1

typedef struct {
	char *data;
	int length;
	int maxlen;
	int quote;
	int nonnull;
} o_string;
#define NULL_O_STRING {NULL,0,0,0,0}
/* used for initialization:
	o_string foo = NULL_O_STRING; */

/* I can almost use ordinary FILE *.  Is open_memstream() universally
 * available?  Where is it documented? */
struct in_str {
	const char *p;
	int __promptme;
	int promptmode;
	int (*get) (struct in_str *);
	int (*peek) (struct in_str *);
};
#define b_getch(input) ((input)->get(input))
#define b_peek(input) ((input)->peek(input))


static void debug_printf(const char *format, ...) { }
#define final_printf debug_printf

static void syntax_err(void) {
	 printf("syntax error\n");
}

/*   o_string manipulation: */
static int b_check_space(o_string *o, int len);
static int b_addchr(o_string *o, int ch);
static void b_reset(o_string *o);
static int b_addqchr(o_string *o, int ch, int quote);

/*  in_str manipulations: */
static void setup_string_in_str(struct in_str *i, const char *s);

/*  "run" the final data structures: */
static char *indenter(int i);

/*   variable assignment: */
static int is_assignment(const char *s)
{
	if (s == NULL)
		return 0;

	if (!isalpha(*s)) return 0;
	++s;
	while(isalnum(*s) || *s=='_') ++s;
	return *s=='=';
}


static void initialize_context(struct p_context *ctx);

static char *lookup_param(char *src);

/*     local variable support */
static char *insert_var_value(char *inp);
static char *insert_var_value_sub(char *inp, int tag_subst);


/*
 * Make new string for parser
 * inp     - array of argument strings to flatten
 * nonnull - indicates argument was quoted when originally parsed
 */
static char *make_string(char **inp, int *nonnull)
{
	char *p;
	char *str = NULL;
	int n;
	int len = 2;
	char *noeval_str;
	int noeval = 0;

	noeval_str = NULL;
	if (noeval_str != NULL && *noeval_str != '0' && *noeval_str != '\0')
		noeval = 1;
	for (n = 0; inp[n]; n++) {
		p = insert_var_value_sub(inp[n], noeval);
		str = xrealloc(str, (len + strlen(p) + (2 * nonnull[n])));
		if (n) {
			strcat(str, " ");
		} else {
			*str = '\0';
		}
		if (nonnull[n])
			strcat(str, "'");
		strcat(str, p);
		if (nonnull[n])
			strcat(str, "'");
		len = strlen(str) + 3;
		if (p != inp[n]) free(p);
	}
	len = strlen(str);
	*(str + len) = '\n';
	*(str + len + 1) = '\0';
	return str;
}

static int handle_dollar(o_string *dest, struct p_context *ctx, struct in_str *input);

static int parse_stream(o_string *dest, struct p_context *ctx, struct in_str *input0, int end_trigger);
/*   setup: */
static int parse_stream_outer(struct in_str *inp, int flag);

static int b_check_space(o_string *o, int len)
{
	/* It would be easy to drop a more restrictive policy
	 * in here, such as setting a maximum string length */
	if (o->length + len > o->maxlen) {
		char *old_data = o->data;
		/* assert (data == NULL || o->maxlen != 0); */
		o->maxlen += max(2*len, B_CHUNK);
		o->data = (char *)realloc(o->data, 1 + o->maxlen);
		if (o->data == NULL) {
			free(old_data);
		}
	}
	return o->data == NULL;
}

static int b_addchr(o_string *o, int ch)
{
	debug_printf("b_addchr: %c %d %p\n", ch, o->length, o);
	if (b_check_space(o, 1)) return B_NOSPAC;
	o->data[o->length] = ch;
	o->length++;
	o->data[o->length] = '\0';
	return 0;
}

static void b_reset(o_string *o)
{
	o->length = 0;
	o->nonnull = 0;
	if (o->data != NULL) *o->data = '\0';
}

static void b_free(o_string *o)
{
	b_reset(o);
	free(o->data);
	o->data = NULL;
	o->maxlen = 0;
}

/* My analysis of quoting semantics tells me that state information
 * is associated with a destination, not a source.
 */
static int b_addqchr(o_string *o, int ch, int quote)
{
	if (quote && strchr("*?[\\",ch)) {
		int rc;
		rc = b_addchr(o, '\\');
		if (rc) return rc;
	}
	return b_addchr(o, ch);
}

static int static_get(struct in_str *i)
{
	int ch = *i->p++;
	if (ch=='\0') return EOF;
	return ch;
}

static int static_peek(struct in_str *i)
{
	return *i->p;
}

static void setup_string_in_str(struct in_str *i, const char *s)
{
	i->peek = static_peek;
	i->get = static_get;
	i->__promptme=1;
	i->promptmode=1;
	i->p = s;
}

static char *insert_var_value(char *inp)
{
	return insert_var_value_sub(inp, 0);
}

static char *insert_var_value_sub(char *inp, int tag_subst)
{
	int res_str_len = 0;
	int len;
	int done = 0;
	char *p, *p1, *res_str = NULL;

	while ((p = (char *)strchr(inp, SPECIAL_VAR_SYMBOL))) {
		/* check the beginning of the string for normal characters */
		if (p != inp) {
			/* copy any characters to the result string */
			len = p - inp;
			res_str = xrealloc(res_str, (res_str_len + len));
			strncpy((res_str + res_str_len), inp, len);
			res_str_len += len;
		}
		inp = ++p;
		/* find the ending marker */
		p = (char *)strchr(inp, SPECIAL_VAR_SYMBOL);
		*p = '\0';
		/* look up the value to substitute */
		if ((p1 = lookup_param(inp))) {
			if (tag_subst)
				len = res_str_len + strlen(p1) + 2;
			else
				len = res_str_len + strlen(p1);
			res_str = xrealloc(res_str, (1 + len));
			if (tag_subst) {
				/*
				 * copy the variable value to the result
				 * string
				 */
				strcpy((res_str + res_str_len + 1), p1);

				/*
				 * mark the replaced text to be accepted as
				 * is
				 */
				res_str[res_str_len] = SUBSTED_VAR_SYMBOL;
				res_str[res_str_len + 1 + strlen(p1)] =
					SUBSTED_VAR_SYMBOL;
			} else
				/*
				 * copy the variable value to the result
				 * string
				 */
				strcpy((res_str + res_str_len), p1);

			res_str_len = len;
		}
		*p = SPECIAL_VAR_SYMBOL;
		inp = ++p;
		done = 1;
	}
	if (done) {
		res_str = (char *)xrealloc(res_str, (1 + res_str_len + strlen(inp)));
		strcpy((res_str + res_str_len), inp);
		while ((p = (char *)strchr(res_str, '\n'))) {
			*p = ' ';
		}
	}
	return (res_str == NULL) ? inp : res_str;
}


static char **make_list_in(char **inp, char *name)
{
	int len, i;
	int name_len = strlen(name);
	int n = 0;
	char **list;
	char *p1, *p2, *p3;

	/* create list of variable values */
	list = (char **)xmalloc(sizeof(*list));
	for (i = 0; inp[i]; i++) {
		p3 = insert_var_value(inp[i]);
		p1 = p3;
		while (*p1) {
			if (*p1 == ' ') {
				p1++;
				continue;
			}
			if ((p2 = (char *)strchr(p1, ' '))) {
				len = p2 - p1;
			} else {
				len = strlen(p1);
				p2 = p1 + len;
			}
			/* we use n + 2 in realloc for list,because we add
			 * new element and then we will add NULL element */
			list = (char **)xrealloc(list, sizeof(*list) * (n + 2));
			list[n] = (char *)xmalloc(2 + name_len + len);
			strcpy(list[n], name);
			strcat(list[n], "=");
			strncat(list[n], p1, len);
			list[n++][name_len + len + 1] = '\0';
			p1 = p2;
		}
		
		if (p3 != inp[i])
			free(p3);
	}
	list[n] = NULL;
	return list;
}

static int run_pipe_real(struct pipe *pi);

static int run_list_real(struct pipe *pi)
{
	char *save_name = NULL;
	char **list = NULL;
	char **save_list = NULL;
	struct pipe *rpipe;
	int flag_rep = 0;

	int rcode=0, flag_skip=1;
	int flag_restore = 0;
	int if_code=0, next_if_code=0;	/* need double-buffer to handle elif */
	reserved_style rmode, skip_more_in_this_rmode=RES_XXXX;
	/* check syntax for "for" */
	for (rpipe = pi; rpipe; rpipe = rpipe->next) {
		if ((rpipe->r_mode == RES_IN ||
			rpipe->r_mode == RES_FOR) &&
			(rpipe->next == NULL)) {
				syntax();
				flag_repeat = 0;
				return 1;
		}
		if ((rpipe->r_mode == RES_IN &&
			(rpipe->next->r_mode == RES_IN &&
			rpipe->next->progs->argv != NULL))||
			(rpipe->r_mode == RES_FOR &&
			rpipe->next->r_mode != RES_IN)) {
				syntax();
				flag_repeat = 0;
				return 1;
		}
	}
	for (; pi; pi = (flag_restore != 0) ? rpipe : pi->next) {
		if (pi->r_mode == RES_WHILE || pi->r_mode == RES_UNTIL ||
			pi->r_mode == RES_FOR) {
				/* check Ctrl-C */
				ctrlc();
				if ((had_ctrlc())) {
					return 1;
				}
				flag_restore = 0;
				if (!rpipe) {
					flag_rep = 0;
					rpipe = pi;
				}
		}
		rmode = pi->r_mode;
		debug_printf("rmode=%d	if_code=%d	next_if_code=%d skip_more=%d\n", rmode, if_code, next_if_code, 
skip_more_in_this_rmode);
		if (rmode == skip_more_in_this_rmode && flag_skip) {
			if (pi->followup == PIPE_SEQ) flag_skip=0;
			continue;
		}
		flag_skip = 1;
		skip_more_in_this_rmode = RES_XXXX;
		if (rmode == RES_THEN || rmode == RES_ELSE) if_code = next_if_code;
		if (rmode == RES_THEN &&  if_code) continue;
		if (rmode == RES_ELSE && !if_code) continue;
		if (rmode == RES_ELIF && !if_code) break;
		if (rmode == RES_FOR && pi->num_progs) {
			if (!list) {
				/* if no variable values after "in" we skip "for" */
				if (!pi->next->progs->argv) continue;
				/* create list of variable values */
				list = make_list_in(pi->next->progs->argv,
					pi->progs->argv[0]);
				save_list = list;
				save_name = pi->progs->argv[0];
				pi->progs->argv[0] = NULL;
				flag_rep = 1;
			}
			if (!(*list)) {
				free(pi->progs->argv[0]);
				free(save_list);
				list = NULL;
				flag_rep = 0;
				pi->progs->argv[0] = save_name;
				continue;
			} else {
				/* insert new value from list for variable */
				if (pi->progs->argv[0])
					free(pi->progs->argv[0]);
				pi->progs->argv[0] = *list++;
			}
		}
		if (rmode == RES_IN) continue;
		if (rmode == RES_DO) {
			if (!flag_rep) continue;
		}
		if (rmode == RES_DONE) {
			if (flag_rep) {
				flag_restore = 1;
			} else {
				rpipe = NULL;
			}
		}
		if (pi->num_progs == 0) continue;
		rcode = run_pipe_real(pi);
		debug_printf("run_pipe_real returned %d\n",rcode);

		if (rcode < -1) {
			last_return_code = -rcode - 2;
			return -2;	/* exit */
		}
		last_return_code=(rcode == 0) ? 0 : 1;

		if ( rmode == RES_IF || rmode == RES_ELIF )
			next_if_code=rcode;  /* can be overwritten a number of times */
		if (rmode == RES_WHILE)
			flag_rep = !last_return_code;
		if (rmode == RES_UNTIL)
			flag_rep = last_return_code;
		if ( (rcode==EXIT_SUCCESS && pi->followup==PIPE_OR) ||
			 (rcode!=EXIT_SUCCESS && pi->followup==PIPE_AND) )
			skip_more_in_this_rmode=rmode;
	}
	return rcode;
}

/* run_pipe_real() starts all the jobs, but doesn't wait for anything
 * to finish.  See checkjobs().
 *
 * return code is normally -1, when the caller has to wait for children
 * to finish to determine the exit status of the pipe.  If the pipe
 * is a simple builtin command, however, the action is done by the
 * time run_pipe_real returns, and the exit code is provided as the
 * return value.
 *
 * The input of the pipe is always stdin, the output is always
 * stdout.  The outpipe[] mechanism in BusyBox-0.48 lash is bogus,
 * because it tries to avoid running the command substitution in
 * subshell, when that is in fact necessary.  The subshell process
 * now has its stdout directed to the input of the appropriate pipe,
 * so this routine is noticeably simpler.
 */
static int run_pipe_real(struct pipe *pi)
{
	int i;
	int nextin;
	int flag = do_repeat ? CMD_FLAG_REPEAT : 0;
	struct child_prog *child;
	char *p;

	nextin = 0;

	/* Check if this is a simple builtin (not part of a pipe).
	 * Builtins within pipes have to fork anyway, and are handled in
	 * pseudo_exec.  "echo foo | read bar" doesn't work on bash, either.
	 */
	if (pi->num_progs == 1) child = & (pi->progs[0]);
		if (pi->num_progs == 1 && child->group) {
		int rcode;
		debug_printf("non-subshell grouping\n");
		rcode = run_list_real(child->group);
		return rcode;
	} else if (pi->num_progs == 1 && pi->progs[0].argv != NULL) {
		for (i=0; is_assignment(child->argv[i]); i++) { /* nothing */ }
		if (i!=0 && child->argv[i]==NULL) {
			/* assignments, but no command: set the local environment */
			for (i=0; child->argv[i]!=NULL; i++) {

				/* Ok, this case is tricky.  We have to decide if this is a
				 * local variable, or an already exported variable.  If it is
				 * already exported, we have to export the new value.  If it is
				 * not exported, we need only set this as a local variable.
				 * This junk is all to decide whether or not to export this
				 * variable. */
				int export_me=0;
				char *name, *value;
				name = (char *)xstrdup(child->argv[i]);
				debug_printf("Local environment set: %s\n", name);
				value = (char *)strchr(name, '=');
				if (value)
					*value=0;
				free(name);
				p = insert_var_value(child->argv[i]);
				if (p != child->argv[i]) 
					free(p);
			}
			return EXIT_SUCCESS;   /* don't worry about errors in set_local_var() yet */
		}
		for (i = 0; is_assignment(child->argv[i]); i++) {
			p = insert_var_value(child->argv[i]);
			if (p != child->argv[i]) {
				child->sp--;
				free(p);
			}
		}
		if (child->sp) {
			char * str = NULL;

			str = make_string(child->argv + i,
					  child->argv_nonnull + i);
			parse_string_outer(str, FLAG_EXIT_FROM_LOOP | FLAG_REPARSING);
			free(str);
			return last_return_code;
		}
		/* check ";", because ,example , argv consist from
		 * "help;flinfo" must not execute
		 */
		if (strchr(child->argv[i], ';')) {
			printf("Unknown command '%s' - try 'help' or use "
					"'run' command\n", child->argv[i]);
			return -1;
		}
		/* Process the command */
		return cmd_process(flag, child->argc, child->argv,
				   &flag_repeat, NULL);
	}
	return -1;
}


/* broken, of course, but OK for testing */
static char *indenter(int i)
{
	static char blanks[]="                                    ";
	return &blanks[sizeof(blanks)-i-1];
}

/* Select which version we will use */
static int run_list(struct pipe *pi)
{
	int rcode=0;
		rcode = run_list_real(pi);
	return rcode;
}

static void initialize_context(struct p_context *ctx)
{
	ctx->pipe=NULL;
	ctx->child=NULL;
	ctx->list_head=NULL;
	ctx->w=RES_NONE;
	ctx->stack=NULL;
	ctx->old_flag=0;
}

/* basically useful version until someone wants to get fancier,
 * see the bash man page under "Parameter Expansion" */
static char *lookup_param(char *src)
{
	char *p;
	char *sep;
	char *default_val = NULL;
	int assign = 0;
	int expand_empty = 0;

	if (!src)
		return NULL;

	sep = (char *)strchr(src, ':');

	if (sep) {
		*sep = '\0';
		if (*(sep + 1) == '-')
			default_val = sep+2;
		if (*(sep + 1) == '=') {
			default_val = sep+2;
			assign = 1;
		}
		if (*(sep + 1) == '+') {
			default_val = sep+2;
			expand_empty = 1;
		}
	}

	if (sep)
		*sep = ':';

	return p;
}

/* return code: 0 for OK, 1 for syntax error */
static int handle_dollar(o_string *dest, struct p_context *ctx, struct in_str *input)
{
	int advance=0;

	int ch = input->peek(input);  /* first character after the $ */
	debug_printf("handle_dollar: ch=%c\n",ch);
	if (isalpha(ch)) {
		b_addchr(dest, SPECIAL_VAR_SYMBOL);
		ctx->child->sp++;
		while(ch=b_peek(input),isalnum(ch) || ch=='_') {
			b_getch(input);
			b_addchr(dest,ch);
		}
		b_addchr(dest, SPECIAL_VAR_SYMBOL);
	} else switch (ch) {
		case '?':
			ctx->child->sp++;
			b_addchr(dest, SPECIAL_VAR_SYMBOL);
			b_addchr(dest, '$');
			b_addchr(dest, '?');
			b_addchr(dest, SPECIAL_VAR_SYMBOL);
			advance = 1;
			break;
		case '{':
			b_addchr(dest, SPECIAL_VAR_SYMBOL);
			ctx->child->sp++;
			b_getch(input);
			/* XXX maybe someone will try to escape the '}' */
			while(ch=b_getch(input),ch!=EOF && ch!='}') {
				b_addchr(dest,ch);
			}
			if (ch != '}') {
				syntax();
				return 1;
			}
			b_addchr(dest, SPECIAL_VAR_SYMBOL);
			break;
		default:
			b_addqchr(dest,'$',dest->quote);
	}
	/* Eat the character if the flag was set.  If the compiler
	 * is smart enough, we could substitute "b_getch(input);"
	 * for all the "advance = 1;" above, and also end up with
	 * a nice size-optimized program.  Hah!  That'll be the day.
	 */
	if (advance) b_getch(input);
	return 0;
}

/* return code is 0 for normal exit, 1 for syntax error */
static int parse_stream(o_string *dest, struct p_context *ctx,
			struct in_str *input, int end_trigger)
{
	unsigned int ch, m;
	int next;

	/* Only double-quote state is handled in the state variable dest->quote.
	 * A single-quote triggers a bypass of the main loop until its mate is
	 * found.  When recursing, quote state is passed in via dest->quote. */

	debug_printf("parse_stream, end_trigger=%d\n",end_trigger);
	while ((ch=b_getch(input))!=EOF) {
		m = map[ch];
		if (input->__promptme == 0) return 1;
		next = (ch == '\n') ? 0 : b_peek(input);

		debug_printf("parse_stream: ch=%c (%d) m=%d quote=%d - %c\n",
			ch >= ' ' ? ch : '.', ch, m,
			dest->quote, ctx->stack == NULL ? '*' : '.');

		if (m==0 || ((m==1 || m==2) && dest->quote)) {
			b_addqchr(dest, ch, dest->quote);
		} else {
			if (m==2) {  /* unquoted IFS */
				//if (done_word(dest, ctx)) {
				if (0) {
					return 1;
				}
			}
			if (ch == end_trigger && !dest->quote && ctx->w==RES_NONE) {
				debug_printf("leaving parse_stream (triggered)\n");
				return 0;
			}
			if (m!=2) switch (ch) {
		case '#':
			if (dest->length == 0 && !dest->quote) {
				while(ch=b_peek(input),ch!=EOF && ch!='\n') { b_getch(input); }
			} else {
				b_addqchr(dest, ch, dest->quote);
			}
			break;
		case '\\':
			if (next == EOF) {
				syntax();
				return 1;
			}
			b_addqchr(dest, '\\', dest->quote);
			b_addqchr(dest, b_getch(input), dest->quote);
			break;
		case '$':
			if (handle_dollar(dest, ctx, input)!=0) return 1;
			break;
		case '\'':
			dest->nonnull = 1;
			while(ch=b_getch(input),ch!=EOF && ch!='\'') {
				if(input->__promptme == 0) return 1;
				b_addchr(dest,ch);
			}
			if (ch==EOF) {
				syntax();
				return 1;
			}
			break;
		case '"':
			dest->nonnull = 1;
			dest->quote = !dest->quote;
			break;
		case ';':
			//done_word(dest, ctx);
			break;
		case '&':
			//done_word(dest, ctx);
			if (next=='&') {
				b_getch(input);
			} else {
				syntax_err();
				return 1;
			}
			break;
		case '|':
			//done_word(dest, ctx);
			if (next=='|') {
				b_getch(input);
			} else {
				/* we could pick up a file descriptor choice here
				 * with redirect_opt_num(), but bash doesn't do it.
				 * "echo foo 2| cat" yields "foo 2". */
				syntax_err();
				return 1;
			}
			break;
		case SUBSTED_VAR_SYMBOL:
			dest->nonnull = 1;
			while (ch = b_getch(input), ch != EOF &&
			    ch != SUBSTED_VAR_SYMBOL) {
				debug_printf("subst, pass=%d\n", ch);
				if (input->__promptme == 0)
					return 1;
				b_addchr(dest, ch);
			}
			debug_printf("subst, term=%d\n", ch);
			if (ch == EOF) {
				syntax();
				return 1;
			}
			break;
		default:
			syntax();   /* this is really an internal logic error */
			return 1;
			}
		}
	}
	/* complain if quote?  No, maybe we just finished a command substitution
	 * that was quoted.  Example:
	 * $ echo "`cat foo` plus more"
	 * and we just got the EOF generated by the subshell that ran "cat foo"
	 * The only real complaint is if we got an EOF when end_trigger != '\0',
	 * that is, we were really supposed to get end_trigger, and never got
	 * one before the EOF.  Can't use the standard "syntax error" return code,
	 * so that parse_stream_outer can distinguish the EOF and exit smoothly. */
	debug_printf("leaving parse_stream (EOF)\n");
	if (end_trigger != '\0') return -1;
	return 0;
}

static void mapset(const unsigned char *set, int code)
{
	const unsigned char *s;
	for (s=set; *s; s++) map[*s] = code;
}

/* most recursion does not come through here, the exeception is
 * from builtin_source() */
static int parse_stream_outer(struct in_str *inp, int flag)
{

	struct p_context ctx;
	o_string temp=NULL_O_STRING;
	int rcode;
	int code = 1;
	do {
		ctx.type = flag;
		initialize_context(&ctx);
		if (!(flag & FLAG_PARSE_SEMICOLON) || (flag & FLAG_REPARSING)) 
			mapset((unsigned char *)";$&|", 0);
		inp->promptmode=1;
		rcode = parse_stream(&temp, &ctx, inp,
				     flag & FLAG_CONT_ON_NEWLINE ? -1 : '\n');
		if (rcode == 1) flag_repeat = 0;
		if (rcode != 1 && ctx.old_flag != 0) {
			syntax();
			flag_repeat = 0;
		}
		if (rcode != 1 && ctx.old_flag == 0) {
			code = run_list(ctx.list_head);
			if (code == -2) {	/* exit */
				b_free(&temp);
				code = 0;
				break;
			}
			if (code == -1)
			    flag_repeat = 0;
		} else {
			if (ctx.old_flag != 0) {
				free(ctx.stack);
				b_reset(&temp);
			}
			if (inp->__promptme == 0) printf("<INTERRUPT>\n");
			inp->__promptme = 1;
			temp.nonnull = 0;
			temp.quote = 0;
			inp->p = NULL;
		}
		b_free(&temp);
	/* loop on syntax errors, return on EOF */
	} while (rcode != -1 && !(flag & FLAG_EXIT_FROM_LOOP) &&
		(inp->peek != static_peek || b_peek(inp)));
	return (code != 0) ? 1 : 0;
}

int parse_string_outer(const char *s, int flag)
{
	struct in_str input;
	char *p = NULL;
	int rcode;
	if (!s)
		return 1;
	if (!*s)
		return 0;
	if (!(p = (char *)strchr(s, '\n')) || *++p) {
		p = (char *)malloc(strlen(s) + 2);
		strcpy(p, s);
		strcat(p, "\n");
		setup_string_in_str(&input, p);
		rcode = parse_stream_outer(&input, flag);
		free(p);
		return rcode;
	} else {
	setup_string_in_str(&input, s);
	return parse_stream_outer(&input, flag);
	}
}


