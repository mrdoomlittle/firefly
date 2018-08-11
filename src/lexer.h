# ifndef __ffly__lexer__h
# define __ffly__lexer__h
# include "ffint.h"
# include "types.h"
# include "system/buff.h"

# define LCS 20
typedef struct ff_lexer {
	ff_u32_t off, end;
	struct ffly_buff tokbuf;
	ff_uint_t *line, *lo;
	struct ffly_buff sbuf;
	void *top;
	void *arg;
	void(*get)(ff_uint_t, ff_uint_t, ff_uint_t, void*, void*);

	ff_u8_t c[LCS];
	ff_u32_t c_off;

	ff_u8_t flags;
} *ff_lexerp;

# define token ff_token
typedef struct ff_token {
	ff_uint_t page, pg_off;
	void *p;
	ff_u8_t kind, id;
	ff_uint_t line, l;
	ff_off_t off, lo;
	ff_u8_t is_float, is_hex;

	struct token *fd;
} *ff_tokenp;

# ifdef __ffly_compiler_internal
void ffly_lexer_init(ff_lexerp);
void ffly_lexer_cleanup(ff_lexerp);
void ff_lexer_free(ff_lexerp, void*);
ff_tokenp ff_token_alloc(void);
void ff_token_free(ff_tokenp);
void ff_token_cleanup(ff_tokenp);
void ff_token_destroy(ff_tokenp);
void ff_lexer_reset(ff_lexerp);

struct token* ffly_lex(ff_lexerp, ff_err_t*);
void ffly_ulex(ff_lexerp, struct token*);
ff_bool_t at_eof(ff_lexerp);

# define is_tokident(__tok) \
	((__tok)->kind == _tok_ident)
# define is_tokstr(__tok) \
	((__tok)->kind == _tok_str)


# define _tok_ident 0
# define _tok_keywd 1
# define _tok_no 2
# define _tok_null 3
# define _tok_str 4
# define _tok_newline 5
# define _tok_chr 6

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
	_l_bracket,
	_r_bracket,
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
	_k_out,
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
    _minus,
	_k_goto,
	_k_syput,
	_k_va_args,
	_k_sizeof,
	_and,
	_k_no_va,
	_k_no_params,
	_k_no_ret
};

# endif
# endif /*__ffly__lexer__h*/
