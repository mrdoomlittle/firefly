# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
void static
parser_decl(dus_nodep *__node) {
	dus_tokenp name = ff_dus_nexttok();

	dus_nodep init = NULL, var;
	dus_nodep nod = (*__node = new_node);
	if (!ff_dus_next_token_is(_dus_tok_keywd, _dus_eq)) {
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
	var->kind = _dus_var;
	dus_hash_put(&dus_env, name->p, name->l, var);
	nod->init = init;
	nod->var = var;
	nod->kind = _dus_decl;
}

void static
parser_out(dus_nodep *__node) {
	dus_nodep nod = (*__node = new_node);

	nod->kind = _dus_out;
	nod->p = ff_dus_exp();
}

void static
parser_syput(dus_nodep *__node) {
	dus_nodep nod = (*__node = new_node);
	nod->kind = _dus_syput;
	dus_tokenp name = ff_dus_nexttok();
	nod->name = name->p;
	nod->len = name->l;
	nod->p = ff_dus_exp();
}

void static
parser_shell(dus_nodep *__node) {
	dus_nodep nod = (*__node = new_node);
	nod->kind = _dus_shell;
	nod->p = ff_dus_exp();
}

void static
parser_set(dus_nodep *__node) {
	dus_nodep nod = (*__node = new_node);
	nod->kind = _dus_set;
	dus_tokenp l;
	l = ff_dus_nexttok();
	ff_dus_nexttok();

	nod->l = (dus_nodep)dus_hash_get(&dus_env, l->p, l->l);
	nod->r = ff_dus_exp();
}

void ff_dus_parse(dus_nodep *__top) {
	dus_nodep p, end = NULL;
	while(!dus_at_eof()) {
		dus_tokenp tok;
		tok = ff_dus_nexttok();
		p = NULL;
		if (!tok) goto _end;
		if (tok->sort == _dus_tok_ident) {
			ff_dus_ulex(tok);
			if (dus_hash_get(&dus_env, tok->p, tok->l) != NULL) {	
				p = ff_dus_exp();
			} else {
				parser_decl(&p);
			}
		} else if (tok->sort == _dus_tok_keywd) {
			if (tok->val == _dus_keywd_out) {
				parser_out(&p);
			} else if (tok->val == _dus_keywd_syput) {
				parser_syput(&p);
			} else if (tok->val == _dus_keywd_shell) {
				parser_shell(&p);
			} else if (tok->val == _dus_keywd_set) {
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
