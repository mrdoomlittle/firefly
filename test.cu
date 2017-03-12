# include <cstdint>
# include <cstdio>
__global__ void cu_test(uint8_t *__data) {
	__data[threadIdx.x] = threadIdx.x;
}

uint8_t *test() {
	uint8_t *data = (uint8_t *)malloc(8);
	uint8_t *_data;
	cudaError_t any_error = cudaSuccess;

	if ((any_error = cudaMalloc((void **)&_data, 8)) != cudaSuccess) {
		fprintf(stderr, "Malloc failed with error code: %d\n", any_error);
		return nullptr;
	}

	cu_test<<<1, 8>>>(_data);

	if ((any_error = cudaMemcpy(data, _data, 8, cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "Memcpy failed with error code: %d\n", any_error);
		return nullptr;
	}

	cudaFree(_data);

	return data;
}
