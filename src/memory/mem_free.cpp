# include "mem_free.h"
# include "../system/errno.h"
# ifdef __USING_CUDA
#	include <cuda_runtime.h>
# endif

mdl::firefly::types::err_t mdl::firefly::memory::gpu_mem_free(void *__p) {
# ifdef __USING_CUDA
	if (cudaFree(__p) != cudaSuccess) return FFLY_FAILURE;
# endif
	return FFLY_SUCCESS;
}
