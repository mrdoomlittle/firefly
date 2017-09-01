# include "draw_skelmap.hpp"
__global__ void cu_draw_skelmap(mdl::u8_t *__skelmap, mdl::u8_t *__pixbuff, mdl::uint_t *__xaxis, mdl::uint_t *__yaxis, mdl::uint_t *__pixbuff_xlen, mdl::uint_t *__scale, mdl::firefly::graphics::colour_t *__colour) {
	mdl::uint_t skel_point = threadIdx.x + (blockIdx.x * blockDim.x);

if (__skelmap[skel_point] == 0x1) {
	mdl::uint_t pix_point = (((threadIdx.x + (*__xaxis)) + ((blockIdx.x + (*__yaxis)) * (*__pixbuff_xlen))) * 4) * (*__scale);

	for (std::size_t y = 0; y != (*__scale); y ++) {
		for (std::size_t x = 0; x != (*__scale); x ++) {
			mdl::uint_t real_pp = pix_point + ((x + (y * (*__pixbuff_xlen))) * 4);
			__pixbuff[real_pp] = __colour-> r;
			__pixbuff[real_pp + 1] = __colour-> g;
			__pixbuff[real_pp + 2] = __colour-> b;
			__pixbuff[real_pp + 3] = __colour-> a;
		}
	}
}
}

mdl::i8_t mdl::firefly::graphics::draw_skelmap(u8_t *__skelmap, u8_t *__pixbuff, uint_t __xaxis, uint_t __yaxis, types::skelmap_info_t __skelmap_info, uint_t __pixbuff_xlen, uint_t __pixbuff_ylen, uint_t __scale, colour_t __colour) {
	static u8_t *skelmap = nullptr, *pixbuff = nullptr;
	static uint_t *xaxis = nullptr, *yaxis = nullptr, *pixbuff_xlen = nullptr, *scale = nullptr;
	static colour_t *colour = nullptr;
	static bool initialized = false;
	cudaError_t any_error = cudaSuccess;

	uint_t pixbuff_size = (__pixbuff_xlen * __pixbuff_ylen) * 4;

	if (pixbuff_size == 0) {
		fprintf(stderr, "error: pixbuff size is zero.\n");
		return -1;
	}

	if (!initialized) {
		if ((any_error = cudaMalloc((void **)&xaxis, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if (__xaxis == 0) {
			if ((any_error = cudaMemcpy(xaxis, &__xaxis, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return -1;	
			}
		}

		if ((any_error = cudaMalloc((void **)&yaxis, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if (__yaxis == 0) {
			if ((any_error = cudaMemcpy(yaxis, &__yaxis, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return -1;
			}
		}

		if ((any_error = cudaMalloc((void **)&pixbuff_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if ((any_error = cudaMalloc((void **)&scale, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if (__scale == 0) {
			if ((any_error = cudaMemcpy(scale, &__scale, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return -1;
			}
		}

		if ((any_error = cudaMalloc((void **)&colour, sizeof(colour_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if (__colour.r == 0 && __colour.g == 0 && __colour.b == 0 && __colour.a == 0) {
			if ((any_error = cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return -1;
			}
		}

		initialized = true;
	}

	static types::skelmap_info_t _skelmap_info = {0, 0};

	if (_skelmap_info.xaxis_len != __skelmap_info.xaxis_len || _skelmap_info.yaxis_len != __skelmap_info.yaxis_len) {
		if (skelmap != nullptr) cudaFree(skelmap);

		if ((any_error = cudaMalloc((void **)&skelmap, (__skelmap_info.xaxis_len * __skelmap_info.yaxis_len) * sizeof(u8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		_skelmap_info = __skelmap_info;
	}

	static uint_t _pixbuff_size = 0;
	static uint_t _pixbuff_xlen = 0;

	if (_pixbuff_size != pixbuff_size) {
		if (pixbuff != nullptr) cudaFree(pixbuff);

		if ((any_error = cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(u8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		if (_pixbuff_xlen != __pixbuff_xlen) {
			if ((any_error = cudaMemcpy(pixbuff_xlen, &__pixbuff_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return -1;
			}

			_pixbuff_xlen = __pixbuff_xlen;
		}

		_pixbuff_size = pixbuff_size;
	}

	if ((any_error = cudaMemcpy(skelmap, __skelmap, (__skelmap_info.xaxis_len * __skelmap_info.yaxis_len) * sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return -1;
	}

	if ((any_error = cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return -1;
	}

	static uint_t _xaxis = 0;
	if (_xaxis != __xaxis) {
		if ((any_error = cudaMemcpy(xaxis, &__xaxis, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return -1;
		}

		_xaxis = __xaxis;
	}

	static uint_t _yaxis = 0;
	if (_yaxis != __yaxis) {
		if ((any_error = cudaMemcpy(yaxis, &__yaxis, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return -1;
		}

		_yaxis = __yaxis;
	}

	static uint_t _scale = 0;
	if (_scale != __scale) {
		if ((any_error = cudaMemcpy(scale, &__scale, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return -1;
		}

		_scale = __scale;
	}

	static colour_t _colour;

	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_error = cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return -1;
		}

		_colour = __colour;
	}

	cu_draw_skelmap<<<__skelmap_info.yaxis_len, __skelmap_info.xaxis_len>>>(skelmap, pixbuff, xaxis, yaxis, pixbuff_xlen, scale, colour);

	if ((any_error = cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(u8_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return -1;
	}

	return 0;
}
