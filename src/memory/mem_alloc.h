# ifndef __mem__alloc__h
# define __mem__alloc__h
# include <malloc.h>
# include <mdlint.h>
# include "../types/bool_t.h"
# include "../ffly_def.h"

# ifdef __WITH_MEM_TRACKER
#	include "../system/mem_tracker.h"
# endif

# ifdef __cplusplus
# ifdef __USING_CUDA
#	include <cuda_runtime.h>
# endif
# ifdef __USING_OPENCL
#	include <CL/cl.hpp>
#	include "../opencl_helper.hpp"
# endif
# endif

# ifdef __cplusplus
extern "C" {
# endif

# ifdef __DEBUG_ENABLED
mdl_uint_t extern ffly_mem_alloc_bc;
mdl_uint_t extern ffly_mem_alloc_c;
# endif

# ifdef __WITH_MEM_TRACKER
void* ffly_mem_alloc(mdl_uint_t, ffly_bool_t);
# else
void* ffly_mem_alloc(mdl_uint_t);
# endif /*__WITH_MEM_TRACKER*/

# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
# include "../types/err_t.h"
# include "../system/errno.h"

namespace mdl {
namespace firefly {
namespace memory {
# ifdef __USING_OPENCL
extern cl_mem_flags __cl_mem_flags__;
cl_mem gpu_mem_alloc(void*& __mptr, std::size_t __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __any_err);

cl_mem __inline__ gpu_mem_alloc(void*& __mptr, std::size_t __bc, types::err_t& __any_err) {
	return gpu_mem_alloc(__mptr, __bc, __cl_mem_flags__, __opencl_helper__.context, __any_err);}

# else /*__USING_OPENCL*/
void gpu_mem_alloc(void*& __mptr, std::size_t __bc, types::err_t& __any_err);
# endif /*__USING_OPENCL*/

# ifdef __WITH_MEM_TRACKER
void __inline__* mem_alloc(uint_t __bc, types::bool_t __track_bypass) {
	return ffly_mem_alloc(__bc, __track_bypass);
}

void __inline__* mem_alloc(uint_t __bc) {
	return mem_alloc(__bc, ffly_false);
}

# else /*__WITH_MEM_TRACKER*/
static void*(*mem_alloc)(uint_t) = &ffly_mem_alloc;
# endif /*__WITH_MEM_TRACKER*/
}
}
}
# endif

# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
# 	ifdef __cplusplus
#		define __ffly_mem_alloc(__ALLOC_BC) mdl::firefly::memory::mem_alloc(__ALLOC_BC, ffly_false)
# 	else
#		define __ffly_mem_alloc(__ALLOC_BC) ffly_mem_alloc(__ALLOC_BC, ffly_false)
# 	endif
# else
#   ifdef __cplusplus
#		define __ffly_mem_alloc(__ALLOC_BC) mdl::firefly::memory::mem_alloc(__ALLOC_BC)
# 	else
#		define __ffly_mem_alloc(__ALLOC_BC) ffly_mem_alloc(__ALLOC_BC)
# 	endif
# endif

# endif /*__mem__alloc__h*/
