# ifndef __ffly__depart__h
# define __ffly__depart__h
/*
	rename 
	action:
	free any thing when pointer passed in is null
	example
	{
		ffly_depart(malloc(1));
		ffly_depart(malloc(1));
		ffly_depart(NULL); ^ both freed 
	}
*/
void ffly_depart(void*);
# endif /*__ffly__depart__h*/
