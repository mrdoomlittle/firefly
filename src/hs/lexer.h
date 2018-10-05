# ifndef __ffly__hs__lexer__h
# define __ffly__hs__lexer__h
# include "../hs.h"
ff_uint_t extern hs_tok_sz;
ff_u8_t extern hs_tok_val;
void extern* hs_tok_data;
ff_u8_t hs_nexttok(ff_hsp);
ff_u8_t hs_peektok(ff_hsp);
void hs_rtok(ff_u8_t);
# endif /*__ffly__hs__lexer__h*/
