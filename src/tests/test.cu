# include <cstdint>
# include <cstdio>
__global__ void test(uint8_t *__ptr, size_t __pitch) {
	int x = threadIdx.x;
	uint8_t *row = (uint8_t*)((char *)__ptr + x * __pitch);
	row[13] = 'X';
/*
	for (int o = 0; o != 10240; o ++) {
		if (*(__ptr + o) == 'K') {
			printf("%d offset\n", o);
			break;
		}
	}
*/
}

# include <cstdlib>
# include <iostream>
int main() {
	int unsigned xlen = 20, ylen = 20;

	uint8_t *large_data = (uint8_t*)malloc(xlen * ylen * sizeof(uint8_t));

	uint8_t **host_data = (uint8_t **)malloc(ylen * sizeof(uint8_t *));
	//uint8_t large_data[ylen * xlen + 200];
//	uint8_t host_data[ylen][xlen];
	for (int unsigned y{}; y != ylen; y ++) {
		host_data[y] = &large_data[y * xlen];
		for (int unsigned x{}; x != xlen; x ++) {
			host_data[y][x] = ' ';
		}
	}

	uint8_t *device_data = nullptr;

	size_t pitch;

	cudaMallocPitch((void **)&device_data, &pitch, xlen * sizeof(uint8_t) - 2, ylen);

	printf("pitch: %d\n", pitch);
	if (cudaMemcpy2D(device_data, pitch, host_data[0], xlen * sizeof(uint8_t), xlen * sizeof(uint8_t) - 2, ylen, cudaMemcpyHostToDevice) != cudaSuccess){
		fprintf(stderr, "failed to copy memory to device.\n");
		return 1;
	}

	test<<<1, xlen>>>(device_data, pitch);
	cudaThreadSynchronize();

	if (cudaMemcpy2D(host_data[0], xlen * sizeof(uint8_t), device_data, pitch, xlen * sizeof(uint8_t) - 2, ylen, cudaMemcpyDeviceToHost) != cudaSuccess) {
		fprintf(stderr, "failed to copy memory to host.\n");
		return 1;
	}

	for (int unsigned y{}; y != ylen; y ++) {
		for (int unsigned x{}; x != xlen; x ++)
			printf("%c", host_data[y][x]);
		printf("\n");
	}

	cudaDeviceReset();

	std::free(large_data);
	std::free(host_data);
	cudaFree(device_data);
}

