# ifndef __ffly__dus__h
# define __ffly__dus__h
# include "../ffint.h"
# define new_node ff_dus_node_alloc()
# define incrp p++
extern ff_u8_t *p;
void ff_dus_init(void);
void ff_dus_cleanup(void);
ff_u8_t at_eof(void);

typedef struct obj {
	struct obj *next;
    ff_u8_t op;
	ff_uint_t size, len;
	struct obj *to, *dst, *src;
	char *name;
	void *p;
} *objp;

typedef struct token {
	struct token *next;
	ff_u8_t sort, val;
	void *p;
	ff_uint_t l;
} *tokenp;

typedef struct node {
	struct node *next;
	ff_u64_t intval;
	void *p;
	ff_uint_t len;
	ff_u8_t kind;
	struct node *var, *init;
	struct node *l, *r;
	struct node *src;
	char *name;
	objp _obj;
} *nodep;

struct hash_entry {
	struct hash_entry *fd, *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

struct hash {
	struct hash_entry **table;
	struct hash_entry *top;
};

struct frag {
	struct frag *next;
	void *p;
	ff_uint_t len;
};

void hash_init(struct hash*);
void hash_destroy(struct hash*);
void hash_put(struct hash*, ff_u8_t const*, ff_uint_t, void*);
void* hash_get(struct hash*, ff_u8_t const*, ff_uint_t);

extern struct hash env;

tokenp ff_dus_nexttok(void);

ff_u8_t ff_dus_next_token_is(ff_u8_t, ff_u8_t);
ff_u8_t ff_dus_expect_token(ff_u8_t, ff_u8_t);
void ff_dus_lexer_cleanup(void);
tokenp peektok(void);
void ff_dus_parse(nodep*);
objp ff_dus_gen(nodep);
void ff_dus_run(objp);

nodep ff_dus_exp(void);
void ff_dus_ulex(tokenp);
tokenp ff_dus_lex(void);

enum {
	_op_copy,
	_op_assign,
	_op_push,
	_op_pop,
	_op_fresh,
	_op_free,
	_op_out,
	_op_cas,
	_op_syput,
	_op_shell
};

enum {
	_decl,
	_var,
	_str,
	_int,
	_assign,
	_out,
	_cas,
	_syput,
	_shell
};

enum {
	_eq,
	_keywd_out,
	_keywd_cas,
	_keywd_syput,
	_keywd_shell
};

enum {
	_tok_ident,
	_tok_keywd,
	_tok_str
};

nodep ff_dus_node_alloc(void);
objp ff_dus_obj_alloc(void);
void to_free(void*);
# endif /*__ffly__dus__h*/
