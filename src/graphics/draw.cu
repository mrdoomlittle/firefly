# include "draw.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
__global__ void static pixdraw(mdl::firefly::types::off_t __xa, mdl::firefly::types::byte_t *__buff, mdl::uint_t __bufw, mdl::firefly::types::byte_t *__pixels) {
	mdl::firefly::types::byte_t *src = __pixels+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	mdl::firefly::types::byte_t *dst = __buff+((threadIdx.x+(blockIdx.x*__bufw)+__xa)*4);

	mdl::firefly::types::byte_t alpha = *(src+A_OFF);
	mdl::firefly::types::byte_t invr_alpha = ~*(src+A_OFF);

	if (*(src+A_OFF) != 0x0) {
		mdl::firefly::types::byte_t r = (alpha**(src+R_OFF)+invr_alpha**(dst+R_OFF))>>8;
		mdl::firefly::types::byte_t g = (alpha**(src+G_OFF)+invr_alpha**(dst+G_OFF))>>8;
		mdl::firefly::types::byte_t b = (alpha**(src+B_OFF)+invr_alpha**(dst+B_OFF))>>8;
		*(dst+R_OFF) = r, *(dst+G_OFF) = g, *(dst+B_OFF) = b, *(dst+A_OFF) = *(src+A_OFF);
	}
}


mdl::firefly::types::byte_t static *buff = nullptr, *pixels = nullptr;
void static cleanup(void *__arg_p) {
	mdl::firefly::system::io::fprintf(ffly_log, "cleanup for draw.\n");
	if (buff != nullptr)
		mdl::firefly::memory::gpu_mem_free(buff);
	if (pixels != nullptr)
		mdl::firefly::memory::gpu_mem_free(pixels);
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_pixdraw(types::off_t __xa, types::off_t __ya, types::byte_t *__buff, uint_t __bufw, types::byte_t *__pixels, uint_t __width, uint_t __height) {
	cudaError_t any_err;
	types::bool_t static inited = ffly_false;
	uint_t bufsize = __bufw*__height*4;
	uint_t size = __width*__height*4;

	if (!inited) {
		if (memory::gpu_mem_alloc((void**)&buff, bufsize) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}

		if (memory::gpu_mem_alloc((void**)&pixels, size) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}

		ffly_act_add_task(&__ffly_act__, act_gid_cleanup, &cleanup, nullptr);
		inited = ffly_true;
	}

	uint_t static _bufsize = bufsize;
	if (_bufsize != bufsize) {
		if (buff != nullptr)
			mdl::firefly::memory::gpu_mem_free(buff);
		if (memory::gpu_mem_alloc((void**)&buff, bufsize) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}
		_bufsize = bufsize;
	}

	uint_t static _size = size;
	if (_size != size) {
		if (pixels != nullptr)
			mdl::firefly::memory::gpu_mem_free(pixels);
		if (memory::gpu_mem_alloc((void**)&pixels, size) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}
		_size = size;
	}

	if ((any_err = cudaMemcpy(buff, __buff, bufsize, cudaMemcpyHostToDevice)) != cudaSuccess) {
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(pixels, __pixels, size, cudaMemcpyHostToDevice)) != cudaSuccess) {
		return FFLY_FAILURE;
	}

	// for now
	pixdraw<<<__height, __width>>>(__xa, buff, __bufw, pixels);
	if ((any_err = cudaMemcpy(__buff+((__ya*__bufw)*4), buff, bufsize, cudaMemcpyDeviceToHost)) != cudaSuccess) {
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
