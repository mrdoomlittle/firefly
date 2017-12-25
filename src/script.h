# ifndef __ffly__script__h
# define __ffly__script__h
# include "system/vec.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/buff.h"
# include "system/map.h"
struct ffly_script {
    struct ffly_map env;
	ffly_byte_t *p, *end;
	ffly_off_t off;
    mdl_uint_t line;
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
    _eqeq,
	_eq,
    _neq,
    _gt,
    _lt,
	_semicolon,
	_l_paren,
	_r_paren,
    _l_brace,
    _r_brace,
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
	_k_i8_t,
    _k_if
};

enum {
    _ast_if,
	_ast_decl,
	_ast_assign,
	_ast_print,
	_ast_literal,
	_ast_print_call,
    _ast_var,
    _op_eq,
    _op_neq,
    _op_gt,
    _op_lt
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

# define _flg_neq 0x1
# define _flg_eq 0x2
# define _flg_gt 0x4
# define _flg_lt 0x8

enum {
	_op_alloc,
	_op_free,
	_op_assign,
	_op_copy,
	_op_print,
    _op_compare,
    _op_jump,
    _op_cond_jump
};

struct node {
    mdl_u8_t kind;
    struct type *_type;
    ffly_byte_t val[sizeof(mdl_u64_t)];
    struct node *init, *var, *arg;
    struct obj *_obj;
    struct node *l, *r;
    struct node *cond;
    struct ffly_vec vec;
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
    mdl_u32_t off;
    mdl_u8_t opcode, cond;
    void *p;
    struct type *_type;
    // dst/src? or'
    struct obj *to, *from, *l, *r;
    struct obj *val, **jmp, *flags;
    struct obj *next;
};

void skip_token(struct ffly_script*);
mdl_uint_t curl(struct ffly_script*);
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
ffly_err_t ffly_script_build(struct ffly_script*);
ffly_err_t ffly_script_gen_free();
ffly_err_t ffly_script_prepare(struct ffly_script*);
ffly_err_t ffly_script_ld(struct ffly_script*, char*);
ffly_err_t ffly_script_parse(struct ffly_script*);
ffly_err_t ffly_script_gen(struct ffly_script*);
ffly_err_t ffly_script_exec(struct ffly_script*);
ffly_err_t ffly_script_free(struct ffly_script*);
ffly_err_t ffly_script_save_bin(struct ffly_script*, char*);
# endif /*__ffly__script__h*/
