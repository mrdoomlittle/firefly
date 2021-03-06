# ifndef __ffly__mem__realloc__h
# define __ffly__mem__realloc__h
#ifndef __ffly_use_allocr
# include <malloc.h>
#else
# include "allocr.h"
#endif
# include "../ffint.h"
# include "../types.h"
# include "mem_alloc.h"
# include "../ffly_def.h"
# ifdef __cplusplus
extern "C" {
# endif
# if defined(__ffly_debug) && !defined(__ffly__mem__alloc__h)
ff_u64_t extern ffly_mem_alloc_bc;
ff_u64_t extern ffly_mem_alloc_c;
# endif
extern void*(*__memrealloc)(void*, ff_uint_t);
# ifdef __ffly_mal_track
void* ffly_mem_realloc(void*, ff_uint_t, ff_bool_t);
# else
void* ffly_mem_realloc(void*, ff_uint_t);
# endif
# ifdef __cplusplus
}
# endif
# ifdef __cplusplus
namespace ff {
# ifdef __ffly_mal_track
void __inline__* mem_realloc(void *__p, uint_t __bc, bool_t __track_bypass) {
	return ffly_mem_realloc(__p, __bc, __track_bypass);
}

void __inline__* mem_realloc(void *__p, uint_t __bc) {
	return mem_realloc(__p, __bc, ffly_false);
}
# else
static void*(*mem_realloc)(void*, uint_t) = &ffly_mem_realloc;
# endif
}
# endif
#if defined(__ffly_mal_track)
# ifdef __cplusplus
#	define __ffly_mem_realloc(__p, __bc) \
		mdl::firefly::memory::mem_realloc(__p, __bc, ffly_false)
# else
#	define __ffly_mem_realloc(__p, __bc) \
		ffly_mem_realloc(__p, __bc, ffly_false)
# endif
#else
# ifdef __cplusplus
#	define __ffly_mem_realloc(__p, __bc) \
		mdl::firefly::memory::mem_realloc(__p, __bc)
# else
#	define __ffly_mem_realloc(__p, __bc) \
		ffly_mem_realloc(__p, __bc)
# endif
#endif

# endif /*__ffly__mem__realloc__h*/
