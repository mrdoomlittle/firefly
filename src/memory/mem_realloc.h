# ifndef __mem_realloc__h
# define __mem_realloc__h
# include <malloc.h>
# ifdef __cplusplus
# include <cstddef>
namespace mdl {
namespace firefly {
namespace memory {
__inline__ void *mem_realloc(void *__mptr, std::size_t __nbc) {
# ifdef __DEBUG_ENABLED
	__mptr -= sizeof(std::size_t);
	*((std::size_t *)__mptr) = __nbc;
	return realloc(__mptr, __nbc + sizeof(std::size_t)) + sizeof(std::size_t);
# else
	return realloc(__mptr, __nbc);
# endif
}
}
}
}
# else
# include <stddef.h>
void *ffly_mem_realloc(void *, size_t);
# endif
# endif /*__mem_realloc__h*/
