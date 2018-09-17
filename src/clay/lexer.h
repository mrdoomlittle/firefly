# ifndef __ffly__clay__lexer__h
# define __ffly__clay__lexer__h
# include "../clay.h"
ff_uint_t extern clay_tok_sz;
ff_u8_t extern clay_tok_val;
void extern* clay_tok_data;

ff_u8_t clay_nexttok(ffly_clayp);
ff_u8_t clay_peektok(ffly_clayp);
void clay_rtok(ff_u8_t);
# endif /*__ffly__clay__lexer__h*/
