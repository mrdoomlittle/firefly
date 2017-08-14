# ifndef __mem__alloc__h
# define __mem__alloc__h
# include <malloc.h>
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
# include <cstddef>
# include <eint_t.hpp>
# include "../types/err_t.h"
# include "../system/errno.h"

extern "C" {
# ifdef __WITH_MEM_TRACKER
	void extern* ffly_mem_alloc(std::size_t, mdl::u8_t);
# else
	void extern* ffly_mem_alloc(std::size_t);
# endif /*__WITH_MEM_TRACKER*/

# ifdef __DEBUG_ENABLED
	extern std::size_t ffly_mem_alloc_bc;
	extern std::size_t ffly_mem_alloc_c;
# endif
}

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
void __inline__* mem_alloc(std::size_t __bc, bool __track_bypass) {
	return ffly_mem_alloc(__bc, __track_bypass);
}

void __inline__* mem_alloc(std::size_t __bc) {
	return mem_alloc(__bc, false);}

# else /*__WITH_MEM_TRACKER*/
void static*(*mem_alloc)(std::size_t) = &ffly_mem_alloc;
# endif /*__WITH_MEM_TRACKER*/
}
}
}
# else /*c section*/

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

# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
# 	ifdef __cplusplus
#		define __ffly_mem_alloc(__ALLOC_BC) mdl::firefly::memory::mem_alloc(__ALLOC_BC, 0)
# 	else
#		define __ffly_mem_alloc(__ALLOC_BC) ffly_mem_alloc(__ALLOC_BC, 0)
# 	endif
# else
#   ifdef __cplusplus
#		define __ffly_mem_alloc(__ALLOC_BC) mdl::firefly::memory::mem_alloc(__ALLOC_BC)
# 	else
#		define __ffly_mem_alloc(__ALLOC_BC) ffly_mem_alloc(__ALLOC_BC)
# 	endif
# endif

# endif /*__mem__alloc__h*/
