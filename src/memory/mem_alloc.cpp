# include "mem_alloc.h"
# ifdef __DEBUG_ENABLED
std::size_t mdl::firefly::memory::alloc_bc;
std::size_t mdl::firefly::memory::alloc_c;
# endif

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

# ifdef __WITH_MEM_TRACKER
void* mdl::firefly::memory::mem_alloc(std::size_t __bc, bool __track_bypass) {
# else
void* mdl::firefly::memory::mem_alloc(std::size_t __bc) {
# endif
	u8_t *mem_ptr;
# ifdef __DEBUG_ENABLED
	mem_ptr = (u8_t*)malloc(__bc + sizeof(std::size_t));
	*((std::size_t *)mem_ptr) = __bc;
	alloc_bc += __bc;
	alloc_c ++;

	mem_ptr += sizeof(std::size_t);
# else
	mem_ptr = (u8_t*)malloc(__bc);
# endif
# ifdef __WITH_MEM_TRACKER
	if (!__track_bypass)
		ffly_mem_track_alloc(&__ffly_mem_track__, (void*)mem_ptr);
# endif
	return (void*)mem_ptr;
}
