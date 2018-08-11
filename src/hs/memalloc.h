# ifndef __ffly__memalloc__h
# define __ffly__memalloc__h
# include "../ffint.h"
void* ff_hs_mem_alloc(ff_uint_t);
void ff_hs_mem_free(void*);
void* ff_hs_mem_realloc(void*, ff_uint_t);
void ff_hs_mem_clean(void);
# endif /*__ffly__memalloc__h*/
