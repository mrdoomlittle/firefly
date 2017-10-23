# include "fill_pixmap.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
__global__ void cu_fill_pixmap(mdl::firefly::types::pixmap_t __pixmap, mdl::firefly::types::colour_t *__colour) {
	mdl::uint_t point = (threadIdx.x+(blockIdx.x*blockDim.x))*4;

	__pixmap[point] =__colour-> r;
	__pixmap[point+1] =__colour-> g;
	__pixmap[point+2] =__colour-> b;
	__pixmap[point+3] =__colour-> a;
}


mdl::firefly::types::err_t mdl::firefly::graphics::fill_pixmap(types::pixmap_t __pixmap, uint_t __xa_len, uint_t __ya_len, types::colour_t __colour) {
	types::pixmap_t static pixmap = nullptr;
	types::colour_t static *colour = nullptr;
	bool static inited = false;
	cudaError_t any_error = cudaSuccess;

	uint_t pixmap_size = __xa_len*__ya_len*4;
	uint_t static _pixmap_size = 0;

	if (pixmap_size == 0) {
		system::io::printf(stderr, "cuda, fill_pixmap: error pixmap size must not be zero.\n");
		return FFLY_FAILURE;
	}

	if (_pixmap_size != pixmap_size) {
		if (pixmap != nullptr) memory::gpu_mem_free(pixmap);

		if (memory::gpu_mem_alloc((void**)&pixmap, pixmap_size*sizeof(types::__pixmap_t)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "cuda, fill_pixmap: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pixmap_size = pixmap_size;
	}

	if (!inited) {
		if (memory::gpu_mem_alloc((void**)&colour, sizeof(types::colour_t)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "cuda, fill_pixmap: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if (__colour.r == 0 && __colour.g == 0 && __colour.b == 0 && __colour.a == 0) {
			if ((any_error = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "cuda, fill_pixmap: failed to call Memcpy, error code: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		inited = true;
	}

	cudaMemcpy(pixmap, __pixmap, pixmap_size*sizeof(types::__pixmap_t), cudaMemcpyHostToDevice);

	types::colour_t static _colour = {0, 0, 0, 0};
	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_error = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda, fill_pixmap: failed to call Memcpy, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_colour = __colour;
	}

	cu_fill_pixmap<<<__ya_len, __xa_len>>>(pixmap, colour);
	cudaMemcpy(__pixmap, pixmap, pixmap_size*sizeof(u8_t), cudaMemcpyDeviceToHost);
	return FFLY_SUCCESS;
}
