# ifndef __ffly__shell__lexer__h
# define __ffly__shell__lexer__h
# include "../ffint.h"
ff_u8_t nexttok(void);
ff_u8_t peektok(void);
char const* tokstr(ff_u8_t);
enum {
	_tok_uk,
	_tok_ident,
	_tok_keywd,
	_tok_str,
	_tok_eof,
	_tok_nl
};

extern void *tok_data;
extern ff_uint_t tok_sz;
extern ff_u8_t tok_val;
# endif /*__ffly__shell__lexer__h*/
