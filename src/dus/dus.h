# ifndef __ffly__dus__h
# define __ffly__dus__h
# include "../ffint.h"
# define new_node ff_dus_node_alloc()
# define incrp dus_p++
extern ff_u8_t *dus_p;
void ff_dus_init(void);
void ff_dus_cleanup(void);
ff_u8_t dus_at_eof(void);

/*
	cluster of shit
*/
struct dus_clus_s {
	void(*init)(void);
};

extern struct dus_clus_s dus_clus;

typedef struct dus_obj {
	struct dus_obj *next;
    ff_u8_t op;
	ff_uint_t size, len;
	// change to void
	struct dus_obj *to, *dst, *src;
	struct dus_obj *objpp;
	char *name;
	void *p;
} *dus_objp;

typedef struct dus_token {
	struct dus_token *next;
	ff_u8_t sort, val;
	void *p;
	ff_uint_t l;
} *dus_tokenp;

typedef struct dus_node {
	struct dus_node *next;
	ff_u64_t intval;
	void *p;
	ff_uint_t len;
	ff_u8_t kind;
	struct dus_node *var, *init;
	struct dus_node *l, *r;
	struct dus_node *src;
	char *name;
	dus_objp _obj;
} *dus_nodep;

struct dus_hash_entry {
	struct dus_hash_entry *fd, *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

struct dus_hash {
	struct dus_hash_entry **table;
	struct dus_hash_entry *top;
};

/*
	string fragment

	% used for args

	% = new fragment
	hello%test

*/
struct dus_frag {
	struct dus_frag *next;
	void *p;
	ff_uint_t len;
};

void dus_hash_init(struct dus_hash*);
void dus_hash_destroy(struct dus_hash*);
void dus_hash_put(struct dus_hash*, ff_u8_t const*, ff_uint_t, void*);
void* dus_hash_get(struct dus_hash*, ff_u8_t const*, ff_uint_t);

extern struct dus_hash dus_env;

dus_tokenp ff_dus_nexttok(void);

ff_u8_t ff_dus_next_token_is(ff_u8_t, ff_u8_t);
ff_u8_t ff_dus_expect_token(ff_u8_t, ff_u8_t);
void ff_dus_lexer_cleanup(void);
dus_tokenp peektok(void);
void ff_dus_parse(dus_nodep*);
dus_objp ff_dus_gen(dus_nodep);
void ff_dus_run(dus_objp);

dus_nodep ff_dus_exp(void);
void ff_dus_ulex(dus_tokenp);
dus_tokenp ff_dus_lex(void);

enum {
	_dus_op_copy,
	_dus_op_assign,
	_dus_op_push,
	_dus_op_pop,
	_dus_op_fresh,
	_dus_op_free,
	_dus_op_out,
	_dus_op_cas,
	_dus_op_syput,
	_dus_op_shell,
	_dus_op_set
};

enum {
	_dus_decl,
	_dus_var,
	_dus_str,
	_dus_int,
	_dus_assign,
	_dus_out,
	_dus_cas,
	_dus_syput,
	_dus_shell,
	_dus_set
};

enum {
	_dus_eq,
	_dus_dollar,
	_dus_keywd_out,
	_dus_keywd_cas,
	_dus_keywd_syput,
	_dus_keywd_shell,
	_dus_keywd_set
};

enum {
	_dus_tok_ident,
	_dus_tok_keywd,
	_dus_tok_str,
	_dus_tok_dec
};

dus_nodep ff_dus_node_alloc(void);
dus_objp ff_dus_obj_alloc(void);
void dus_to_free(void*);
# endif /*__ffly__dus__h*/
