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
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __DEBUG_ENABLED
std::size_t extern alloc_bc;
std::size_t extern alloc_c;
# endif

# ifdef __USING_OPENCL
extern cl_mem_flags __cl_mem_flags__;
cl_mem gpu_mem_alloc(void*& __mptr, std::size_t __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __any_err);


cl_mem __inline__ gpu_mem_alloc(void*& __mptr, std::size_t __bc, types::err_t& __any_err) {
	return gpu_mem_alloc(__mptr, __bc, __cl_mem_flags__, __opencl_helper__.context, __any_err);
}
/*
void __inline__ gpu_mem_alloc(void*& __mptr, std::size_t __bc, cl_mem_flags __mem_flags) {
	gpu_mem_alloc(__mptr, __bc, __mem_flags, __opencl_helper__.context);
}
void __inline__ gpu_mem_alloc(void*& __mptr, std::size_t __bc, cl_context __context) {
	gpu_mem_alloc(__mptr, __bc, __cl_mem_flags__, __context);
}*/

# else
void gpu_mem_alloc(void*& __mptr, std::size_t __bc, types::err_t& __any_err);
# endif

# ifdef __WITH_MEM_TRACKER
void* mem_alloc(std::size_t __bc, bool __track_bypass = false);
void __inline__* mem_alloc(std::size_t __bc) {return mem_alloc(__bc, false);}
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

# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
# 	ifdef __cplusplus
#		define __ffly_mem_alloc(__MEM_PTR) mdl::firefly::memory::mem_alloc(__MEM_PTR, 0)
# 	else
#		define __ffly_mem_alloc(__MEM_PTR) ffly_mem_alloc(__MEM_PTR, 0)
# 	endif
# else
#   ifdef __cplusplus
#		define __ffly_mem_alloc(__MEM_PTR) mdl::firefly::memory::mem_alloc(__MEM_PTR)
# 	else
#		define __ffly_mem_alloc(__MEM_PTR) ffly_mem_alloc(__MEM_PTR)
# 	endif
# endif

# endif /*__mem__alloc__h*/
