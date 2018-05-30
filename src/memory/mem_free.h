# ifndef __ffly__mem__free__h
# define __ffly__mem__free__h
# ifndef __ffly_use_allocr
#   include <malloc.h>
# else
#   include "allocr.h"
# endif
# include "../ffint.h"
# include "../types.h"
# include "../system/atomic.h"
# include "../ffly_def.h"
# ifdef __ffly_mal_track
#	include "../system/mal_track.h"
# endif

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __ffly_debug
ff_atomic_uint_t extern ffly_mem_free_bc;
ff_atomic_uint_t extern ffly_mem_free_c;
# endif /*__ffly_debug_enabled*/

# ifdef __ffly_mal_track
ff_err_t ffly_mem_free(void*, ff_bool_t);
# else
ff_err_t ffly_mem_free(void*);
# endif /*__ffly_mal_track*/
# ifdef __cplusplus
}
# endif
# ifdef __cplusplus
namespace ff {
//types::err_t gpu_mem_free(void *__p);

# ifdef __ffly_mal_track
err_t __inline__ mem_free(void *__p, bool_t __track_bypass) {
	return ffly_mem_free(__p, __track_bypass);
}

err_t __inline__ mem_free(void *__p) {
	return mem_free(__p, ffly_false);
}

# else
static err_t(*mem_free)(void*) = &ffly_mem_free;
# endif /*__ffly_mal_track*/
}
# endif
# if defined(__ffly_mal_track)
#	ifdef __cplusplus
#		define __ffly_mem_free(__p) mdl::firefly::memory::mem_free(__p, 0)
#	else
#		define __ffly_mem_free(__p) ffly_mem_free(__p, 0)
#	endif
# else
#	ifdef __cplusplus
#		define __ffly_mem_free(__p) mdl::firefly::memory::mem_free(__p)
#   else
#		define __ffly_mem_free(__p) ffly_mem_free(__p)
#	endif
# endif
# endif /*__ffly__mem__free__h*/
