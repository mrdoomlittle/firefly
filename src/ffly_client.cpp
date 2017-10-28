# include "ffly_client.hpp"
# include "system/errno.h"
# include "graphics/wd_flags.h"
# include "system/timer.hpp"
# include "data/mem_set.h"
mdl::firefly::types::bool_t mdl::ffly_client::to_shutdown = false;

void static ctrl_c(int __sig) {
	mdl::firefly::system::io::printf(stdout, "looks like ctrl_c was called bye bye...\n");
	mdl::ffly_client::to_shutdown = true;
}

mdl::firefly::types::err_t mdl::ffly_client::init(char const *__frame_title) {
	firefly::types::err_t any_err = FFLY_SUCCESS;
	if ((any_err = this->window.init(this->wd_xa_len, this->wd_ya_len, __frame_title)) != FFLY_SUCCESS) {
		// err
	}

	return any_err;
}

mdl::firefly::types::err_t mdl::ffly_client::de_init() {
	firefly::types::err_t any_err = FFLY_SUCCESS;
	if ((any_err = this->window.de_init()) != FFLY_SUCCESS) {
		// err
	}
	return any_err;
}

/*
mdl::firefly::types::err_t mdl::ffly_client::forward_events() {


}*/

extern "C" {
	void ffly_usleep(mdl_u64_t,  mdl_u32_t);
}

mdl::firefly::types::err_t mdl::ffly_client::begin(void(*__extern_loop)(i8_t, portal_t*, void*), void *__this) {
	firefly::types::err_t any_err = FFLY_SUCCESS;
	if ((any_err = this->window.begin()) != FFLY_SUCCESS) {
		// err
		goto err;
	}

	while(!firefly::system::is_flag(window.flags(), FFLY_FLG_WD_ALIVE));
	firefly::data::mem_set(this->window.frame_buff(), 0x0, this->wd_xa_len*this->wd_ya_len*4);

	firefly::system::timer timer;
	timer.begin();

	this->portal.init(this);

	do {
		ffly_usleep(0, 20000000);
		if (firefly::system::is_flag(window.flags(), FFLY_FLG_WD_DEAD)) {
			firefly::system::io::printf(stdout, "window has been closed.\n");
			goto _pro;
		}

		if (timer.now<firefly::system::time::ns_to_ms>() > 1000) {
			this->fps = this->frame_c;
			this->frame_c = 0;
			timer.reset();
		}

		if (!window.is_event_buff_empty()) {
			firefly::system::io::printf(stdout, "dumping window event buffer.\n");
			window.dump_event_buff();
		}

		mdl_u8_t c = 0;
		// pull max of 10 events from window at one time
		while(window.event_queue.size() > 0 && c++ < 10) {
			firefly::types::err_t any_err = FFLY_SUCCESS;
			firefly::system::event_add(window.event_queue.pop(any_err));
			firefly::system::io::printf(stdout, "forwarding window event queue to global one.\n");
			if (any_err != FFLY_SUCCESS) {
				firefly::system::io::printf(stderr, "failure.");
			}
		}

		__extern_loop(0, &this->portal, NULL);
# ifdef __WITH_LAYER_MANAGER
		this->layer_m.draw_layers(this->window.frame_buff(), this->wd_xa_len, this->wd_ya_len);
# endif
		if (!firefly::system::is_flag(window.flags(), FFLY_FLG_WD_DRAW_FRAME))
			firefly::system::add_flag(&window.flags(), FFLY_FLG_WD_DRAW_FRAME, 0);

		if (!firefly::system::is_flag(window.flags(), FFLY_FLG_WD_NEXT_FRAME)) continue;
		firefly::system::rm_flag(&window.flags(), FFLY_FLG_WD_NEXT_FRAME);
		this->frame_c++;
	} while(!to_shutdown);

	firefly::system::add_flag(&window.flags(), FFLY_FLG_WD_KILL, 0);
	while(!firefly::system::is_flag(window.flags(), FFLY_FLG_WD_DEAD));
	_pro:
	firefly::system::add_flag(&window.flags(), FFLY_FLG_WD_OK, 0);

	err:
	return any_err;
}
