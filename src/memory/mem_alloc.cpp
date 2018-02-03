# include "mem_alloc.h"
# include "../system/errno.h"
# include "../system/io.h"
# ifdef __ffly_use_cuda
#	include <cuda_runtime.h>
# endif
# ifdef __ffly_use_opencl
#	include <CL/cl.hpp>
#	include "../opencl_helper.hpp"
# endif

# ifdef __ffly_use_opencl
cl_mem_flags mdl::firefly::memory::__cl_mem_flags__;
# endif

# ifdef __ffly_use_opencl
cl_mem mdl::firefly::memory::gpu_mem_alloc(void*& __p, uint_t  __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __err) {
# else
mdl::firefly::types::err_t mdl::firefly::memory::gpu_mem_alloc(void** __p, uint_t  __bc) {
# endif
# ifdef __ffly_use_cuda
	types::cl_err_t err;
	if ((err = cudaMalloc(__p, __bc)) != ffly_cl_success) {
		ffly_fprintf(ffly_err, "mem_alloc: gpu, cuda: failed to alloc memory, errno: %d\n", err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
# endif
# ifdef __ffly_use_opencl
	types::cl_err_t err;
	cl_mem mem = clCreateBuffer(__context, __mem_flags, __bc, __p, &err);
	if (err != ffly_cl_success) {
		ffly_fprintf(ffly_err, "mem_alloc: gpu, opencl: failed to alloc memory, errno: %d\n", err);
		__err = FFLY_FAILURE;
		return mem;
	}
# endif
}
