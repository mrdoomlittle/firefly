# include "fill_pixmap.hpp"
__global__ void cu_fill_pixmap(boost::uint8_t *__pixmap, mdl::firefly::graphics::colour_t *__colour) {
	mdl::uint_t point = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;

	__pixmap[point] =__colour-> r;
	__pixmap[point + 1] =__colour-> g;
	__pixmap[point + 2] =__colour-> b;
	__pixmap[point + 3] =__colour-> a;
}


boost::int8_t mdl::firefly::graphics::fill_pixmap(boost::uint8_t *__pixmap, uint_t __xlen, uint_t __ylen, colour_t __colour) {
	static boost::uint8_t *pixmap = nullptr;
	static colour_t *colour = nullptr;
	static bool initialized = false;
	cudaError_t any_error = cudaSuccess;

	uint_t pixmap_size = (__xlen * __ylen) * 4;
	static uint_t _pixmap_size = 0;

	if (pixmap_size == 0) {
		fprintf(stderr, "error pixmap size must not be zero.\n");
		return -1;
	}

	if (_pixmap_size != pixmap_size) {
		if (pixmap == nullptr) cudaFree(pixmap);

		if ((any_error = cudaMalloc((void **)&pixmap, pixmap_size * sizeof(boost::uint8_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, error code: %d\n", any_error);
			return -1;
		} 
	}

	if (!initialized) {
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
	}

	cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);

	static colour_t _colour = {0, 0, 0, 0};

	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_error = cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, error code: %d\n", any_error);
			return -1;
		}
	}

	cu_fill_pixmap<<<__ylen, __xlen>>>(pixmap, colour);

	cudaMemcpy(__pixmap, pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost);

	return 0;
}
