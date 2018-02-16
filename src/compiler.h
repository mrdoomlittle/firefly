# ifndef __ffly__compiler__h
# define __ffly__compiler__h
# include "system/vec.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/buff.h"
# include "system/map.h"
# include "data/pair.h"
# include "types/bool_t.h"
# include "system/lat.h"

typedef struct ffly_compiler_file {
	ffly_byte_t *p, *end;
	ffly_off_t off;
	char *path;
	mdl_uint_t line, lo;
} *ffly_compiler_filep;

# ifdef __ffly_compiler_internal
struct keywd;
# endif

typedef struct ffly_compiler {
	void *ret_type;
    struct ffly_map env, macros;
    struct ffly_map *local;
    void *frame, *ret_to;
    void *brk[20], **brkp;

    struct ffly_compiler_file *file;

    struct ffly_vec nodes;
    struct ffly_vec vecs;
    struct ffly_vec maps;
   
    struct ffly_map typedefs; 
	struct ffly_buff sbuf;
    struct ffly_vec to_free;
    // rename
	struct ffly_buff iject_buff;

	/*
		keywords
	*/

	struct ffly_lat keywd;
	void *call[86];
} *ffly_compilerp;

# ifdef __ffly_compiler_internal
typedef struct keywd {
	struct keywd *next;
	mdl_u8_t id;
	char key[40];
} *keywdp;

# include "system/err.h"
# define errmsg(...) ffly_errmsg(__VA_ARGS__)
# define TOK_IDENT 0
# define TOK_KEYWORD 1
# define TOK_NO 2
# define TOK_NULL 3
# define TOK_STR 4
# define TOK_NEWLINE 5
# define TOK_CHR 6

# define ring_parser_func_call(__compiler, ...) \
	ringup(ffly_err_t, __compiler, _parser_func_call, __VA_ARGS__)
# define ring_parser_decl_spec(__compiler, ...) \
	ringup(ffly_err_t, __compiler, _parser_func_call, __VA_ARGS__)

void set_call(struct ffly_compiler*, void*, mdl_u8_t);
mdl_u64_t _ringup(struct ffly_compiler*, mdl_u8_t, ...);
# define ringup(__rettype, __compiler, __no, ...) \
	((__rettype)_ringup(__compiler, __no, __VA_ARGS__))

enum {
	_parser_func_call,
	_parser_decl_spec
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
    _k_typedef,
    _k_ret,
    _k_brk,
    _ellipsis,
    _plus,
    _minus
};

enum {
    _ast_ret,
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
    _ast_conv,
    _op_cast,
    _ast_brk,
    _op_add,
    _op_sub,
    _op_mul,
    _op_div
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

struct node {
    mdl_u8_t kind, op;
    struct type *_type, *to;
    ffly_byte_t val[sizeof(mdl_u64_t)];
    struct node *init, *var, *arg;
    struct obj *_obj, **jmp;
    struct node *l, *r, *operand, *no, *ret;
    struct node *cond, *call, *_struct;
    struct ffly_vec block, args, params;
    struct ffly_vec _else, _do;
    ffly_pair pair;
    void *p;
    ffly_bool_t va;
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

char const* tokk_str(mdl_u8_t);
char const* tokid_str(mdl_u8_t);
mdl_uint_t tokcol(struct token*);
void skip_token(struct ffly_compiler*);
ffly_off_t toklo(struct token*);
ffly_off_t curlo(struct ffly_compiler*);
mdl_uint_t tokl(struct token*);
mdl_uint_t curl(struct ffly_compiler*);
ffly_bool_t next_token_is(struct ffly_compiler*, mdl_u8_t, mdl_u8_t);
ffly_bool_t is_keyword(struct token*, mdl_u8_t);
ffly_bool_t expect_token(struct ffly_compiler*, mdl_u8_t, mdl_u8_t);
struct token* next_token(struct ffly_compiler*);
struct token* peek_token(struct ffly_compiler*);
// change to lex & ulex without 'ffly_'
struct token* ffly_lex(struct ffly_compiler*, ffly_err_t*);
void ffly_ulex(struct ffly_compiler*, struct token*);

ffly_bool_t maybe_keyword(struct ffly_compiler*, struct token*);
ffly_bool_t at_eof(struct ffly_compiler*);
void pr_tok(struct token*);
void vec_cleanup(struct ffly_compiler*, struct ffly_vec*);
void map_cleanup(struct ffly_compiler*, struct ffly_map*);
void cleanup(struct ffly_compiler*, void*);
ffly_bool_t next_tok_nl(struct ffly_compiler*);
# endif
ffly_err_t ffly_compiler_ld(struct ffly_compiler*, char const*);
ffly_err_t ffly_compiler_build(struct ffly_compiler*, void **, ffly_byte_t**);
ffly_err_t ffly_compiler_prepare(struct ffly_compiler*);
ffly_err_t ffly_compiler_free(struct ffly_compiler*);
mdl_u8_t ffly_compiler_kwno(char const*);
void ffly_compiler_ldkeywd(struct ffly_compiler*, mdl_u8_t);
ffly_err_t ffly_parse(struct ffly_compiler*);
ffly_err_t ffly_gen(struct ffly_compiler*, void**, ffly_byte_t**);
# endif /*__ffly__compiler__h*/
