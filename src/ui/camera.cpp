# include "camera.hpp"
mdl::firefly::types::err_t mdl::firefly::ui::camera::init(uint_t __xaxis_len, uint_t __yaxis_len) {
	if ((this-> pixmap = memory::alloc_pixmap(__xaxis_len, __yaxis_len)) == NULL) {
		fprintf(stderr, "camera: failed to alloc memory for pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> xaxis_len = __xaxis_len;
	this-> yaxis_len = __yaxis_len;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::de_init() {
	memory::mem_free(this-> pixmap);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::handle() {
	types::pixmap_t chunk_pm = this-> _uni_manager-> chunk_pixmap(types::__coords__<>(this-> xaxis, this-> yaxis, this-> zaxis));
	if (graphics::crop_pixmap(this-> xaxis, this-> yaxis, this-> pixmap, this-> xaxis_len, this-> yaxis_len, chunk_pm,
		this-> _uni_manager-> _chunk_manager-> get_chunk_xlen(), this-> _uni_manager-> _chunk_manager-> get_chunk_ylen(), 4) == FFLY_FAILURE) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::ui::camera::draw_camera(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen) {
	if (graphics::draw_pixmap(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, this-> pixmap, this-> xaxis_len, this-> yaxis_len) == FFLY_FAILURE)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
