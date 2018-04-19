# ifndef __ffly__compiler__h
# define __ffly__compiler__h
# include "system/vec.h"
# include "types.h"
# include "system/buff.h"
# include "system/map.h"
# include "data/pair.h"
# include "system/lat.h"

# ifdef __ffly_script
# ifndef ffc_final
# define ffc_final(__compiler, __top, __stack) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**))_ffc_final)(__compiler, __top, __stack)
# endif /*ffc_final*/
# ifndef ffc_build
# define ffc_build(__compiler, __top, __stack) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**))_ffc_build)(__compiler, __top, __stack)
# endif /*ffc_build*/
# ifndef ffc_parse
# define ffc_parse(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_parse)(__compiler, __top, __stack)
# endif /*ffc_parse*/
# ifndef ffc_gen
# define ffc_gen(__compiler, __top, __stack) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**))_ffc_gen)(__compiler, __top, __stack)
# endif /*ffc_gen*/
# elif defined(__ffly_ff)
# ifndef ffc_final
# define ffc_final(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_final)(__compiler)
# endif /*ffc_final*/
# ifndef ffc_build
# define ffc_build(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_build)(__compiler)
# endif /*ffc_build*/
# ifndef ffc_parse
# define ffc_parse(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_parse)(__compiler)
# endif /*ffc_parse*/
# ifndef ffc_gen
# define ffc_gen(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_gen)(__compiler)
# endif /*ffc_gen*/
# endif

enum {
	_ffc_ff,
	_ffc_script
};

/*
	start renaming to ffc_
	for firefly compiler _ ...

*/
typedef struct ffly_compiler_file {
	ff_byte_t *p, *end;
	ff_off_t off;
	char *path;
	ff_uint_t line, lo;
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
	ff_u8_t lang;


	void(*out)(void*, ff_uint_t);
	struct ffly_lat keywd;
	void *call[86];
} *ffly_compilerp;

# define ff_compilerp ffly_compilerp

# ifdef __ffly_compiler_internal
typedef struct keywd {
	struct keywd *next;
	ff_u8_t id;
	char key[40];
} *keywdp;

# include "system/err.h"
# define errmsg(...) ffly_errmsg(__VA_ARGS__)
# define _tok_ident 0
# define _tok_keywd 1
# define _tok_no 2
# define _tok_null 3
# define _tok_str 4
# define _tok_newline 5
# define _tok_chr 6

# define ring_parser_func_call(__compiler, ...) \
	ringup(ff_err_t, __compiler, _parser_func_call, __VA_ARGS__)
# define ring_parser_decl_spec(__compiler, ...) \
	ringup(ff_err_t, __compiler, _parser_func_call, __VA_ARGS__)

void set_call(struct ffly_compiler*, void*, ff_u8_t);
ff_u64_t _ringup(struct ffly_compiler*, ff_u8_t, ...);
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
	_k_as,
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
    _op_div,
	_ast_as
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
    ff_u8_t kind, op;
    struct type *_type, *to;
    ff_byte_t val[sizeof(ff_u64_t)];
    struct node *init, *var, *arg;
    struct obj *_obj, **jmp;
    struct node *l, *r, *operand, *no, *ret;
    struct node *cond, *call, *_struct;
    struct ffly_vec block, args, params;
    struct ffly_vec _else, _do;
    ffly_pair pair;
    void *p;
    ff_bool_t va;
    struct ffly_vec fields;
};

struct token {
	void *p;
	ff_u8_t kind, id;
    ff_uint_t line;
    ff_off_t off, lo;
    ff_u8_t is_float, is_hex;
};

struct type {
	ff_u8_t kind;
	ff_uint_t size;
    ff_off_t off;
    struct ffly_map fields;
    struct type *ptr;
};

char const* tokk_str(ff_u8_t);
char const* tokid_str(ff_u8_t);
ff_uint_t tokcol(struct token*);
void sktok(ffly_compilerp);
ff_off_t toklo(struct token*);
ff_off_t curlo(struct ffly_compiler*);
ff_uint_t tokl(struct token*);
ff_uint_t curl(struct ffly_compiler*);
ff_bool_t next_token_is(struct ffly_compiler*, ff_u8_t, ff_u8_t);
ff_bool_t is_keyword(struct token*, ff_u8_t);
ff_bool_t expect_token(struct ffly_compiler*, ff_u8_t, ff_u8_t);
struct token* next_token(struct ffly_compiler*);
struct token* peek_token(struct ffly_compiler*);
// change to lex & ulex without 'ffly_'
struct token* ffly_lex(struct ffly_compiler*, ff_err_t*);
void ffly_ulex(struct ffly_compiler*, struct token*);


ff_bool_t maybe_keyword(struct ffly_compiler*, struct token*);
ff_bool_t at_eof(struct ffly_compiler*);
void pr_tok(struct token*);
void vec_cleanup(struct ffly_compiler*, struct ffly_vec*);
void map_cleanup(struct ffly_compiler*, struct ffly_map*);
void cleanup(struct ffly_compiler*, void*);
ff_bool_t next_tok_nl(struct ffly_compiler*);
# endif
ff_err_t ffly_compiler_ld(struct ffly_compiler*, char const*);

void extern *_ffc_final;
void extern *_ffc_build;
void extern *_ffly_parse;
void extern *_ffly_gen;

ff_err_t ffly_compiler_prepare(struct ffly_compiler*);
ff_err_t ffly_compiler_free(struct ffly_compiler*);
ff_u8_t ffly_compiler_kwno(char const*);
void ffly_compiler_ldkeywd(struct ffly_compiler*, ff_u8_t);
void ffc_ldlang(ffly_compilerp, ff_u8_t);
# endif /*__ffly__compiler__h*/
