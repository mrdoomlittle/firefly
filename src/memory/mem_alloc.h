# ifndef __ffly__mem__alloc__h
# define __ffly__mem__alloc__h
#ifndef __ffly_use_allocr
# include <malloc.h>
#else
# include "allocr.h"
#endif
# include "../ffint.h"
# include "../types.h"
# include "../ffly_def.h"
# ifdef __ffly_mal_track
#	include "../system/mal_track.h"
# endif

# ifdef __cplusplus
extern "C" {
# endif

# ifdef __ffly_debug
ff_u64_t extern ffly_mem_alloc_bc;
ff_u64_t extern ffly_mem_alloc_c;
# endif
extern void*(*__memalloc)(ff_uint_t);
# ifdef __ffly_mal_track
void* ffly_mem_alloc(ff_uint_t, ff_bool_t);
# else
void* ffly_mem_alloc(ff_uint_t);
# endif /*__ffly_mal_track*/

# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
# include "../system/errno.h"

namespace ff {
# ifdef __ffly_use_opencl
extern cl_mem_flags __cl_mem_flags__;
cl_mem gpu_mem_alloc(void*& __p, uint_t __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __any_err);

cl_mem __inline__ gpu_mem_alloc(void*& __p, uint_t __bc, types::err_t& __any_err) {
	return gpu_mem_alloc(__p, __bc, __cl_mem_flags__, __opencl_helper__.context, __any_err);}

# else
//types::err_t gpu_mem_alloc(void** __p, uint_t __bc);
# endif

# ifdef __ffly_mal_track
void __inline__* mem_alloc(uint_t __bc, bool_t __track_bypass) {
	return ffly_mem_alloc(__bc, __track_bypass);
}

void __inline__* mem_alloc(uint_t __bc) {
	return mem_alloc(__bc, ffly_false);
}

# else
static void*(*mem_alloc)(uint_t) = &ffly_mem_alloc;
# endif /*__ffly_mal_track*/
}
# endif
#if defined(__ffly_mal_track)
# ifdef __cplusplus
#	define __ffly_mem_alloc(__bc) \
		mdl::firefly::memory::mem_alloc(__bc, ffly_false)
# else
#	define __ffly_mem_alloc(__bc) \
		ffly_mem_alloc(__bc, ffly_false)
# endif
#else
# ifdef __cplusplus
#	define __ffly_mem_alloc(__bc) \
		mdl::firefly::memory::mem_alloc(__bc)
# else
#	define __ffly_mem_alloc(__bc) \
		ffly_mem_alloc(__bc)
# endif
#endif
# endif /*__ffly__mem__alloc__h*/
