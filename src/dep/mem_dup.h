# ifndef __ffly__mem__dup__h
# define __ffly__mem__dup__h
# include "../ffint.h"
# include "../types.h"
ff_err_t ffly_mem_dup(void**, void const*, ff_u32_t);
ff_err_t __inline__ static ffly_mem_dupe(void **__p, void const *__src, ff_u32_t __bc) {
	return ffly_mem_dup(__p, __src, __bc);
}
# endif /*__ffly__mem__dup__h*/
