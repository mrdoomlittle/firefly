# ifndef __ffly__mem__cpy__h
# define __ffly__mem__cpy__h
# include "../ffint.h"
ff_u8_t ffly_mem_cpy(void*, void const*, ff_u32_t);
# ifdef __cplusplus
# include "../ffly_def.h"
namespace ff {
ff_u8_t static __force_inline__ mem_cpy(void *__dst, void const *__src, ff_u32_t __size) {
	return ffly_mem_cpy(__dst, __src, __size);
}
}
# endif
# endif /*__ffly__mem__cpy__h*/
