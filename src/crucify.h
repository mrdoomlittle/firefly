# ifndef __ffly__crucify__h
# define __ffly__crucify__h
# include "ffint.h"
/*
	rename

	when init is called a de-init
	function will be passed to hear.
*/
void ffly_crucify(void(*)(void*), void*);
void ffly_crucify_run(void);
# endif /*__ffly__crucify__h*/
