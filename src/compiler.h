# ifndef __ffly__compiler__h
# define __ffly__compiler__h
# include "system/vec.h"
# include "types.h"
# include "system/buff.h"
# include "system/map.h"
# include "data/pair.h"
# include "system/lat.h"
# include "lexer.h"
# include "linux/limits.h"
# ifdef __ffly_script
# ifndef ffc_final
# define ffc_final(__compiler, __top, __stack, __syput) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t)))_ffc_final)(__compiler, __top, __stack, __syput)
# endif /*ffc_final*/
# ifndef ffc_build
# define ffc_build(__compiler, __top, __stack, __syput) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t)))_ffc_build)(__compiler, __top, __stack, __syput)
# endif /*ffc_build*/
# ifndef ffc_parse
# define ffc_parse(__compiler) \
	((ff_err_t(*)(struct ffly_compiler*))_ffc_parse)(__compiler, __top, __stack)
# endif /*ffc_parse*/
# ifndef ffc_gen
# define ffc_gen(__compiler, __top, __stack, __syput) \
	((ff_err_t(*)(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t)))_ffc_gen)(__compiler, __top, __stack, __syput)
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
	char const *path;
	char dir[PATH_MAX];
	ff_uint_t line, lo;
} *ffly_compiler_filep;

# ifdef __ffly_compiler_internal
struct keywd;
# endif

typedef struct ffly_compiler {
	void *ret_type;
	struct ffly_map env, macros;
	ffly_mapp local;
	ffly_vecp var_pond;
	void *frame, *ret_to;
	void *brk[20], **brkp;

	struct ffly_compiler_file *file;

	struct ffly_vec nodes;
	struct ffly_vec vecs;
	struct ffly_vec maps;
   
	struct ffly_map typedefs; 
	struct ffly_buff sbuf;
	struct ffly_vec to_free;
	char *dir;

	ff_u8_t lang;
	struct ff_lexer lexer;

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
	_ast_syput,
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
	_ast_as,
	_ast_out,
	_ast_label,
	_ast_jmpto,
	_ast_va_args
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
	_unknown,
	_array
};

# define _flg_neq 0x1
# define _flg_eq 0x2
# define _flg_gt 0x4
# define _flg_lt 0x8

# define _func_exr 0x1
# define _func_gbl 0x2
# define _func_lca 0x4
# define _func_def 0x8
// clean up

struct exec_reg {
	struct obj **start;
	struct obj **end;
};

struct node {
	ff_u8_t kind, op;

	struct type *_type, *to; 
	ff_byte_t val[sizeof(ff_u64_t)];
	struct node *init, *var, *arg;
	struct obj *_obj, **jmp;
	struct node *l, *r, *operand, *no, *ret, *code;
	struct node *cond, *call, *_struct;
	struct ffly_vec block, args, params;
	struct ffly_vec _else, _do;
	// exec region
	struct exec_reg er;
	char const *name;
	void *p;
	ff_bool_t va;
	ff_u8_t flags;
	ff_int_t s_off;
	struct node **va_param;
	struct node *rtv;
	struct ffly_vec var_pond;
	struct ffly_vec fields;
};

struct type {
	ff_u8_t kind;
	ff_uint_t size, len;
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

ff_bool_t maybe_keyword(struct ffly_compiler*, struct token*);
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
