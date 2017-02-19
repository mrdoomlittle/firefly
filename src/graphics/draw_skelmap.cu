# include "draw_skelmap.hpp"
__global__ void cu_draw_skelmap(boost::uint8_t *__skelmap, boost::uint8_t *__pixbuff, mdl::uint_t *__xaxis, mdl::uint_t *__yaxis, mdl::uint_t *__pixbuff_xlen, mdl::uint_t *__scale, mdl::firefly::graphics::colour_t *__colour) {
	mdl::uint_t skel_point = threadIdx.x + (blockIdx.x * blockDim.x);
	if (__skelmap[skel_point] == 0x0) return;

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

void mdl::firefly::graphics::draw_skelmap(boost::uint8_t *__skelmap, boost::uint8_t *__pixbuff, uint_t __xaxis, uint_t __yaxis, types::skelmap_info_t __skelmap_info, uint_t __pixbuff_xlen, uint_t __pixbuff_ylen, uint_t __scale, colour_t __colour) {
	boost::uint8_t *skelmap, *pixbuff;
	uint_t *xaxis, *yaxis, *pixbuff_xlen, *scale;
	colour_t *colour;

	uint_t pixbuff_size = (__pixbuff_xlen * __pixbuff_ylen) * 4;
	cudaMalloc((void **)&skelmap, (__skelmap_info.xaxis_len * __skelmap_info.yaxis_len) * sizeof(boost::uint8_t));
	cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(boost::uint8_t));
	cudaMalloc((void **)&xaxis, sizeof(uint_t));
	cudaMalloc((void **)&yaxis, sizeof(uint_t));
	cudaMalloc((void **)&pixbuff_xlen, sizeof(uint_t));
	cudaMalloc((void **)&scale, sizeof(uint_t));
	cudaMalloc((void **)&colour, sizeof(colour_t));

	cudaMemcpy(skelmap, __skelmap, (__skelmap_info.xaxis_len * __skelmap_info.yaxis_len) * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);
	cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);
	cudaMemcpy(xaxis, &__xaxis, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(yaxis, &__yaxis, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(pixbuff_xlen, &__pixbuff_xlen, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(scale, &__scale, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(colour, &__colour, sizeof(colour_t), cudaMemcpyHostToDevice);

	cu_draw_skelmap<<<__skelmap_info.yaxis_len, __skelmap_info.xaxis_len>>>(skelmap, pixbuff, xaxis, yaxis, pixbuff_xlen, scale, colour);

	cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost);

	cudaFree(skelmap);
	cudaFree(pixbuff);	
	cudaFree(xaxis);
	cudaFree(yaxis);
	cudaFree(pixbuff_xlen);
	cudaFree(scale);
	cudaFree(colour);
}
