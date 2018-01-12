# ifndef __ffly__script__h
# define __ffly__script__h
# include "system/vec.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/buff.h"
# include "system/map.h"
# include "data/pair.h"
typedef struct {
/*
    
    -----------------
    | static memory |
    -----------------
    | to be used    |
    -----------------
*/
    ffly_byte_t *stack;
    ffly_byte_t *fresh;
    void *top; // top object
    void *arg_p;
    // id, arg_p, params that where passed thru. max 12
    void*(*call)(mdl_u8_t, void*, void**);
} ffscript;

typedef ffscript* ffscriptp;

struct ffly_script {
    struct ffly_map env, macros;
    struct ffly_map *local;
	ffly_byte_t *p, *end;
	ffly_off_t off;

    mdl_uint_t line, lo;
    struct ffly_vec nodes;
    struct ffly_vec vecs;
    struct ffly_vec maps;
   
    struct ffly_map typedefs; 
	struct ffly_buff sbuf;
	// token injection buffer
    struct ffly_vec to_free;
	struct ffly_buff iject_buff;
};

# ifdef __ffly_script_internal
# define TOK_IDENT 0
# define TOK_KEYWORD 1
# define TOK_NO 2
# define TOK_NULL 3
# define TOK_STR 4
# define TOK_NEWLINE 5
# define TOK_CHR 6
enum {
    _64l_u,
    _32l_u,
    _16l_u,
    _8l_u,
    _64l_s,
    _32l_s,
    _16l_s,
    _8l_s,
    _float32,
    _float64
};

enum {
    _astrisk,
    _l_arrow,
    _r_arrow,
    _comma,
    _colon,
    _eqeq,
	_eq,
    _neq,
    _gt,
    _lt,
    _ampersand,
    _percent,
    _period,
	_semicolon,
	_l_paren,
	_r_paren,
    _l_brace,
    _r_brace,
    _k_var,
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
    _k_void,
    _k_if,
    _k_fn,
    _k_extern,
    _k_struct,
    _k_exit,
    _k_while,
    _k_match,
    _incr,
    _decr,
    _k_else,
    _k_float,
    _k_typedef
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
    _op_lt,
    _ast_func,
    _ast_func_call,
    _ast_struct_ref,
    _ast_exit,
    _ast_while,
    _ast_incr,
    _ast_decr,
    _ast_match,
    _ast_call,
    _ast_addrof,
};

enum {
	_uint_t,
    _u64_t,
    _u32_t,
    _u16_t,
    _u8_t,
	_int_t,
	_i64_t,
	_i32_t,
	_i16_t,
	_i8_t,
    _void,
    _float,
    _struct,
    _unknown
};

# define _flg_neq 0x1
# define _flg_eq 0x2
# define _flg_gt 0x4
# define _flg_lt 0x8

enum {
	_op_fresh,
    _op_free,
	_op_assign,
	_op_copy,
	_op_print,
    _op_compare,
    _op_jump,
    _op_cond_jump,
    _op_zero,
    _op_push,
    _op_pop,
    _op_incr,
    _op_decr,
    _op_call,
    _op_exit,
    _op_frame,
    _op_free_frame
};

struct node {
    mdl_u8_t kind;
    struct type *_type;
    ffly_byte_t val[sizeof(mdl_u64_t)];
    struct node *init, *var, *arg;
    struct obj *_obj, **jmp, **ret;
    struct node *l, *r, *operand, *no;
    struct node *cond, *call, *_struct;
    struct ffly_vec block, args, params;
    struct ffly_vec _else, _do;
    ffly_pair pair;
    void *p;
    struct ffly_vec fields;
};

struct token {
	void *p;
	mdl_u8_t kind, id;
    mdl_uint_t line;
    ffly_off_t off, lo;
    mdl_u8_t is_float, is_hex;
};

struct type {
	mdl_u8_t kind;
	mdl_uint_t size;
    ffly_off_t off;
    struct ffly_map fields;
    struct type *ptr;
};

struct obj {
    mdl_u32_t off;
    mdl_u8_t opcode, cond;
    void *p;

    mdl_uint_t size;
    mdl_u8_t _type;

    // dst/src? or'
    struct obj *objpp, *frame;
    struct obj **dst, *_obj, **no;
    struct obj **to, **from, **l, **r, **by;
    struct obj **val, ***jmp, *flags;
    struct obj *next;
    struct obj **params[12];
};

char const* tokk_str(mdl_u8_t);
char const* tokid_str(mdl_u8_t);
mdl_uint_t tokcol(struct token*);
void skip_token(struct ffly_script*);
ffly_off_t toklo(struct token*);
ffly_off_t curlo(struct ffly_script*);
mdl_uint_t tokl(struct token*);
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
void vec_cleanup(struct ffly_script*, struct ffly_vec*);
void map_cleanup(struct ffly_script*, struct ffly_map*);
void cleanup(struct ffly_script*, void*);
ffly_bool_t next_tok_nl(struct ffly_script*);
# endif
ffly_err_t ffly_script_build(struct ffly_script*, void**, ffly_byte_t**);
ffly_err_t ffly_script_prepare(struct ffly_script*);
ffly_err_t ffly_script_ld(struct ffly_script*, char*);
ffly_err_t ffly_script_parse(struct ffly_script*);
ffly_err_t ffly_script_gen(struct ffly_script*, void**, ffly_byte_t**);
// ()
ffly_err_t ffscript_init(ffscriptp, mdl_uint_t);
ffly_err_t ffscript_exec(ffscriptp, void*(*)(mdl_u8_t, void*, void**), void*, void*, void*);
void* ffscript_call(ffscriptp, void*);
ffly_err_t ffscript_free(ffscriptp);
ffly_err_t ffly_script_free(struct ffly_script*);
ffly_err_t ffly_script_save_bin(struct ffly_script*, char*);
# endif /*__ffly__script__h*/
