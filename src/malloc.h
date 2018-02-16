# ifndef __ffly__malloc__h
# define __ffly__malloc__h
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# define malloc(__bc) \
	__ffly_mem_alloc(__bc)
# define free(__p) \
	__ffly_mem_free(__p)
# define realloc(__p, __bc) \
	__ffly_mem_realloc(__p, __bc)
# endif /*__ffly__malloc__h*/
