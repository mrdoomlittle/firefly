# ifndef __ffly__typo__memalloc__h
# define __ffly__typo__memalloc__h
# include "../ffint.h"
void* ffly_tmalloc(ff_uint_t);
void ffly_tfree(void*);
void* ffly_trealloc(void*, ff_uint_t);
void ffly_tmpurge(void);
# endif /*__ffly__typo__memalloc__h*/
