# include "dus.h"
# include "../string.h"
# include "../ffly_def.h"
# include "../malloc.h"
# include "../stdio.h"
struct dus_hash dus_env;

void ff_dus_init(void);
struct dus_clus_s dus_clus = {
	.init = ff_dus_init
};

void ff_dus_init(void) {
	dus_hash_init(&dus_env);
}

ff_u8_t
ff_dus_next_token_is(ff_u8_t __sort, ff_u8_t __val) {
	dus_tokenp tok;
	ff_u8_t res;
	if (!(tok = ff_dus_nexttok()))
		return 0;

	res = (tok->sort == __sort && tok->val == __val);
	if (!res)
		ff_dus_ulex(tok);
	return res;
}

ff_u8_t
ff_dus_expect_token(ff_u8_t __sort, ff_u8_t __val) {
	dus_tokenp tok;
	ff_u8_t res;
	if (!(tok = ff_dus_nexttok()))
		return 0;
	res = (tok->sort == __sort && tok->val == __val);
	return res;
}

#define to_keyword(__tok, __val) \
	__tok->sort = _dus_tok_keywd;\
	__tok->val = __val

dus_tokenp
ff_dus_nexttok(void) {
	dus_tokenp tok;
	if (!(tok = ff_dus_lex())) return NULL;
	if (tok->sort == _dus_tok_ident) {
		/*
			NOTE:
				dont remove {} from if statments as 
				'to_keyword' is a macro
		*/
		if (!strcmp(tok->p, "out")) {
			to_keyword(tok, _dus_keywd_out);
		} else if (!strcmp(tok->p, "cas")) {
			to_keyword(tok, _dus_keywd_cas);
		} else if (!strcmp(tok->p, "syput")) {
			to_keyword(tok, _dus_keywd_syput);
		} else if (!strcmp(tok->p, "shell")) {
			to_keyword(tok, _dus_keywd_shell);
		} else if (!strcmp(tok->p, "set")) {
			to_keyword(tok, _dus_keywd_set);
		}
	}
	return tok;
}

/*
	FOR NOW! may not exist in the future
*/
static void *tf[2048];
static void **fresh = tf;
void dus_to_free(void *__p) {
	if (fresh-tf >= 2048) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

dus_nodep ff_dus_node_alloc(void) {
	dus_nodep ret;
	ret = (dus_nodep)malloc(sizeof(struct dus_node));
	dus_to_free(ret);
	return ret;
}

dus_objp ff_dus_obj_alloc(void) {
	dus_objp ret;
	ret = (dus_objp)malloc(sizeof(struct dus_obj));
	dus_to_free(ret);
	return ret;
}

# include "mm.h"
void ff_dus_cleanup(void) {
	dus_hash_destroy(&dus_env);
	void **cur = tf;
	while(cur != fresh)
		free(*(cur++));
	ff_dus_lexer_cleanup();
	ff_dus_mm_cleanup();
}
