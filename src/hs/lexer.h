# ifndef __ffly__hs__lexer__h
# define __ffly__hs__lexer__h
# include "../hs.h"
ff_uint_t extern tok_sz;
ff_u8_t extern tok_val;
void extern* tok_data;
ff_u8_t nexttok(ff_hsp);
ff_u8_t peektok(ff_hsp);
void rtok(ff_u8_t);
# endif /*__ffly__hs__lexer__h*/
