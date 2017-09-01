# ifndef __mem__free__h
# define __mem__free__h
# include <malloc.h>
# include <mdlint.h>
# include "../types/bool_t.h"
# ifdef __WITH_MEM_TRACKER
#	include "../system/mem_tracker.h"
# endif

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __DEBUG_ENABLED
mdl_uint_t ffly_mem_free_bc;
mdl_uint_t ffly_mem_free_c;
# endif /*__DEBUG_ENABLED*/

# ifdef __WITH_MEM_TRACKER
void ffly_mem_free(void*, ffly_bool_t);
# else
void ffly_mem_free(void*);
# endif /*__WITH_MEM_TRACKER*/
# ifdef __cplusplus
}
# endif


# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __WITH_MEM_TRACKER
void __inline__ mem_free(void *__mem_ptr, types::bool_t __track_bypass) {
	ffly_mem_free(__mem_ptr, __track_bypass);
}

void __inline__ mem_free(void *__mem_ptr) {
	mem_free(__mem_ptr, ffly_false);
}

# else
static void(*mem_free)(void*) = &ffly_mem_free;
# endif /*__WITH_MEM_TRACKER*/
}
}
}
# endif

# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
#	ifdef __cplusplus
#		define __ffly_mem_free(__MEM_PTR) mdl::firefly::memory::mem_free(__MEM_PTR, 0)
#	else
#		define __ffly_mem_free(__MEM_PTR) ffly_mem_free(__MEM_PTR, 0)
#	endif
# else
#	ifdef __cplusplus
#		define __ffly_mem_free(__MEM_PTR) mdl::firefly::memory::mem_free(__MEM_PTR)
#   else
#		define __ffly_mem_free(__MEM_PTR) ffly_mem_free(__MEM_PTR)
#	endif
# endif

# endif /*__mem__free__h*/
