# include "mem_free.h"
# include "../system/errno.h"
# ifdef __ffly_use_cuda
#	include <cuda_runtime.h>
# endif

mdl::firefly::types::err_t mdl::firefly::memory::gpu_mem_free(void *__p) {
# ifdef __ffly_use_cuda
	if (cudaFree(__p) != ffly_cl_success) return FFLY_FAILURE;
# endif
	return FFLY_SUCCESS;
}
