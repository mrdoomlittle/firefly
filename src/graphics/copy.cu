# include "copy.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::gpu_pixcopy(types::byte_t *__dst, types::byte_t *__src, mdl_uint_t __nopix) {
	cudaError_t any_err;
	mdl_uint_t size = __nopix*4;
	cudaMemcpy(__dst, __src, size, cudaMemcpyHostToHost);
	cudaDeviceSynchronize();
	return FFLY_SUCCESS;
}
