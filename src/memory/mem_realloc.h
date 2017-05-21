# ifndef __mem_realloc__h
# define __mem_realloc__h
# include <malloc.h>
# include "mem_alloc.h"
# ifdef __cplusplus
# include <cstddef>
namespace mdl {
namespace firefly {
namespace memory {
# if defined(__DEBUG_ENABLED) && defined(__WITH_MEM_TRACKER)
std::size_t extern alloc_bc;
std::size_t extern alloc_c;
# endif
void __inline__* mem_realloc(void *__mem_ptr, std::size_t __nbc) {
# ifdef __DEBUG_ENABLED
	u8_t *mem_ptr = ((u8_t*)__mem_ptr) - sizeof(std::size_t);
	uint_t mem_size = *((std::size_t *)mem_ptr);

	if (__nbc > mem_size)
		alloc_bc += __nbc - mem_size;
	else if (__nbc < mem_size)
		alloc_bc -= mem_size - __nbc;

	*((std::size_t *)mem_ptr) = __nbc;
	mem_ptr = ((u8_t*)realloc(mem_ptr, __nbc + sizeof(std::size_t))) + sizeof(std::size_t);
	return (void*)mem_ptr;
# else
	return realloc(__mem_ptr, __nbc);
# endif
}
}
}
}
# else
# include <stddef.h>
# if defined(__DEBUG_ENABLED) && defined(__WITH_MEM_TRACKER)
size_t extern ffly_mem_alloc_bc;
size_t extern ffly_mem_alloc_c;
# endif
void* ffly_mem_realloc(void*, size_t);
# endif
# endif /*__mem_realloc__h*/
