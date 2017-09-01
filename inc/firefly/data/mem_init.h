# ifndef __mem__init__h
# define __mem__init__h
# include "mem_set.h"
void __inline__* ffly_mem_init(void *__p, mdl_uint_t __bc) {
	ffly_mem_set(__p, 0, __bc);
	return __p;
}

# endif /*__mem__init__h*/
