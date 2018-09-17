# ifndef __ffly__clay__memalloc__h
# define __ffly__clay__memalloc__h
# include "../ffint.h"
void* clay_mem_alloc(ff_uint_t);
void clay_mem_free(void*);
void clay_mem_cleanup(void);
# endif /*__ffly__clay__memalloc__h*/
