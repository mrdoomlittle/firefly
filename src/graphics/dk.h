# ifndef __ffly__graphics__dk__h
# define __ffly__graphics__dk__h
# include "../ffint.h"
/*
	find name for this, in short memory that is passed to graphics functions are not copyed
	so the memory will have to stay until done.
*/

void ffly_grdk_add(void(*)(void));
void ffly_grdk_run(void);
# endif /*__ffly__graphics__dk__h*/
