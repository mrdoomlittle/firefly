# include "dus.h"
# include "../string.h"
# include "../ffly_def.h"
# include "../malloc.h"
# include "../stdio.h"
struct hash env;
void ff_dus_init(void) {
	hash_init(&env);
}

ff_u8_t ff_dus_next_token_is(ff_u8_t __sort, ff_u8_t __val) {
	tokenp tok = ff_dus_nexttok();
	if (!tok)
		return 0;
	ff_u8_t res = (tok->sort == __sort && tok->val == __val);
	if (!res)
		ff_dus_ulex(tok);
	return res;
}

ff_u8_t ff_dus_expect_token(ff_u8_t __sort, ff_u8_t __val) {
	tokenp tok = ff_dus_nexttok();
	ff_u8_t res = (tok->sort == __sort && tok->val == __val);
	return res;
}

# define to_keyword(__tok, __val) \
	__tok->sort = _tok_keywd;\
	__tok->val = __val

tokenp ff_dus_nexttok(void) {
	tokenp tok = ff_dus_lex();
	if (!tok) return NULL;
	if (tok->sort == _tok_ident) {
		if (!strcmp(tok->p, "out")) {
			to_keyword(tok, _keywd_out);
		} else if (!strcmp(tok->p, "cas")) {
			to_keyword(tok, _keywd_cas);
		} else if (!strcmp(tok->p, "syput")) {
			to_keyword(tok, _keywd_syput);
		} else if (!strcmp(tok->p, "shell")) {
			to_keyword(tok, _keywd_shell);
		}
	}
	return tok;
}

void *tf[100];
void **fresh = tf;
void to_free(void *__p) {
	if (fresh-tf >= 100) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

nodep ff_dus_node_alloc(void) {
	nodep ret;
	ret = (nodep)malloc(sizeof(struct node));
	to_free(ret);
	return ret;
}

objp ff_dus_obj_alloc(void) {
	objp ret;
	ret = (objp)malloc(sizeof(struct obj));
	to_free(ret);
	return ret;
}

void ff_dus_cleanup(void) {
	hash_destroy(&env);
	void **cur = tf;
	while(cur != fresh)
		free(*(cur++));
	ff_dus_lexer_cleanup();
}
