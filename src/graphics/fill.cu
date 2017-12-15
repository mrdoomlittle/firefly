# include "fill.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
__global__ void static pixfill(mdl::firefly::types::byte_t *__buff, mdl::firefly::types::colour_t *__colour) {
	mdl::firefly::types::byte_t *p = __buff+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	*(p+R_OFF) = __colour->r, *(p+G_OFF) = __colour->g, *(p+B_OFF) = __colour->b, *(p+A_OFF) = __colour->a;
}

mdl::firefly::types::byte_t static *buff = nullptr;
mdl::firefly::types::colour_t static *colour = nullptr;
void static cleanup(void *__arg_p) {
	mdl::firefly::system::io::fprintf(ffly_log, "cleanup for fill.\n");
	if (buff != nullptr)
		mdl::firefly::memory::gpu_mem_free(buff);
	if (colour != nullptr)
		mdl::firefly::memory::gpu_mem_free(colour);
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_pixfill(types::byte_t *__buff, mdl_uint_t __nopix, types::colour_t __colour) {
	cudaError_t any_err;
	types::bool_t static inited = ffly_false;
	mdl_uint_t size = __nopix*4;
	if (!inited) {
		if (memory::gpu_mem_alloc((void**)&colour, sizeof(types::colour_t)) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for colour.\n");
			return FFLY_FAILURE;
		}

		if (memory::gpu_mem_alloc((void**)&buff, size) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for buff.\n");
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMemcpy(buff, __buff, size, cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::fprintf(ffly_err, "cuda, failed to copy buff to device.\n");
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::fprintf(ffly_err, "cuda, failed to copy colour to device.\n");
			return FFLY_FAILURE;
		}
		ffly_act_add_task(&__ffly_act__, act_gid_cleanup, &cleanup, nullptr);
		inited = ffly_true;
	}

	mdl_uint_t static _size = size;
	if (_size != size) {
		if (buff != nullptr) memory::gpu_mem_free(buff);
		if (memory::gpu_mem_alloc((void**)&buff, size) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for buff.\n");
			return FFLY_FAILURE;
		}
		_size = size;
	}


	if ((any_err = cudaMemcpy(buff, __buff, size, cudaMemcpyHostToDevice)) != cudaSuccess) {
		system::io::fprintf(ffly_err, "cuda, failed to copy buff to device.\n");
		return FFLY_FAILURE;
	}

	types::colour_t static _colour = __colour;
	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_err = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::fprintf(ffly_err, "cuda, failed to copy colour to device.\n");
			return FFLY_FAILURE;
		}
		_colour = __colour;
	}

	mdl_uint_t blk_size;
	mdl_uint_t no_blks, left, off;

	if (!(__nopix>>8)) {
		blk_size = __nopix;
		no_blks = 1;
	} else {
		blk_size = 1<<8;
		no_blks = __nopix>>8;
	}

	system::io::fprintf(ffly_log, "no_blks: %u, blk_size: %u\n", no_blks, blk_size);
	pixfill<<<no_blks, blk_size>>>(buff, colour);
	if ((left = (__nopix-(off = (no_blks*(1<<8)))))>0 && (__nopix>>8)>0)
		pixfill<<<1, left>>>(buff+(off*4), colour);
	if ((any_err = cudaMemcpy(__buff, buff, size, cudaMemcpyDeviceToHost)) != cudaSuccess) {
		system::io::fprintf(ffly_err, "cuda, failed to copy buff from device to host.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
