# include "draw_grid.hpp"
# define GRID_THIKNESS 8
# define GRID_CR 63
# define GRID_CG 65
# define GRID_CB 89
# define GRID_CA 255
__global__ void _draw_grid(boost::uint8_t *__pixmap, mdl::uint_t *__offsets, mdl::uint_t *xlen) {

	for (std::size_t o = (threadIdx.x * __offsets[0]); o != GRID_THIKNESS + (threadIdx.x * __offsets[0]); o ++) {
		mdl::uint_t pix_pos = (o + ((blockIdx.x * __offsets[1]) * (*xlen))) * 4;
		__pixmap[pix_pos] = GRID_CR;
		__pixmap[pix_pos + 1] = GRID_CG;
		__pixmap[pix_pos + 2] = GRID_CB;
		__pixmap[pix_pos + 3] = GRID_CA;
	}
}

# include <cstdio>
boost::uint8_t *pixmap;
mdl::uint_t *offsets, *xlen;
void draw_grid(boost::uint8_t *__pixmap, mdl::uint_t __size[2], mdl::uint_t __offsets[2]) {
	mdl::uint_t xx = GRID_THIKNESS * (__size[0] / __offsets[0]);
	mdl::uint_t yy = (__size[1] / __offsets[1]);

	mdl::uint_t size = (__size[0] * __size[1]) * 4;
	cudaMalloc((void **)&pixmap, size);
	cudaMalloc((void **)&offsets, 2 * sizeof(mdl::uint_t));
	cudaMalloc((void **)&xlen, sizeof(mdl::uint_t));

	cudaMemcpy(pixmap, __pixmap, size, cudaMemcpyHostToDevice);

	cudaMemcpy(offsets, __offsets, 2 * sizeof(mdl::uint_t), cudaMemcpyHostToDevice);

	cudaMemcpy(xlen, &__size[0], sizeof(mdl::uint_t), cudaMemcpyHostToDevice);

	_draw_grid<<<yy, xx>>>(pixmap, offsets, xlen);

	cudaMemcpy(__pixmap, pixmap, size, cudaMemcpyDeviceToHost);

	cudaFree(pixmap);
	cudaFree(offsets);
}
