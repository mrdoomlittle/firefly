# include "ffly_client.hpp"
# include "system/errno.h"
# include "graphics/wd_flags.h"
# include "system/timer.hpp"
# include "data/mem_set.h"
# include "system/io.h"
# include "firefly.hpp"
# include "system/err.h"
mdl::firefly::types::bool_t mdl::ffly_client::to_shutdown = false;

void static ctrl_c(int __sig) {
	mdl::firefly::system::io::fprintf(ffly_out, "looks like ctrl_c was called bye bye...\n");
	mdl::ffly_client::to_shutdown = true;
}

mdl::firefly::types::err_t mdl::ffly_client::init(u16_t __wd_width, u16_t __wd_height, char const *__wd_title) {
	firefly::types::err_t err;
	firefly::system::io::fprintf(ffly_log, "initing engine.\n");
	if (_err(err = firefly::init())) {
		firefly::system::io::fprintf(ffly_err, "failed to init engine.\n");
		return err;
	} else
		firefly::system::io::fprintf(ffly_log, "successfully inited engine.\n");

	firefly::system::io::fprintf(ffly_log, "initing window.\n");
	if (_err(err = this->window.init(__wd_width, __wd_height, __wd_title))) {
		firefly::system::io::fprintf(ffly_err, "failed to init window.\n");
		return err;
	} else
		firefly::system::io::fprintf(ffly_log, "successfully inited window.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_client::de_init() {
	firefly::types::err_t err;
	firefly::system::io::fprintf(ffly_log, "de-initing window.\n");
	if (_err(err = this->window.de_init())) {
		firefly::system::io::fprintf(ffly_err, "failed to de-init window.\n");
		return err;
	} else
		firefly::system::io::fprintf(ffly_log, "no problem occurred while de-inited window.\n");

	firefly::system::io::fprintf(ffly_log, "de-initing engine.\n");
	if (_err(err = firefly::de_init())) {
		firefly::system::io::fprintf(ffly_err, "failed to de-init engine.\n");
		return err;
	} /*else
		firefly::system::io::fprintf(ffly_log, "no problem occurred while de-inited engine.\n");*/
	return FFLY_SUCCESS;
}

/*
mdl::firefly::types::err_t mdl::ffly_client::forward_events() {


}*/
# include "graphics/pipe.h"
# include "system/nanosleep.h"
mdl::firefly::types::err_t mdl::ffly_client::begin(ffly_err_t(*__extern_loop)(i8_t, portal_t*, void*), void *__this) {
	firefly::types::err_t err = FFLY_SUCCESS;
	if (_err(err = this->window.begin()) != FFLY_SUCCESS) {
		//err
		goto _fatal;
	}

	while(!window.is_alive());
	firefly::data::mem_set(this->window.frame_buff(), 0x0, this->window.get_width()*this->window.get_height()*4);

	firefly::system::timer timer;
	timer.begin();

	this->portal.init(this);

	do {
		ffly_nanosleep(0, 20000000);
		if (firefly::system::is_flag(window.flags(), FF_FLG_WD_DEAD)) {
			firefly::system::io::fprintf(ffly_log, "window has been closed.\n");
			goto _exit;
		}

		if (timer.now<(ffly_tpv_t(*)(ffly_tpv_t))firefly::system::time::ns_to_ms>() > 1000) {
			this->fps = this->frame_c;
			this->frame_c = 0;
			timer.reset();
		}
/*
		if (!window.is_event_buff_empty()) {
			firefly::system::io::printf(stdout, "dumping window event buffer.\n");
			window.dump_event_buff();
		}

		mdl_u8_t c = 0;
*/
		// pull max of 10 events from window at one time
	/*
		while(window.event_queue.size() > 0 && c++ < 10) {
			firefly::types::err_t any_err = FFLY_SUCCESS;
			firefly::system::event_add(window.event_queue.pop(any_err));
			firefly::system::io::printf(stdout, "forwarding window event queue to global one.\n");
			if (any_err != FFLY_SUCCESS) {
				firefly::system::io::printf(stderr, "failure.");
			}
		}
*/
		if (_err(__extern_loop(0, &this->portal, NULL))) {
			firefly::system::io::fprintf(ffly_err, ".\n");
			goto _fatal;
		}
# ifdef __WITH_LAYER_MANAGER
		this->layer_m.draw_layers(this->window.frame_buff(), this->window.get_width(), this->window.get_height());
# endif
		firefly::graphics::pipe_unload_all(&__ffly_grp__);
		this->window.display();
		this->frame_c++;
	} while(!to_shutdown);

	window.kill();

	_exit:
	window.ok();

	_fatal:
	return err;
}
