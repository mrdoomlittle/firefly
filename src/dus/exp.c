# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../system/string.h"
# include "../string.h"
void static
exp_assign(nodep *__expr) {
	if (ff_dus_next_token_is(_tok_keywd, _eq)) {
		nodep exp = new_node;
		exp->r = ff_dus_exp();
		exp->l = *__expr;
		if (!exp->l || !exp->r) {
			printf("error.\n");
		}
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
exp_cas(nodep *__expr) {
	if (ff_dus_next_token_is(_tok_keywd, _keywd_cas)) {
		nodep exp = new_node;
		exp->p = ff_dus_exp();
		exp->kind = _cas;
		*__expr = exp;
	}
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
	//		char c = *(char*)tok->p;
	//		if () {
	//			exp = exp_int((char*)tok->p);
			if (tok->sort == _tok_str) {
				exp = new_node;
				exp->kind = _str;
				exp->p = (char*)tok->p;
				exp->len = tok->l+1;
			}
		} else {
			if (tok->sort == _tok_keywd && tok->val == _dollar) {
				tokenp n = ff_dus_nexttok();	
				exp = new_node;
				exp->kind = _str;
				exp->p = hash_get(&env, n->p, 1);
				exp->len = strlen(exp->p)+1;
			} else {
				ff_dus_ulex(tok);
				return;
			}
		}
	}
	*__expr = exp;
}

nodep ff_dus_exp(void) {
	nodep exp = NULL;
	exp_primary(&exp);
	exp_cas(&exp);
	exp_assign(&exp);
	return exp;
}
