# include "window.hpp"
# include "../data/mem_set.h"
# include "../data/str_len.h"
# include "../data/mem_dupe.h"
# include "../types/event_t.h"
# include "../system/io.h"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __width, u16_t __height, char const *__title) {
	this->width = __width;
	this->height = __height;

	system::io::fprintf(ffly_log, "window, width: %u, height: %u, title: %s\n", __width, __height, __title);
	ffly_mem_dupe(const_cast<void const**>(reinterpret_cast<void**>(const_cast<char**>(&this->title))), const_cast<char*>(__title), ffly_str_len(const_cast<char*>(__title)));
	ffly_wd_init(&this->raw, __width, __height, this->title);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::de_init() {
	ffly_wd_close(&this->raw);
	ffly_wd_cleanup(&this->raw);
	return FFLY_SUCCESS;
}

mdl::firefly::types::bool_t mdl::firefly::graphics::window::poll_event(types::event_t*& __event, types::err_t& __err) {
	__err = FFLY_SUCCESS;
	return (__event = ffly_wd_poll_event(&this->raw, &__err)) != nullptr;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::free_event(types::event_t *__event) {
	ffly_wd_free_event(&this->raw, __event);
	return FFLY_SUCCESS;
}

/*
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
*/
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
