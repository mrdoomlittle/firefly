# ifndef __ffly__clay__input__h
# define __ffly__clay__input__h
# include "../clay.h"
extern void(*clay_get)(ff_uint_t, ff_uint_t, void*);
extern ff_u8_t(*clay_at_eof)(void);
ff_u8_t clay_getc(void);
void clay_ugetc(ff_u8_t);
# endif /*__ffly__clay__input__h*/
