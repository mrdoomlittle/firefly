# include "window.hpp"
# include "../data/mem_set.h"
# include "../data/str_dupe.h"
# include "../types/event_t.h"
# include "../system/io.h"
# include "../system/err.h"
mdl::firefly::types::err_t mdl::firefly::graphics::window::init(u16_t __width, u16_t __height, char const *__title) {
	this->width = __width;
	this->height = __height;

	system::io::fprintf(ffly_log, "window, width: %u, height: %u, title: %s\n", __width, __height, __title);
	this->title = ffly_str_dupe(const_cast<char*>(__title));
	types::err_t err;
	if (_err(err = ffly_wd_init(&this->raw, __width, __height, this->title)))
		return err;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::de_init() {
	types::err_t err;
	if (_err(err = ffly_wd_close(&this->raw)))
		return err;
	if (_err(err = ffly_wd_cleanup(&this->raw)))
		return err;
	return FFLY_SUCCESS;
}

mdl::firefly::types::bool_t mdl::firefly::graphics::window::poll_event(types::event_t*& __event, types::err_t& __err) {
	__err = FFLY_SUCCESS;
	return (__event = ffly_wd_poll_event(&this->raw, &__err)) != nullptr;
}

mdl::firefly::types::err_t mdl::firefly::graphics::window::free_event(types::event_t *__event) {
	types::err_t err;
	if (_err(err = ffly_wd_free_event(&this->raw, __event)))
		return err;
	return FFLY_SUCCESS;
}
