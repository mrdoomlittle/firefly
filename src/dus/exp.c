# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../system/string.h"
# define new_node (nodep)malloc(sizeof(struct node))

void static
exp_assign(nodep *__expr) {
	if (ff_dus_next_token_is(_tok_keywd, _eq)) {
		nodep exp = new_node;
		exp->r = ff_dus_exp();
		exp->l = *__expr;
		exp->kind = _assign;
		*__expr = exp;
	}
}

ff_u64_t static
read_int(char *__s) {
	return ffly_stno(__s);
}

nodep static
exp_int(char *__s) {
	nodep exp = new_node;
	exp->kind = _int;
	exp->intval = read_int(__s);
	return exp;
}

void static
exp_primary(nodep *__expr) {
	tokenp tok = ff_dus_nexttok();
	if (!tok) {
		printf("null token.\n");
		return;
	}
	nodep exp;
	if (tok->sort == _tok_ident) {
		exp = hash_get(&env, tok->p, tok->l);
	} else {
		if (tok->sort == _tok_str){// || tok->sort == _dec) {
			char c = *(char*)tok->p;
			if (c>=0 && c<='9') {
				exp = exp_int((char*)tok->p);
			} else if (c>='a' && c<='z') {
				exp = new_node;
				exp->kind = _str;
				exp->p = (char*)tok->p;
				exp->len = tok->l+1;
			}
		} else {
			ff_dus_ulex(tok);
			return;
		}
	}
	*__expr = exp;
}

nodep ff_dus_exp(void) {
	nodep exp = NULL;
	exp_primary(&exp);
	exp_assign(&exp);
	return exp;
}
