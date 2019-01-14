# ifndef __ffly__inform__h
# define __ffly__inform__h
# include "ffint.h"
/*
	if we get an error or issue then inform the us or the user first.
*/
void(*ffly_inform)(ff_u16_t);

// user should use this or if they want use ffly_inform
void(*ffly_inform_user)(ff_u16_t);
# endif /*__ffly__inform__h*/
