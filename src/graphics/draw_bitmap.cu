# include "draw_bitmap.hpp"
__global__ void cu_draw_bitmap(mdl::firefly::types::bitmap_t __bitmap, mdl::firefly::types::pixmap_t __pixbuff, mdl::uint_t *__pb_xlen, mdl::firefly::types::coords_t *__coords, mdl::firefly::graphics::colour_t *__colour) {
	mdl::uint_t bit_point = threadIdx.x + (blockIdx.x * blockDim.x);
	mdl::uint_t pix_point = ((threadIdx.x + __coords-> xaxis) + ((blockIdx.x + __coords-> yaxis) * (*__pb_xlen))) * 4;

	__pixbuff[pix_point] = __colour-> r * __bitmap[bit_point] / 255;
	__pixbuff[pix_point + 1] = __colour-> g * __bitmap[bit_point] / 255;
	__pixbuff[pix_point + 2] = __colour-> b * __bitmap[bit_point] / 255;
	__pixbuff[pix_point + 3] = __colour-> a * __bitmap[bit_point] / 255;
}

boost::int8_t mdl::firefly::graphics::draw_bitmap(types::bitmap_t __bitmap, uint_t __bm_xlen, uint_t __bm_ylen, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::coords_t __coords, colour_t __colour) {
	static types::bitmap_t bitmap = nullptr;
	static types::pixmap_t pixbuff = nullptr;
	static uint_t *pb_xlen = nullptr;
	static types::coords_t *coords = nullptr;
	static colour_t *colour = nullptr;
	static bool initialized = false;
	cudaError_t any_error = cudaSuccess;

	if (!initialized) {
		if ((any_error = cudaMalloc((void **)&pb_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&coords, sizeof(types::coords_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}


		if ((any_error = cudaMalloc((void **)&colour, sizeof(colour_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}


		if (__pb_xlen == 0) {
			if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}


		if (__coords.xaxis == 0 && __coords.yaxis == 0) {
			if ((any_error = cudaMemcpy(coords, &__coords, sizeof(types::coords_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__colour.r == 0 && __colour.g == 0 && __colour.b == 0 && __colour.a == 0) {
			if ((any_error = cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		initialized = true;
	}

	uint_t bitmap_size = __bm_xlen * __bm_ylen;
	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;

	static uint_t _pb_xlen = 0;
	if (_pb_xlen != __pb_xlen) {
		if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pb_xlen = __pb_xlen;
	}

	static types::coords_t _coords = {
		.xaxis = 0,
		.yaxis = 0
	};
	if (_coords.xaxis != __coords.xaxis || _coords.yaxis != __coords.yaxis) {
		if ((any_error = cudaMemcpy(coords, &__coords, sizeof(types::coords_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_coords = __coords;
	}


	static colour_t _colour = {
		.r = 0,
		.g = 0,
		.b = 0,
		.a = 0
	};
	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_error = cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_colour = __colour;
	}

	static uint_t _bitmap_size = 0;
	if (_bitmap_size != bitmap_size) {
		if (bitmap != nullptr) {
			if ((any_error = cudaFree(bitmap)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call cudaFree, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}
		if ((any_error = cudaMalloc((void **)&bitmap, bitmap_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_bitmap_size = bitmap_size;
	}

	static uint_t _pixbuff_size = 0;
	if (_pixbuff_size != pixbuff_size) {
		if (pixbuff != nullptr) { 
			if ((any_error = cudaFree(pixbuff)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call cudaFree, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}
		if ((any_error = cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pixbuff_size = pixbuff_size;
	}

	if ((any_error = cudaMemcpy(bitmap, __bitmap, bitmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}

	cu_draw_bitmap<<<__bm_ylen, __bm_xlen>>>(bitmap, pixbuff, pb_xlen, coords, colour);

	if ((any_error = cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}	

	return FFLY_SUCCESS;
}
