# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
void static
parser_decl(nodep *__node) {
	tokenp name = ff_dus_nexttok();

	nodep init = NULL, var;
	nodep nod = (*__node = new_node);
	if (!ff_dus_next_token_is(_tok_keywd, _eq)) {
		goto _sk;
	}

	init = ff_dus_exp();
	if (!init) {
		// error
		while(1);
	}
//	printf("decl, %s\n", name->p);
_sk:
	var = new_node;
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

void static
parser_syput(nodep *__node) {
	nodep nod = (*__node = new_node);
	nod->kind = _syput;
	tokenp name = ff_dus_nexttok();
	nod->name = name->p;
	nod->len = name->l;
	nod->p = ff_dus_exp();
}

void static
parser_shell(nodep *__node) {
	nodep nod = (*__node = new_node);
	nod->kind = _shell;
	nod->p = ff_dus_exp();
}

void static
parser_set(nodep *__node) {
	nodep nod = (*__node = new_node);
	nod->kind = _set;
	tokenp l;
	l = ff_dus_nexttok();
	ff_dus_nexttok();

	nod->l = (nodep)hash_get(&env, l->p, l->l);
	nod->r = ff_dus_exp();
}

void ff_dus_parse(nodep *__top) {
	nodep p, end = NULL;
	while(!at_eof()) {
		tokenp tok;
		tok = ff_dus_nexttok();
		p = NULL;
		if (!tok) goto _end;
		if (tok->sort == _tok_ident) {
			ff_dus_ulex(tok);
			if (hash_get(&env, tok->p, tok->l) != NULL) {	
				p = ff_dus_exp();
			} else {
				parser_decl(&p);
			}
		} else if (tok->sort == _tok_keywd) {
			if (tok->val == _keywd_out) {
				parser_out(&p);
			} else if (tok->val == _keywd_syput) {
				parser_syput(&p);
			} else if (tok->val == _keywd_shell) {
				parser_shell(&p);
			} else if (tok->val == _keywd_set) {
				parser_set(&p);
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
