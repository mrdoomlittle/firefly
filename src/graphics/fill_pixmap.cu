# include "fill_pixmap.hpp"
__global__ void cu_fill_pixmap(boost::uint8_t *__pixmap, mdl::firefly::graphics::colour_t *__colour) {
	mdl::uint_t point = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;

	__pixmap[point] =__colour-> r;
	__pixmap[point + 1] =__colour-> g;
	__pixmap[point + 2] =__colour-> b;
	__pixmap[point + 3] =__colour-> a;
}


void mdl::firefly::graphics::fill_pixmap(boost::uint8_t *__pixmap, uint_t __xlen, uint_t __ylen, colour_t __colour) {
	boost::uint8_t *pixmap;
	colour_t *colour;
	uint_t pixmap_size = (__xlen * __ylen) * 4;

	cudaMalloc((void **)&pixmap, pixmap_size * sizeof(boost::uint8_t));
	cudaMalloc((void **)&colour, sizeof(colour_t));


	cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);
	cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice);

	cu_fill_pixmap<<<__ylen, __xlen>>>(pixmap, colour);

	cudaMemcpy(__pixmap, pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost);

	cudaFree(pixmap);
	cudaFree(colour);
}
