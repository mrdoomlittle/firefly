# ifndef __mem__free__h
# define __mem__free__h
# include <malloc.h>
# include <mdlint.h>
# include "../types/bool_t.h"
# include "../types/err_t.h"
# include "../system/atomic.h"
# include "../ffly_def.h"
# ifdef __MAL_TRACK
#	include "../system/mal_track.h"
# endif

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __DEBUG_ENABLED
ffly_atomic_uint_t extern ffly_mem_free_bc;
ffly_atomic_uint_t extern ffly_mem_free_c;
# endif /*__DEBUG_ENABLED*/

# ifdef __MAL_TRACK
ffly_err_t ffly_mem_free(void*, ffly_bool_t);
# else
ffly_err_t ffly_mem_free(void*);
# endif /*__MAL_TRACK*/
# ifdef __cplusplus
}
# endif
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace memory {
types::err_t gpu_mem_free(void *__p);

# ifdef __MAL_TRACK
types::err_t __inline__ mem_free(void *__mem_ptr, types::bool_t __track_bypass) {
	return ffly_mem_free(__mem_ptr, __track_bypass);
}

types::err_t __inline__ mem_free(void *__mem_ptr) {
	return mem_free(__mem_ptr, ffly_false);
}

# else
static types::err_t(*mem_free)(void*) = &ffly_mem_free;
# endif /*__MAL_TRACK*/
}
}
}
# endif

# if defined(__MAL_TRACK)
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
