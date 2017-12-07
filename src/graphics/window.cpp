# include "window.hpp"
# include "../data/mem_set.h"
# include "../data/str_len.h"
# include "../data/mem_dupe.h"
# include "../system/thread.h"
# include "../types/event_t.h"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __width, u16_t __height, char const *__title) {
	ffly_mem_set(&this->handle, 0x0, sizeof(struct ffly_x11_wd));
	this->width = __width;
	this->height = __height;

	ffly_mem_dupe(reinterpret_cast<void**>(const_cast<char**>(&this->title)), const_cast<char*>(__title), ffly_str_len(const_cast<char*>(__title)));
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::begin() {
	system::thread t(
# if defined(__USING_X11)
&ffly_x11_wd_begin
# elif defined(__USING_XCB)
&ffly_xcb_wd_begin
# else
# error "please select define __USING_X11 or __USING_XCB"
# endif
, &this->handle, &this->width, &this->height, this->title, this->tid);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::de_init() {
	system::io::printf(stdout, "window, waiting for handle thread to die.\n");
	while(!ffly_thread_dead(this->tid));
	system::io::printf(stdout, "window, handle thread is now dead.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::dump_event_buff() {
	if (this->event_queue.size() > 8) {
		system::io::printf(stdout, "window, event queue is full.\n");
		return FFLY_NOP;
	}

	ffly_buff_lock(&this->handle.event_buff);
	types::event_t *itr = (types::event_t*)ffly_buff_begin(&this->handle.event_buff);
	while(itr <= (types::event_t*)ffly_buff_end(&this->handle.event_buff))
		this->event_queue.push(*(itr++));

	ffly_buff_off_reset(&this->handle.event_buff);

	ffly_buff_unlock(&this->handle.event_buff);
	return FFLY_SUCCESS;
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
