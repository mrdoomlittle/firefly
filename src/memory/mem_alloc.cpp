# include "mem_alloc.h"
# ifdef __USING_OPENCL
cl_mem_flags mdl::firefly::memory::__cl_mem_flags__;
# endif

# ifdef __USING_OPENCL
cl_mem mdl::firefly::memory::gpu_mem_alloc(void*& __mptr, std::size_t __bc, cl_mem_flags __mem_flags, cl_context __context, types::err_t& __any_err) {
# else
void mdl::firefly::memory::gpu_mem_alloc(void*& __mptr, std::size_t __bc, types::err_t& __any_err) {
# endif
	__any_err = FFLY_SUCCESS;
# ifdef __USING_CUDA
	types::cu_err_t any_err = cudaSuccess;
	if ((any_err = cudaMalloc((void **)&__mptr, __bc * sizeof(u8_t))) != cudaSuccess) {
		fprintf(stderr, "mem_alloc: gpu, cuda: failed to alloc memory, errno: %d\n", any_err);
		__any_err = FFLY_FAILURE;
		return;
	}
# endif
# ifdef __USING_OPENCL
	types::cl_err_t any_err = CL_SUCCESS;
	cl_mem mem = clCreateBuffer(__context, __mem_flags, __bc * sizeof(u8_t), __mptr, &any_err);
	if (any_err != CL_SUCCESS) {
		fprintf(stderr, "mem_alloc: gpu, opencl: failed to alloc memory, errno: %d\n", any_err);
		__any_err = FFLY_FAILURE;
		return mem;
	}
# endif
}
