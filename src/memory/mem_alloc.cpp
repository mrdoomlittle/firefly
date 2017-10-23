# include "mem_alloc.h"
# include "../system/errno.h"
# ifdef __USING_CUDA
#	include <cuda_runtime.h>
# endif
# ifdef __USING_OPENCL
#	include <CL/cl.hpp>
#	include "../opencl_helper.hpp"
# endif

# ifdef __USING_OPENCL
cl_mem_flags mdl::firefly::memory::__cl_mem_flags__;
# endif

# ifdef __USING_OPENCL
cl_mem mdl::firefly::memory::gpu_mem_alloc(void*& __p, uint_t  __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __any_err) {
# else
mdl::firefly::types::err_t mdl::firefly::memory::gpu_mem_alloc(void** __p, uint_t  __bc) {
# endif
# ifdef __USING_CUDA
	types::cu_err_t any_err = cudaSuccess;
	if ((any_err = cudaMalloc(__p, __bc)) != cudaSuccess) {
		fprintf(stderr, "mem_alloc: gpu, cuda: failed to alloc memory, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
# endif
# ifdef __USING_OPENCL
	types::cl_err_t any_err = CL_SUCCESS;
	cl_mem mem = clCreateBuffer(__context, __mem_flags, __bc, __p, &any_err);
	if (any_err != CL_SUCCESS) {
		fprintf(stderr, "mem_alloc: gpu, opencl: failed to alloc memory, errno: %d\n", any_err);
		__any_err = FFLY_FAILURE;
		return mem;
	}
# endif
}
