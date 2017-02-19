# include "draw_pixmap.hpp"
__global__ void cu_draw_pixmap(mdl::uint_t *__xoffset, mdl::uint_t *__yoffset, boost::uint8_t *__pixbuff, mdl::uint_t *__pb_xlen, boost::uint8_t *__pixmap) {
	mdl::uint_t pixbuff_pos = ((threadIdx.x + (*__xoffset)) + ((blockIdx.x + (*__yoffset)) * (*__pb_xlen))) * 4;
	mdl::uint_t pixmap_pos = (threadIdx.x + (blockIdx.x * blockDim.x)) * 4;

	boost::uint8_t alpha = __pixmap[pixmap_pos + 3] + 1;
	boost::uint8_t inv_alpha = 256 - __pixmap[pixmap_pos + 3];
 
	if (__pixmap[pixmap_pos + 3] == 0) return;

	boost::uint8_t new_r = (boost::uint8_t)((alpha * __pixmap[pixmap_pos] + inv_alpha * __pixbuff[pixbuff_pos]) >> 8);
	boost::uint8_t new_g = (boost::uint8_t)((alpha * __pixmap[pixmap_pos + 1] + inv_alpha * __pixbuff[pixbuff_pos + 1]) >> 8);
	boost::uint8_t new_b = (boost::uint8_t)((alpha * __pixmap[pixmap_pos + 2] + inv_alpha * __pixbuff[pixbuff_pos + 2]) >> 8);

	__pixbuff[pixbuff_pos] = new_r;
	__pixbuff[pixbuff_pos + 1] = new_g;
	__pixbuff[pixbuff_pos + 2] = new_b;

	__pixbuff[pixbuff_pos + 3] = __pixmap[pixmap_pos + 3];
}

void mdl::firefly::graphics::draw_pixmap(uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, boost::uint8_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen) {
	uint_t *xoffset, *yoffset, *pb_xlen;
	boost::uint8_t *pixbuff, *pixmap;

	cudaMalloc((void **)&xoffset, sizeof(uint_t));
	cudaMalloc((void **)&yoffset, sizeof(uint_t));
	cudaMalloc((void **)&pb_xlen, sizeof(uint_t));

	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;
	uint_t	pixmap_size = (__pm_xlen * __pm_ylen) * 4;

	cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(boost::uint8_t));
	cudaMalloc((void **)&pixmap, pixmap_size * sizeof(boost::uint8_t));

	cudaMemcpy(xoffset, &__xoffset, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(yoffset, &__xoffset, sizeof(uint_t), cudaMemcpyHostToDevice);
	cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice);

	cudaMemcpy(pixbuff, __pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);
	cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(boost::uint8_t), cudaMemcpyHostToDevice);

	cu_draw_pixmap<<<__pm_ylen, __pm_xlen>>>(xoffset, yoffset, pixbuff, pb_xlen, pixmap);

	cudaMemcpy(__pixbuff, pixbuff, pixbuff_size * sizeof(boost::uint8_t), cudaMemcpyDeviceToHost);

	cudaFree(xoffset);
	cudaFree(yoffset);
	cudaFree(pb_xlen);
	cudaFree(pixbuff);
	cudaFree(pixmap);
}
