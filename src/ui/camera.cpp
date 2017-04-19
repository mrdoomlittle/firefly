# include "camera.hpp"
mdl::firefly::types::err_t mdl::firefly::ui::camera::init(uint_t __xaxis_len, uint_t __yaxis_len) {
	if ((this-> pixmap = memory::alloc_pixmap(__xaxis_len, __yaxis_len)) == NULL) {
		fprintf(stderr, "camera: failed to alloc memory for pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> xaxis = this-> yaxis = this-> zaxis = 0;
	this-> xaxis_len = __xaxis_len;
	this-> yaxis_len = __yaxis_len;
	if ((this-> _2d_pm = memory::make_2d_pm(this-> pixmap, __xaxis_len, __yaxis_len)) == NULL) {
		fprintf(stderr, "camera: failed to make 2d pixmap from 1d pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> inited = true;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::de_init() {
	memory::mem_free(this-> pixmap);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::handle() {
//	printf("%d - %d - %d - %d - %d\n", this-> xaxis, this-> yaxis, this-> zaxis, this-> xaxis_len, this-> yaxis_len);
	types::pixmap_t chunk_pm = this-> _uni_manager-> chunk_pixmap(types::__coords__<>(this-> xaxis, this-> yaxis, this-> zaxis));

	uint_t cnk_xlen = this-> _uni_manager-> _chunk_manager-> get_chunk_xlen(), cnk_ylen = this-> _uni_manager-> _chunk_manager-> get_chunk_ylen();

	uint_t cxfs = floor(this-> xaxis / cnk_xlen) * cnk_xlen;
	uint_t cyfs = floor(this-> yaxis / cnk_ylen) * cnk_ylen;

	uint_t xaxis = this-> xaxis - cxfs, yaxis = this-> yaxis - cyfs;

	uint_t xt{}, yt{};
	if (xaxis + this-> xaxis_len > cxfs + cnk_xlen)
		xt = (xaxis + this-> xaxis_len) - (cxfs + cnk_xlen);
	if (yaxis + this-> yaxis_len > cyfs + cnk_ylen)
		yt = (yaxis + this-> yaxis_len) - (cyfs + cnk_ylen);

	if (graphics::crop_pixmap(xaxis, yaxis, this-> pixmap, this-> xaxis_len - xt, this-> yaxis_len - yt, chunk_pm,
		this-> _uni_manager-> _chunk_manager-> get_chunk_xlen(), this-> _uni_manager-> _chunk_manager-> get_chunk_ylen(), 4) == FFLY_FAILURE) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::draw_camera(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen) {
	if (graphics::draw_pixmap(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, this-> pixmap, this-> xaxis_len, this-> yaxis_len) == FFLY_FAILURE)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
