# include "fill.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
__global__ void static fill_frame(mdl::firefly::types::byte_t *__frame, mdl::firefly::types::colour_t *__colour) {
	mdl::firefly::types::byte_t *p = __frame+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	*(p+R_OFF) = __colour->r, *(p+G_OFF) = __colour->g, *(p+B_OFF) = __colour->b, *(p+A_OFF) = __colour->a;
}

mdl::firefly::types::byte_t static *frame;
mdl::firefly::types::colour_t static *colour;
void static cleanup(void *__arg_p) {
	mdl::firefly::memory::gpu_mem_free(frame);
	mdl::firefly::memory::gpu_mem_free(colour);
}

mdl::firefly::types::err_t mdl::firefly::graphics::fill_frame(types::byte_t *__frame, mdl_uint_t __width, mdl_uint_t __height, types::colour_t __colour) {
	cudaError_t any_err;
	types::bool_t inited = ffly_false;
	mdl_uint_t size = __width*__height*4;
	if (!inited) {
		if (memory::gpu_mem_alloc((void**)&colour, sizeof(types::colour_t)) != FFLY_SUCCESS) {
			system::io::printf(ffly_err, "failed to allocate memory for colour.\n");
			return FFLY_FAILURE;
		}

		if (memory::gpu_mem_alloc((void**)&frame, size*sizeof(types::byte_t)) != FFLY_SUCCESS) {
			system::io::printf(ffly_err, "failed to allocate memory for frame.\n");
			return FFLY_FAILURE;
		}

		cudaMemcpy(frame, __frame, size*sizeof(types::byte_t), cudaMemcpyHostToDevice);
		if ((any_err = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(ffly_err, "failed to copy colour to device.\n");
			return FFLY_FAILURE;
		}

		ffly_act_add_task(&__ffly_act__, act_gid_cleanup, &cleanup, NULL);
		inited = ffly_true;
	}

	mdl_uint_t static _size = size;
	if (_size != size) {
		if (frame != nullptr) memory::gpu_mem_free(frame);
		if (memory::gpu_mem_alloc((void**)&frame, size*sizeof(types::byte_t)) != FFLY_SUCCESS) {
			system::io::printf(ffly_err, "failed to allocate memory for frame.\n");
			return FFLY_FAILURE;
		}
		_size = size;
	}

	types::colour_t static _colour = __colour;
	cudaMemcpy(frame, __frame, size*sizeof(types::byte_t), cudaMemcpyHostToDevice);
	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_err = cudaMemcpy(colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(ffly_err, "failed to copy colour to device.\n");
			return FFLY_FAILURE;
		}
		_colour = __colour;
	}

	fill_frame<<<__height, __width>>>(frame, colour);
	cudaMemcpy(__frame, frame, size*sizeof(types::byte_t), cudaMemcpyDeviceToHost);
	return FFLY_SUCCESS;
}
