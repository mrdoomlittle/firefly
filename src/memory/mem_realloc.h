# ifndef __mem_realloc__h
# define __mem_realloc__h

namespace mdl {
namespace firefly {
namespace memory {
__inline__ void *mem_realloc(void *__old, std::size_t __nbytes) {
	return realloc(__old, __nbytes);
}
}
}
}

# endif /*__mem_realloc__h*/
