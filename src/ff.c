# include "ffint.h"
# include "types.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_dup.h"
# include "dep/str_len.h"
# include "dep/mem_cpy.h"
# include "shell.h"
static struct hash cmdmap;

struct cmd {
	char const *ident;
	ff_uint_t func;
};

static struct cmd *cmd_tbl[] = {
	&(struct cmd){"help", _cmd_help},
	&(struct cmd){"exit", _cmd_exit},
	&(struct cmd){"info", _cmd_info},
	&(struct cmd){"ff5", _cmd_ff5},
	&(struct cmd){"ff6", _cmd_ff6},
	&(struct cmd){"exec", _cmd_exec},
	NULL
};

static void
ldcmds(void) {
	struct cmd **cur = cmd_tbl, *c;
	while((c = *cur) != NULL) {
		ffsh_cmdput(&cmdmap, c->ident, c->func);
		cur++;
	}
}

static char line[512];
ff_uint_t static off = 0;
ff_uint_t static e;

void static
_get(ff_uint_t __n, ff_uint_t __offset, void *__buf) {
	ffly_mem_cpy((ff_u8_t*)__buf+__offset, line+off, __n);	
	off+=__n;
}

ff_u8_t static
_at_eof(void) {
	return off>=e;
}

ff_i8_t static run = 0;
# include "linux/termios.h"
/*
	can break needs fixing and shit.
*/

# include "line.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffsh_run = &run;
	struct termios term, old;
	tcgetattr(ffly_in->fd, &term);
	old = term;
	term.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(ffly_in->fd, &term);

	get = _get;
	at_eof = _at_eof;
	ff_err_t err;
	hash_init(&cmdmap);
	ldcmds();
	struct node *n;
	char *p;
	char buf[26];
_again:
	p = line;
	/*
		will be moved later just piecing it together
	*/
	while(1) {
		ffly_l_show(ffly_out);
		ff_uint_t n, i = 0;
		n = read(ffly_in->fd, buf, sizeof(buf));
		char c;
		while(i != n) {
			c = *(buf+i);
			if (c == '\n') goto _out;
			if (c == 27) {
				i++;
				i++;
				if (*(buf+2) == 'D') {
					ffly_l_backward;
				} else if (*(buf+2) == 'C') {
					ffly_l_forward;
				}
			} else if (c == 127) {
				ffly_l_del();
			} else if (c>=32 && c<=126) {
				ffly_l_put(c);
			}
			i++;
		}
	}
_out:
	p+=ffly_l_load(line);
	ffly_l_reset();
	*p = '\0';
	ffly_printf("\n: line: %s\n", line);
	ffly_fdrain(ffly_out);
	e = p-line;
	off = 0;

	n = ffsh_parse();
	if (n != NULL) {
		if (n->kind == _cmd) {
			ffly_printf("command: %s\n", n->name);
			void *p = ffsh_cmdget(&cmdmap, n->name);
			if (!p) {
				ffly_printf("command does not exist.\n");
			} else
				ffsh_exec_cmd(p, n->argc, (struct arg_s**)n->args);
		}
	}
	mem_cleanup();
	if (!run)
		goto _again;

	hash_destroy(&cmdmap);
	if (tcsetattr(ffly_in->fd, &old) == -1) {
		ffly_printf("failed to set tc attr.\n");
	}
}
