# include "window.hpp"
# include "../data/mem_set.h"
# include "../data/str_len.h"
# include "../data/mem_dupe.h"
# include "../system/thread.h"
# include "../types/event_t.h"
# include "../system/io.h"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __width, u16_t __height, char const *__title) {
	this->width = __width;
	this->height = __height;

	system::io::fprintf(ffly_log, "window, width: %u, height: %u, title: %s\n", __width, __height, __title);
	ffly_mem_dupe(reinterpret_cast<void**>(const_cast<char**>(&this->title)), const_cast<char*>(__title), ffly_str_len(const_cast<char*>(__title)));
	ffly_wd_init(&this->raw, __width, __height, this->title);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::kill() {
	system::add_flag(&this->flags(), FF_FLG_WD_KILL, 0);
	while(!system::is_flag(this->flags(), FF_FLG_WD_DEAD));
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::begin() {
	ffly_wd_open(&this->raw);
	system::thread t(&ffly_wd_begin, &this->raw, this->tid);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::de_init() {
	system::io::fprintf(stdout, "window, waiting for handle thread to die.\n");
	while(!ffly_thread_dead(this->tid));
	system::io::fprintf(stdout, "window, handle thread is now dead.\n");
	ffly_wd_close(&this->raw);
	ffly_wd_cleanup(&this->raw);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::dump_event_buff() {
	if (this->event_queue.size() > 8) {
		system::io::fprintf(stdout, "window, event queue is full.\n");
		return FFLY_NOP;
	}

	ffly_buff_lock(&this->raw.event_buff);
	types::event_t *itr = (types::event_t*)ffly_buff_begin(&this->raw.event_buff);
	while(itr <= (types::event_t*)ffly_buff_end(&this->raw.event_buff))
		this->event_queue.push(*(itr++));

	ffly_buff_off_reset(&this->raw.event_buff);

	ffly_buff_unlock(&this->raw.event_buff);
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
