# include "scale_pixmap.hpp"

__global__ void cu_scale_pixmap(boost::uint8_t *__pixmap, boost::uint8_t *__pixbuff, mdl::uint_t *__scale) {
	// dont know if this way is better then copying the memory over every time
	// may change.
	mdl::uint_t pixbuff_xlen = (gridDim.x * (*__scale));
	mdl::uint_t pixmap_point = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;
	mdl::uint_t pixbuff_point = ((threadIdx.x + (blockIdx.x * pixbuff_xlen)) * 4) * (*__scale);

	for (std::size_t y = 0; y != (*__scale); y ++) {
		for (std::size_t x = 0; x != (*__scale); x ++) {
			mdl::uint_t real_point = pixbuff_point + ((x + (y * pixbuff_xlen)) * 4);

			__pixbuff[real_point] = __pixmap[pixmap_point];
			__pixbuff[real_point + 1] = __pixmap[pixmap_point + 1];
			__pixbuff[real_point + 2] = __pixmap[pixmap_point + 2];
			__pixbuff[real_point + 3] = __pixmap[pixmap_point + 3];
		}
	}
}

boost::int8_t mdl::firefly::graphics::scale_pixmap(boost::uint8_t *&__pixmap, uint_t& __pixmap_xlen, uint_t& __pixmap_ylen, uint_t __scale) {
	if (__scale == 0) {
		fprintf(stderr, "error: scale must be above 0.\n");
		return -1;
	}

	uint_t pixmap_size = (__pixmap_xlen * __pixmap_ylen) * 4;
	static uint_t _pixmap_size = 0;

	static boost::uint8_t *pixmap = nullptr, *pixbuff = nullptr;
	static uint_t new_size = 0;
	boost::uint8_t *_pixbuff = nullptr;
	cudaError_t any_error = cudaSuccess;

	if (_pixmap_size != pixmap_size) {
		if (pixmap != nullptr) cudaFree(pixmap);

		if ((any_error = cudaMalloc((void **)&pixmap, pixmap_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n");
			return -1;
		}

		uint_t new_xlen = __pixmap_xlen * __scale;
		uint_t new_ylen = __pixmap_ylen * __scale;
		new_size = (new_xlen * new_ylen) * 4;

		if (pixbuff != nullptr) cudaFree(pixbuff);

		if ((any_error = cudaMalloc((void **)&pixbuff, new_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n");
			return -1;
		}

		_pixbuff = static_cast<boost::uint8_t *>(malloc(new_size * sizeof(boost::uint8_t)));
	}

	static uint_t *scale = nullptr;
	static bool initialized = false;

	if (!initialized) {
		if ((any_error = cudaMalloc((void **)&scale, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		}

		initialized = true;
	}

	static uint_t _scale = 0;

	if (_scale != __scale) {
		if ((any_error = cudaMemcpy(scale, &__scale, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		}

		_scale = __scale;
	}


	if ((any_error = cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
		return -1;
	}

	cu_scale_pixmap<<<__pixmap_ylen, __pixmap_xlen>>>(pixmap, pixbuff, scale);

	if ((any_error = cudaMemcpy(_pixbuff, pixbuff, new_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n");
		return -1;
	}

	std::free(__pixmap);
	__pixmap = _pixbuff;
	__pixmap_xlen = __pixmap_xlen * __scale;
	__pixmap_ylen = __pixmap_ylen * __scale;

	return 0;
}
