# include "draw.hpp"
__global__ void _draw_pixmap(int unsigned *__xoffset, int unsigned *__yoffset, boost::uint8_t *__pixels, boost::uint8_t *__pixmap, int unsigned *__xlen) {
	int unsigned curr_pixel = ((threadIdx.x + (*__xoffset)) + ((blockIdx.x + (*__yoffset)) * (*__xlen))) * 4;
	int unsigned pixmap_pos = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;

	__pixels[curr_pixel] = __pixmap[pixmap_pos];
	__pixels[curr_pixel + 1] = __pixmap[pixmap_pos + 1];
	__pixels[curr_pixel + 2] = __pixmap[pixmap_pos + 2];
	__pixels[curr_pixel + 3] = __pixmap[pixmap_pos + 3];
}

__global__ void _reset_pixmap(boost::uint8_t *__pixmap) {
	int unsigned pos = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;
	__pixmap[pos] = 0x0;
	__pixmap[pos + 1] = 0x0;
	__pixmap[pos + 2] = 0x0;
	__pixmap[pos + 3] = 0x0;
}

int unsigned *xoffset, *yoffset, *xlen;
boost::uint8_t *pixels, *pixmap;
void draw_pixmap(int unsigned __xoffset, int unsigned __yoffset, boost::uint8_t *__pixels, boost::uint8_t *__pixmap, int unsigned __sizes[2], int unsigned __imgsize[2], int unsigned __xlen) {
	cudaMalloc((void **)&xoffset, sizeof(int unsigned));
	cudaMalloc((void **)&yoffset, sizeof(int unsigned));
	cudaMalloc((void **)&xlen, sizeof(int unsigned));	

	cudaMalloc((void **)&pixels, __sizes[0]);
	cudaMalloc((void **)&pixmap, __sizes[1]);

	cudaMemcpy(xoffset, &__xoffset, sizeof(int unsigned), cudaMemcpyHostToDevice);
	cudaMemcpy(yoffset, &__yoffset, sizeof(int unsigned), cudaMemcpyHostToDevice);
	cudaMemcpy(xlen, &__xlen, sizeof(int unsigned), cudaMemcpyHostToDevice);

	cudaMemcpy(pixels, __pixels, __sizes[0], cudaMemcpyHostToDevice);
	cudaMemcpy(pixmap, __pixmap, __sizes[1], cudaMemcpyHostToDevice);

	_draw_pixmap<<<__imgsize[1], __imgsize[0]>>>(xoffset, yoffset, pixels, pixmap, xlen);

	cudaMemcpy(__pixels, pixels, __sizes[0], cudaMemcpyDeviceToHost);

	cudaFree(xoffset);
	cudaFree(yoffset);
	cudaFree(xlen);

	cudaFree(pixels);
	cudaFree(pixmap);
}

boost::uint8_t *bg_pixmap;
void reset_pixmap(boost::uint8_t *__pixmap, int unsigned __sizes[2]) {
	int unsigned size = (__sizes[0] * __sizes[1]) * 4;
	cudaMalloc((void **)&bg_pixmap, size);

	cudaMemcpy(bg_pixmap, __pixmap, size, cudaMemcpyHostToDevice);

	_reset_pixmap<<<__sizes[1], __sizes[0]>>>(bg_pixmap);

	cudaMemcpy(__pixmap, bg_pixmap, size, cudaMemcpyDeviceToHost);

	cudaFree(bg_pixmap);
}
