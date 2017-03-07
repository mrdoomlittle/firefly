# include "window.hpp"

boost::int8_t mdl::firefly::graphics::window::init(uint_t __wd_xlen, uint_t __wd_ylen, char const *__frame_title) {
	if (__frame_title == nullptr) {
		fprintf(stderr, "failed to init window, because 'frame_title' is null.\n");
		return FFLY_FAILURE;
	}

	if (__wd_xlen == 0 || __wd_ylen == 0) {
		fprintf(stderr, "cant create window if xlen or ylen is not > 0\n");
		return FFLY_FAILURE;
	}

	this-> wd_xlen = __wd_xlen;
	this-> wd_ylen = __wd_ylen;
	this-> frame_title = __frame_title;

	this-> init_report = FFLY_SUCCESS;
	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::graphics::window::begin() {
	if (this-> init_report != FFLY_SUCCESS) {
		fprintf(stderr, "failed to begin window, because init failed.");
		return FFLY_FAILURE;
	}

	this-> wd_handler.begin(this-> wd_xlen, this-> wd_ylen, this-> frame_title);
}

mdl::firefly::types::pixmap_t mdl::firefly::graphics::window::get_pixbuff() {
	return this-> wd_handler.pixbuff;
}

boost::int8_t mdl::firefly::graphics::window::set_pixbuff(types::pixmap_t __pixmap, uint_t __xlen, uint_t __ylen) {
	if ((__xlen == 0 && __ylen != 0) || __xlen != 0 && __ylen == 0) return FFLY_FAILURE;

	if (draw_pixmap(0, 0, this-> get_pixbuff(), this-> wd_xlen, this-> wd_ylen, __pixmap, __xlen == 0?
		this-> wd_xlen : __xlen, __ylen == 0? this-> wd_ylen : __ylen) != FFLY_SUCCESS) return FFLY_FAILURE;

	return FFLY_SUCCESS;
}
