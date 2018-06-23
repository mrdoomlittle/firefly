# ifndef __ffly__shell__memalloc__h
# define __ffly__shell__memalloc__h
# include "../ffint.h"
void* mem_alloc(ff_uint_t);
void mem_free(void*);
void mem_cleanup(void);
# endif /*__ffly__shell__memalloc__h*/
