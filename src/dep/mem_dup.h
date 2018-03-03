# ifndef __ffly__mem__dup__h
# define __ffly__mem__dup__h
# include <mdlint.h>
# include "../types/err_t.h"
ffly_err_t ffly_mem_dup(void**, void const*, mdl_u32_t);
ffly_err_t __inline__ static ffly_mem_dupe(void **__p, void const *__src, mdl_u32_t __bc) {
	return ffly_mem_dup(__p, __src, __bc);
}
# endif /*__ffly__mem__dup__h*/
