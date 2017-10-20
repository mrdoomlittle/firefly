# ifndef __mem__realloc__h
# define __mem__realloc__h
# include <malloc.h>
# include <mdlint.h>
# include "mem_alloc.h"
# include "../ffly_def.h"
# include "../types/bool_t.h"
# include "../system/atomic.h"
# ifdef __cplusplus
extern "C" {
# endif
# if defined(__DEBUG_ENABLED) && !defined(__mem__alloc__h)
ffly_atomic_uint_t extern ffly_mem_alloc_bc;
ffly_atomic_uint_t extern ffly_mem_alloc_c;
# endif
# ifdef __WITH_MEM_TRACKER
void* ffly_mem_realloc(void*, mdl_uint_t, ffly_bool_t);
# else
void* ffly_mem_realloc(void*, mdl_uint_t);
# endif
# ifdef __cplusplus
}
# endif
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __WITH_MEM_TRACKER
void __inline__* mem_realloc(void *__mem_ptr, uint_t __bc, types::bool_t __track_bypass) {
	return ffly_mem_realloc(__mem_ptr, __bc, __track_bypass);
}

void __inline__* mem_realloc(void *__mem_ptr, uint_t __bc) {
	return mem_realloc(__mem_ptr, __bc, ffly_false);
}
# else
static void*(*mem_realloc)(void*, uint_t) = &ffly_mem_realloc;
# endif
}
}
}
# endif
# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
#	ifdef __cplusplus
#		define __ffly_mem_realloc(__MEM_PTR, __ALLOC_BC) mdl::firefly::memory::mem_realloc(__MEM_PTR, __ALLOC_BC, ffly_false)
#	else
#		define __ffly_mem_realloc(__MEM_PTR, __ALLOC_BC) ffly_mem_realloc(__MEM_PTR, __ALLOC_BC, ffly_false)
#	endif
# else
#	ifdef __cplusplus
#		define __ffly_mem_realloc(__MEM_PTR, __ALLOC_BC) mdl::firefly::memory::mem_realloc(__MEM_PTR, __ALLOC_BC)
#	else
#		define __ffly_mem_realloc(__MEM_PTR, __ALLOC_BC) ffly_mem_realloc(__MEM_PTR, __ALLOC_BC)
#	endif
# endif

# endif /*__mem__realloc__h*/
