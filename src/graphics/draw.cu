# include "draw.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
__global__ void static pixdraw(mdl::uint_t __x, mdl::firefly::types::byte_t *__dst, mdl::uint_t __dstw, mdl::firefly::types::byte_t *__pixels) {
	mdl::firefly::types::byte_t *src = __pixels+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	mdl::firefly::types::byte_t *dst = __dst+((threadIdx.x+(blockIdx.x*__dstw)+__x)*4);

	mdl::firefly::types::byte_t alpha = *(src+A_OFF);
	mdl::firefly::types::byte_t invr_alpha = ~*(src+A_OFF);

	if (*(src+A_OFF) != 0x0) {
		mdl::firefly::types::byte_t r = (alpha**(src+R_OFF)+invr_alpha**(dst+R_OFF))>>8;
		mdl::firefly::types::byte_t g = (alpha**(src+G_OFF)+invr_alpha**(dst+G_OFF))>>8;
		mdl::firefly::types::byte_t b = (alpha**(src+B_OFF)+invr_alpha**(dst+B_OFF))>>8;
		*(dst+R_OFF) = r, *(dst+G_OFF) = g, *(dst+B_OFF) = b, *(dst+A_OFF) = *(src+A_OFF);
	}
}


mdl::firefly::types::byte_t static *dst = nullptr, *pixels = nullptr;
void static cleanup(void *__arg_p) {
	mdl::firefly::system::io::fprintf(ffly_log, "cleanup for draw.\n");
	if (dst != nullptr)
		mdl::firefly::memory::gpu_mem_free(dst);
	if (pixels != nullptr)
		mdl::firefly::memory::gpu_mem_free(pixels);
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_pixdraw(uint_t __x, uint_t __y, types::byte_t *__dst, uint_t __dstw, types::byte_t *__pixels, uint_t __width, uint_t __height) {
	types::cl_err_t err;
	types::bool_t static inited = ffly_false;
	uint_t dstsize = __dstw*__height*4;
	uint_t size = __width*__height*4;

	if (!inited) {
		if (memory::gpu_mem_alloc((void**)&dst, dstsize) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}

		if (memory::gpu_mem_alloc((void**)&pixels, size) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}

		ffly_act_add_task(&__ffly_act__, act_gid_cleanup, &cleanup, nullptr);
		inited = ffly_true;
	}

	uint_t static _dstsize = dstsize;
	if (_dstsize != dstsize) {
		if (dst != nullptr)
			mdl::firefly::memory::gpu_mem_free(dst);
		if (memory::gpu_mem_alloc((void**)&dst, dstsize) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}
		_dstsize = dstsize;
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

	if ((err = cudaMemcpy(dst, __dst, dstsize, cudaMemcpyHostToDevice)) != ffly_cl_success) {
		system::io::fprintf(ffly_err, "failed to copy dest to device, %s\n", cudaGetErrorString(err));
		return FFLY_FAILURE;
	}

	if ((err = cudaMemcpy(pixels, __pixels, size, cudaMemcpyHostToDevice)) != ffly_cl_success) {
		system::io::fprintf(ffly_err, "failed to copy pixels to device, %s\n", cudaGetErrorString(err));
		return FFLY_FAILURE;
	}

	// for now
	pixdraw<<<__height, __width>>>(__x, dst, __dstw, pixels);
	if ((err = cudaMemcpy(__dst+((__y*__dstw)*4), dst, dstsize, cudaMemcpyDeviceToHost)) != ffly_cl_success) {
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
