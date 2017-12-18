# ifndef __ffly__mem__free__h
# define __ffly__mem__free__h
# include <malloc.h>
# include <mdlint.h>
# include "../types/bool_t.h"
# include "../types/err_t.h"
# include "../system/atomic.h"
# include "../ffly_def.h"
# ifdef __ffly_mal_track
#	include "../system/mal_track.h"
# endif

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __ffly_debug_enabled
ffly_atomic_uint_t extern ffly_mem_free_bc;
ffly_atomic_uint_t extern ffly_mem_free_c;
# endif /*__ffly_debug_enabled*/

# ifdef __ffly_mal_track
ffly_err_t ffly_mem_free(void*, ffly_bool_t);
# else
ffly_err_t ffly_mem_free(void*);
# endif /*__ffly_mal_track*/
# ifdef __cplusplus
}
# endif
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace memory {
types::err_t gpu_mem_free(void *__p);

# ifdef __ffly_mal_track
types::err_t __inline__ mem_free(void *__p, types::bool_t __track_bypass) {
	return ffly_mem_free(__p, __track_bypass);
}

types::err_t __inline__ mem_free(void *__p) {
	return mem_free(__p, ffly_false);
}

# else
static types::err_t(*mem_free)(void*) = &ffly_mem_free;
# endif /*__ffly_mal_track*/
}
}
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
