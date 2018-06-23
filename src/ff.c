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
	&(struct cmd){"ff6", _cmd_ff6},
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

ff_i8_t ffsh_run = 0;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	get = _get;
	at_eof = _at_eof;
	ff_err_t err;
	hash_init(&cmdmap);
	ldcmds();
	struct node *n;
_again:
	ffly_printf("~: ");
	ffly_fdrain(ffly_out);
	e = ffly_rdline(line, 512, ffly_in);
	off = 0;

	n = ffsh_parse();
	if (n != NULL) {
		if (n->kind == _cmd) {
			ffly_printf("command: %s\n", n->name);
			ffsh_exec_cmd(ffsh_cmdget(&cmdmap, n->name), n->argc, (struct arg_s**)n->args);
		}
	}
	mem_cleanup();
	if (!ffsh_run)
		goto _again;

	hash_destroy(&cmdmap);
}
