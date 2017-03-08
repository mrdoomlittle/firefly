# include "draw_pixmap.hpp"
__global__ void cu_draw_pixmap(mdl::uint_t *__xoffset, mdl::uint_t *__yoffset, boost::uint8_t *__pixbuff, mdl::uint_t *__pb_xlen, boost::uint8_t *__pixmap) {
	mdl::uint_t pixbuff_pos = ((threadIdx.x + (*__xoffset)) + ((blockIdx.x + (*__yoffset)) * (*__pb_xlen))) * 4;
	mdl::uint_t pixmap_pos = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;

	boost::uint8_t alpha = __pixmap[pixmap_pos + 3];
	boost::uint8_t inv_alpha = 255 - __pixmap[pixmap_pos + 3];
 
	if (__pixmap[pixmap_pos + 3] != 0x0) {

	boost::uint8_t new_r = (boost::uint8_t)((alpha * __pixmap[pixmap_pos] + inv_alpha * __pixbuff[pixbuff_pos]) >> 8);
	boost::uint8_t new_g = (boost::uint8_t)((alpha * __pixmap[pixmap_pos + 1] + inv_alpha * __pixbuff[pixbuff_pos + 1]) >> 8);
	boost::uint8_t new_b = (boost::uint8_t)((alpha * __pixmap[pixmap_pos + 2] + inv_alpha * __pixbuff[pixbuff_pos + 2]) >> 8);

	__pixbuff[pixbuff_pos] = new_r;
	__pixbuff[pixbuff_pos + 1] = new_g;
	__pixbuff[pixbuff_pos + 2] = new_b;

	__pixbuff[pixbuff_pos + 3] = __pixmap[pixmap_pos + 3];

	}
}

// NOTE: not thread safe
boost::int8_t mdl::firefly::graphics::draw_pixmap(uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, boost::uint8_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen) {
	static uint_t *xoffset = nullptr, *yoffset = nullptr, *pb_xlen = nullptr;
	static boost::uint8_t *pixbuff = nullptr, *pixmap = nullptr;
	static bool initialized = false;
	cudaError_t any_error = cudaSuccess;

	if (!initialized) {
		if ((any_error = cudaMalloc((void **)&xoffset, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}
	
		if ((any_error = cudaMalloc((void **)&yoffset, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&pb_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		// if the any of thease are 0 then it will skip the memcpy so in the kernel we could end up having some junk memory
		// that might cause an error.
		if (__xoffset == 0) {
			if ((any_error = cudaMemcpy(xoffset, &__xoffset, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__yoffset == 0) {
			if ((any_error = cudaMemcpy(yoffset, &__yoffset, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__pm_xlen == 0) {
			if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		initialized = true;
	}

	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;
	uint_t pixmap_size = (__pm_xlen * __pm_ylen) * 4;

	static uint_t _pixbuff_size = 0;

	if (_pixbuff_size != pixbuff_size) {
		if (pixbuff != nullptr) cudaFree(pixbuff);

		if ((any_error = cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pixbuff_size = pixbuff_size;
	}

	static uint_t _pixmap_size = 0;

	if (_pixmap_size != pixmap_size) {
		if (pixmap != nullptr) cudaFree(pixmap);

		if ((any_error = cudaMalloc((void **)&pixmap, pixmap_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pixmap_size = pixmap_size;
	}

	static uint_t _xoffset = 0;
	
	if (_xoffset != __xoffset) {
		if ((any_error = cudaMemcpy(xoffset, &__xoffset, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_xoffset = __xoffset;
	}


	static uint_t _yoffset = 0;

	if (_yoffset != __yoffset) {
		if ((any_error = cudaMemcpy(yoffset, &__yoffset, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_yoffset = __yoffset;
	}


	static uint_t _pb_xlen = 0;

	if (_pb_xlen != __pb_xlen) { 
		if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_pb_xlen = __pb_xlen;
	}

	if ((any_error = cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

	cu_draw_pixmap<<<__pm_ylen, __pm_xlen>>>(xoffset, yoffset, pixbuff, pb_xlen, pixmap);

	if ((any_error = cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}
