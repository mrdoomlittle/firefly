# ifndef __ffly__bzero__h
# define __ffly__bzero__h
# include "mem_set.h"
# include <mdlint.h>
void __inline__ static ffly_bzero(void *__p, mdl_uint_t __bc) {
	ffly_mem_set(__p, 0x0, __bc);
}
# endif /*__ffly__bzero__h*/
