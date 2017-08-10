# ifndef __mem__realloc__h
# define __mem__realloc__h
# include <malloc.h>
# include "mem_alloc.h"
# ifdef __cplusplus
# include <cstddef>

extern "C" {
	void extern* ffly_mem_realloc(void*, std::size_t);
# if defined(__DEBUG_ENABLED) && !defined(__mem__halloc__h)
	std::size_t extern ffly_mem_alloc_bc;
	std::size_t extern ffly_mem_alloc_c;
# endif
}

namespace mdl {
namespace firefly {
namespace memory {
void static*(*mem_realloc)(void*, std::size_t) = &ffly_mem_realloc;
}
}
}
# else /*c section*/

# include <stddef.h>
# if defined(__DEBUG_ENABLED) && !defined(__mem__alloc__h)
size_t extern ffly_mem_alloc_bc;
size_t extern ffly_mem_alloc_c;
# endif

void* ffly_mem_realloc(void*, size_t);

# endif
# endif /*__mem__realloc__h*/
