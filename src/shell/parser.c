# include "parser.h"
# include "../ffly_def.h"
# include "lexer.h"
# include "../string.h"
# include "memalloc.h"
# include "../stdio.h"
# include "exec.h"
nodep static n;
void static
parser_cmd(void) {
	ff_u8_t ident;
	ident = nexttok();

	n = (nodep)mem_alloc(sizeof(struct node));
	n->name = tok_data;
	n->nl = tok_sz;
	n->kind = _cmd;

	void **arg;

	ff_u8_t tok;
	arg = n->args;
	for(;;) {
		tok = nexttok();
		if (tok == _tok_nl || tok == _tok_eof) break;

		struct arg_s *a = (struct arg_s*)mem_alloc(sizeof(struct arg_s));
		*(arg++) = a;
		printf("arg: %s\n", tok_data);
		a->p = tok_data;
		a->l = tok_sz;
	}
	*arg = NULL;

	n->argc = arg-n->args;
}

nodep
ffsh_parse(void) {
	ff_u8_t tok;
	tok = peektok();
	printf("tok: %s\n", tokstr(tok));
	if (tok == _tok_ident) {
		parser_cmd();
	}
	if (tok == _tok_eof)
		return NULL;
	return n;
}
