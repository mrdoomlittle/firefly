# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../system/string.h"
# include "../string.h"
void static
exp_assign(dus_nodep *__expr) {
	if (ff_dus_next_token_is(_dus_tok_keywd, _dus_eq)) {
		dus_nodep exp = new_node;
		exp->r = ff_dus_exp();
		exp->l = *__expr;
		if (!exp->l || !exp->r) {
			printf("error.\n");
		}
		exp->kind = _dus_assign;
		*__expr = exp;
	}
}

ff_u64_t static
read_int(char *__s) {
	return ffly_stno(__s);
}

dus_nodep static
exp_int(char *__s) {
	dus_nodep exp = new_node;
	exp->kind = _dus_int;
	exp->intval = read_int(__s);
	return exp;
}

void static
exp_cas(dus_nodep *__expr) {
	if (ff_dus_next_token_is(_dus_tok_keywd, _dus_keywd_cas)) {
		dus_nodep exp = new_node;
		exp->p = ff_dus_exp();
		exp->kind = _dus_cas;
		*__expr = exp;
	}
}

void static
exp_primary(dus_nodep *__expr) {
	dus_tokenp tok = ff_dus_nexttok();
	if (!tok) {
		printf("null token.\n");
		return;
	}
	dus_nodep exp;
	if (tok->sort == _dus_tok_ident) {
		exp = dus_hash_get(&dus_env, tok->p, tok->l);
	} else {
		if (tok->sort == _dus_tok_str){// || tok->sort == _dec) {
	//		char c = *(char*)tok->p;
	//		if () {
	//			exp = exp_int((char*)tok->p);
			if (tok->sort == _dus_tok_str) {
				exp = new_node;
				exp->kind = _dus_str;
				exp->p = (char*)tok->p;
				exp->len = tok->l+1;
			}
		} else {
			if (tok->sort == _dus_tok_keywd && tok->val == _dus_dollar) {
				dus_tokenp n = ff_dus_nexttok();	
				exp = new_node;
				exp->kind = _dus_str;
				exp->p = dus_hash_get(&dus_env, n->p, 1);
				exp->len = strlen(exp->p)+1;
			} else {
				ff_dus_ulex(tok);
				return;
			}
		}
	}
	*__expr = exp;
}

dus_nodep ff_dus_exp(void) {
	dus_nodep exp = NULL;
	exp_primary(&exp);
	exp_cas(&exp);
	exp_assign(&exp);
	return exp;
}
