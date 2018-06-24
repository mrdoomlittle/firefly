# ifndef __ffly__shell__input__h
# define __ffly__shell__input__h
# include "../ffint.h"
ff_u8_t getc(void);
extern void(*get)(ff_uint_t, ff_uint_t, void*);
extern ff_u8_t(*at_eof)(void);
void ugetc(ff_u8_t);
# endif /*__ffly__shell__input__h*/
