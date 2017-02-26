# include "draw_rect.hpp"
__global__ void cu_draw_rect(boost::uint8_t *__pixbuff, mdl::uint_t *__b_xcoord, mdl::uint_t *__b_ycoord, mdl::firefly::graphics::colour_t *__colour, mdl::uint_t *__pd_xlen) {
	mdl::uint_t point = ((threadIdx.x + (*__b_xcoord)) + ((blockIdx.x + (*__b_ycoord)) * (*__pd_xlen))) * 4;

	__pixbuff[point] = __colour-> r;
	__pixbuff[point + 1] = __colour-> g;
	__pixbuff[point + 2] = __colour-> b;
	__pixbuff[point + 3] = __colour-> a;
}


boost::uint8_t *pixbuff;
mdl::uint_t *b_xcoord, *b_ycoord, *pd_xlen;
mdl::firefly::graphics::colour_t *colour;
boost::int8_t mdl::firefly::graphics::draw_rect(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pd_xlen, uint_t __pd_ylen) {
	uint_t pixbuff_size = (__pd_xlen * __pd_ylen) * 4;

	cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(boost::uint8_t));
	cudaMalloc((void **)&b_xcoord, sizeof(uint_t));
	cudaMalloc((void **)&b_ycoord, sizeof(uint_t));
	cudaMalloc((void **)&colour, sizeof(colour_t));
	cudaMalloc((void **)&pd_xlen, sizeof(uint_t));

	cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);
	cudaMemcpy(b_xcoord, &__b_xcoord, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(b_ycoord, &__b_ycoord, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice);
	cudaMemcpy(pd_xlen, &__pd_xlen, sizeof(uint_t), cudaMemcpyHostToDevice);

	cu_draw_rect<<<(__e_ycoord - __b_ycoord), (__e_xcoord - __b_xcoord)>>>(
		pixbuff,
		b_xcoord,
		b_ycoord,
		colour,
		pd_xlen
	);

	cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost);

	cudaFree(pixbuff);
	cudaFree(b_xcoord);
	cudaFree(b_ycoord);
	cudaFree(colour);
	cudaFree(pd_xlen);
}
