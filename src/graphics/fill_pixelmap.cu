# include "fill_pixelmap.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/errno.h"
# include "../ffly_def.h"
# include "../system/io.h"
# include "../types/byte_t.h"
# include "colour_attr.hpp"
__global__ void static fill_pixelmap(mdl::firefly::types::pixelmap_t __pixelmap, mdl::firefly::types::colour_t *__colour) {
	mdl::firefly::types::byte_t *p = __pixelmap+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	*(p+R_OFF) = __colour->r, *(p+G_OFF) = __colour->g, *(p+B_OFF) = __colour->b, *(p+A_OFF) = __colour->a;
}

void mdl::firefly::graphics::gpu_fill_pixelmap(gpu_fl_pm_mem& __mem, types::pixelmap_t __pixelmap, mdl_uint_t __xa_len, mdl_uint_t __ya_len, types::colour_t __colour) {
	mdl_uint_t pixelmap_size = __xa_len*__ya_len*4;
	cudaError_t any_err;

	if (!__mem.inited) {
		printf("init.\n");
		__mem.pixelmap_size = pixelmap_size;
		if (memory::gpu_mem_alloc((void**)&__mem.colour, sizeof(types::colour_t)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "cuda failed.\n");
			return;
		}		

		if (memory::gpu_mem_alloc((void**)&__mem.pixelmap, pixelmap_size*sizeof(types::__pixelmap_t)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "cuda failed.\n");
			return;
		}

		__mem.inited = ffly_true;
	}

	if (__mem.pixelmap_size != pixelmap_size) {
		if (__mem.pixelmap != nullptr) memory::gpu_mem_free(__mem.pixelmap);
		if (memory::gpu_mem_alloc((void**)&__mem.pixelmap, pixelmap_size*sizeof(types::__pixelmap_t)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "cuda failed.\n");
			return;
		}
		__mem.pixelmap_size = pixelmap_size;
	}

	cudaMemcpy(__mem.pixelmap, __pixelmap, pixelmap_size*sizeof(types::__pixelmap_t), cudaMemcpyHostToDevice);
	if (__mem._colour.r != __colour.r || __mem._colour.g != __colour.g || __mem._colour.b != __colour.b || __mem._colour.a != __colour.a) {
		if ((any_err = cudaMemcpy(__mem.colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda failed.\n");
			return;
		}
		__mem._colour = __colour;
	}

	fill_pixelmap<<<__ya_len, __xa_len>>>(__mem.pixelmap, __mem.colour);
	cudaMemcpy(__pixelmap, __mem.pixelmap, pixelmap_size*sizeof(types::__pixelmap_t), cudaMemcpyDeviceToHost);
}
