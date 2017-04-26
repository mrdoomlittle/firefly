# ifndef __mem__alloc__h
# define __mem__alloc__h
# include <malloc.h>
# ifdef __WITH_MEM_TRACKER
#	include "../system/mem_tracker.h"
# endif

# ifdef __cplusplus
# include <cstddef>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __DEBUG_ENABLED
extern std::size_t alloc_bc;
extern std::size_t alloc_c;
# endif
# ifdef __WITH_MEM_TRACKER
void* mem_alloc(std::size_t __bc, bool __track_bypass = false);
# else
void* mem_alloc(std::size_t __bc);
# endif /*__WITH_MEM_TRACKER*/
}
}
}
extern "C" {
	extern size_t ffly_mem_alloc_bc;
	extern size_t ffly_mem_alloc_c;
}
# else
# include <eint_t.h>
# include <stddef.h>
# ifdef __DEBUG_ENABLED
extern size_t ffly_mem_alloc_bc;
extern size_t ffly_mem_alloc_c;
# endif
# ifdef __WITH_MEM_TRACKER
void *ffly_mem_alloc(size_t, mdl_u8_t);
# else
void *ffly_mem_alloc(size_t);
# endif /*__WITH_MEM_TRACKER*/
# endif


# endif /*__mem__alloc__h*/
