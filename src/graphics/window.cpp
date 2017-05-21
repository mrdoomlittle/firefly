# include "window.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __wd_xaxis_len, u16_t __wd_yaxis_len, char const *__frame_title) {
	if (__frame_title == nullptr) {
		fprintf(stderr, "failed to init window, because 'frame_title' is null.\n");
		return FFLY_FAILURE;
	}

	if (__wd_xaxis_len == 0 || __wd_yaxis_len == 0) {
		fprintf(stderr, "cant create window if xlen or ylen is not > 0\n");
		return FFLY_FAILURE;
	}

	this-> wd_handler.ev_queue = &this-> ev_queue;
	this-> wd_xaxis_len = __wd_xaxis_len;
	this-> wd_yaxis_len = __wd_yaxis_len;
	this-> frame_title = __frame_title;

	this-> init_report = FFLY_SUCCESS;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::begin() {
	if (this-> init_report != FFLY_SUCCESS) {
		fprintf(stderr, "window: failed to begin window, init failed.");
		return FFLY_FAILURE;
	}

	types::err_t any_err;
	if ((any_err = this-> wd_handler.open_in_thread(this-> wd_xaxis_len, this-> wd_yaxis_len, this-> frame_title)) != FFLY_SUCCESS) {
		fprintf(stderr, "window: failed to open window, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::pixmap_t mdl::firefly::graphics::window::get_pixbuff() {
	return this-> wd_handler.pixbuff;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::set_pixbuff(types::pixmap_t __pixmap, u16_t __xaxis_len, u16_t __yaxis_len) {
	if ((__xaxis_len == 0 && __yaxis_len != 0) || (__xaxis_len != 0 && __yaxis_len == 0)) return FFLY_FAILURE;
	if (draw_pixmap(0, 0, this-> get_pixbuff(), this-> wd_xaxis_len, this-> wd_yaxis_len, __pixmap, __xaxis_len == 0?
		this-> wd_xaxis_len : __xaxis_len, __yaxis_len == 0? this-> wd_yaxis_len : __yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
/*
int main() {
	mdl::firefly::graphics::window window;
	window.init(640, 640, "Hello World");
	window.begin();


	usleep(1000000);
	window.de_init();
	printf("finished.\n");
	return 0;
}*/
