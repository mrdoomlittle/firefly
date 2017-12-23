# ifndef __ffly__script__h
# define __ffly__script__h
# include "system/vec.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/buff.h"
struct ffly_script {
	ffly_byte_t *p, *end;
	ffly_off_t off;
	struct ffly_vec nodes;
	struct ffly_vec toks;
	struct ffly_buff sbuf;
	// token injection buffer
	struct ffly_buff iject_buff;
	void *top;
};

# ifdef __ffly_script_internal
# define TOK_IDENT 0
# define TOK_KEYWORD 1
# define TOK_NO 2
enum {
	_eq,
	_semicolon,
	_l_paren,
	_r_paren,
	_k_print,
	_k_uint_t,
	_k_int_t,
	_k_u64_t,
	_k_i64_t,
	_k_u32_t,
	_k_i32_t,
	_k_u16_t,
	_k_i16_t,
	_k_u8_t,
	_k_i8_t
};

enum {
	_ast_decl,
	_ast_assign,
	_ast_print,
	_ast_literal,
	_ast_print_call
};

enum {
	_uint_t,
	_int_t,
	_u64_t,
	_i64_t,
	_u32_t,
	_i32_t,
	_u16_t,
	_i16_t,
	_u8_t,
	_i8_t
};

enum {
	_op_alloc,
	_op_free,
	_op_assign,
	_op_copy,
	_op_print,
};

struct node {
	mdl_u8_t kind;
	struct type *_type;
	ffly_byte_t val[sizeof(mdl_u64_t)];
	struct node *init, *arg;
};

struct token {
	void *p;
	mdl_u8_t kind, id;
};

struct type {
	mdl_u8_t kind;
	mdl_uint_t size;
};

struct obj {
	mdl_u8_t opcode;
	void *p;
	struct type *_type;
	struct obj *to, *from;
	struct obj *val;
	struct obj *next;
};

ffly_bool_t next_token_is(struct ffly_script*, mdl_u8_t, mdl_u8_t);
ffly_bool_t is_keyword(struct token*, mdl_u8_t);
ffly_bool_t expect_token(struct ffly_script*, mdl_u8_t, mdl_u8_t);
struct token* next_token(struct ffly_script*);
struct token* peek_token(struct ffly_script*);
struct token* ffly_script_lex(struct ffly_script*, ffly_err_t*);
void ffly_script_ulex(struct ffly_script*, struct token*);
ffly_bool_t maybe_keyword(struct token*);
ffly_bool_t is_eof(struct ffly_script*);
void pr_tok(struct token*);
# endif
ffly_err_t ffly_script_prepare(struct ffly_script*);
ffly_err_t ffly_script_ld(struct ffly_script*, char*);
ffly_err_t ffly_script_parse(struct ffly_script*);
ffly_err_t ffly_script_gen(struct ffly_script*);
ffly_err_t ffly_script_exec(struct ffly_script*);
ffly_err_t ffly_script_free(struct ffly_script*);
# endif /*__ffly__script__h*/
