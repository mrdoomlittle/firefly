# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# define new_node (nodep)malloc(sizeof(struct node))

void static
parser_decl(nodep *__node) {
	tokenp name = ff_dus_nexttok();

	nodep nod = (*__node = new_node);
	if (!ff_dus_expect_token(_tok_keywd, _eq)) {
		// error
		printf("error expected.\n");
	}

	nodep init;
	init = ff_dus_exp();
	if (!init) {
		// error
		while(1);
	}
	printf("decl, %s = %s, %u\n", name->p, init->p, init->len);

	nodep var = new_node;
	var->kind = _var;
	hash_put(&env, name->p, name->l, var);
	nod->init = init;
	nod->var = var;
	nod->kind = _decl;
}

void static
parser_out(nodep *__node) {
	nodep nod = (*__node = new_node);

	nod->kind = _out;
	nod->p = ff_dus_exp();
}

void ff_dus_parse(nodep *__top) {
	nodep p, end = NULL;
	while(!at_eof()) {
		tokenp tok;
		tok = ff_dus_nexttok();
		p = NULL;
		if (!tok) goto _end;
		if (tok->sort == _tok_ident) {
			if (hash_get(&env, tok->p, tok->l) != NULL) {	
				p = ff_dus_exp();
			} else {
				ff_dus_ulex(tok);
				parser_decl(&p);
			}
		} else if (tok->sort == _tok_keywd) {
			if (tok->val == _keywd_out) {
				parser_out(&p);
			}
		}

		if (p != NULL) {
			if (!*__top)
				*__top = p;
			if (end != NULL)
				end->next = p;
			end = p;
		}
	}
_end:
	if (end != NULL)
		end->next = NULL;
}
