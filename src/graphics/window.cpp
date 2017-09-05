# include "window.hpp"
# include "../data/mem_set.h"
# include "../data/str_len.h"
# include "../data/mem_dupe.h"
# include "../system/thread.h"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __xa_len, u16_t __ya_len, char const *__title) {
	ffly_mem_set(&this->handle, 0, sizeof(struct ffly_x11_wd));
	this->xa_len = __xa_len;
	this->ya_len = __ya_len;

	ffly_mem_dupe(reinterpret_cast<void**>(const_cast<char**>(&this->title)), const_cast<char*>(__title), ffly_str_len(const_cast<char*>(__title)));
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::begin() {
	system::thread t(ffly_x11_wd_begin, &this->handle, this->xa_len, this->ya_len, this->title);
}


// testing
/*
# include "wd_flags.h"
using namespace mdl::firefly;
int main() {
	graphics::window window;
	window.init(100, 100, "To Tell The World!");

	window.begin();

	while(!ffly_is_flag(window.flags(), FLG_WD_OPEN));
	printf("window open.\n");

	while(!ffly_is_flag(window.flags(), FLG_WD_CLOSED)) {
		if (!ffly_is_flag(window.flags(), FLG_WD_WAITING)) continue;
		if (ffly_is_flag(window.flags(), FLG_WD_DONE_DRAW)) continue;

		ffly_mem_set(window.frame_buff(), 255, 100*100*4);

		ffly_add_flag(&window.flags(), FLG_WD_DONE_DRAW, 0);
		ffly_rm_flag(&window.flags(), FLG_WD_WAITING);
	}

	printf("window closed.\n");
}*/
