# ifndef __ffly__lexer__h
# define __ffly__lexer__h
# include "ffint.h"
# include "types.h"
# include "system/buff.h"

typedef struct ff_lexer {
	ff_u8_t *p, *end, *bed;
	struct ffly_buff tokbuf;
	ff_uint_t *line, *lo;
	struct ffly_buff sbuf;
	void *top;
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

void ffly_lexer_init(ff_lexerp);
void ffly_lexer_cleanup(ff_lexerp);
void ff_lexer_free(ff_lexerp, void*);
ff_tokenp ff_token_alloc();
void ff_token_free(ff_tokenp);
void ff_token_cleanup(ff_tokenp);
void ff_token_destroy(ff_tokenp);

struct token* ffly_lex(ff_lexerp, ff_err_t*);
void ffly_ulex(ff_lexerp, struct token*);
ff_bool_t at_eof(ff_lexerp);

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
	_and
};

# endif /*__ffly__lexer__h*/
