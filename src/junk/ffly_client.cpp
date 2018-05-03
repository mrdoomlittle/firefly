# include "ffly_client.hpp"
# include "system/errno.h"
# include "graphics/wd_flags.h"
# include "system/timer.hpp"
# include "data/mem_set.h"
# include "system/io.h"
# include "firefly.hpp"
# include "system/err.h"
# include <cuda_runtime.h>
mdl::firefly::types::bool_t mdl::ffly_client::to_shutdown = 0;

void static ctrl_c(int __sig) {
	if (mdl::ffly_client::to_shutdown) {
		exit(0);
	}

	mdl::firefly::system::io::fprintf(ffly_out, "looks like ctrl_c was called bye bye...\n");
	mdl::ffly_client::to_shutdown = 1;
}

void mdl::ffly_client::shutdown() {
	mdl::ffly_client::to_shutdown = 1;
}

mdl::firefly::types::err_t mdl::ffly_client::init(u16_t __wd_width, u16_t __wd_height, char const *__wd_title) {
	cudaDeviceReset();
	firefly::types::err_t err;
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

	this->asset_m.de_init();

	firefly::system::io::fprintf(ffly_log, "de-initing engine.\n");
	if (_err(err = firefly::de_init())) {
		firefly::system::io::fprintf(ffly_err, "failed to de-init engine.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

//mdl::firefly::types::err_t mdl::ffly_client::forward_event(types::event_t& __event) {


//}

# include "system/event_kind.h"
# include "types/event_t.h"
# include "graphics/pipe.h"
# include "system/nanosleep.h"
mdl::firefly::types::err_t mdl::ffly_client::begin(ffly_err_t(*__extern_loop)(i8_t, portal_t*, void*), void *__arg_p) {
	window.open();
	firefly::types::err_t err;
	firefly::data::mem_set(this->window.frame_buff(), 0xFF, this->window.get_width()*this->window.get_height()*4);

	firefly::system::timer timer;
	timer.begin();

	this->portal.init(this);
	do {
//		ffly_nanosleep(0, 40000000); // debug
		if (timer.now<(ffly_tpv_t(*)(ffly_tpv_t))firefly::system::time::ns_to_ms>() > 1000) {
			this->fps = this->frame_c;
			this->frame_c = 0;
			timer.reset();
		}

		mdl::firefly::system::io::fprintf(ffly_log, "fps: %u\n", this->get_fps());

		firefly::types::event_t *event;
		while(window.poll_event(event, err)) {
			if (event->kind == _ffly_ek_unknown) {
				firefly::system::io::fprintf(ffly_log, "unknown window event was caught.\n");
				firefly::free_event(this, event);
			}
			else {
				switch(event->kind) {
					case _ffly_wd_ek_closed:
						firefly::system::io::fprintf(ffly_log, "got event to close window.\n");
					break;
					case _ffly_wd_ek_key_press:
						firefly::system::io::fprintf(ffly_log, "got key press event from window.\n");
					break;
                    case _ffly_wd_ek_btn_press:
                        firefly::system::io::fprintf(ffly_log, "got button press event from window.\n");
                    break;
					default:
    					firefly::system::io::fprintf(ffly_log, "window event hasent been implemented.\n");
						firefly::free_event(this, event);
						continue;
				}
				firefly::system::event_push(event);
			}
		}

		if (_err(__extern_loop(0, &this->portal, __arg_p))) {
			firefly::system::io::fprintf(ffly_err, ".\n");
			goto _fatal;
		}

		firefly::system::io::fprintf(ffly_log, "frame buff: %p\n", this->window.frame_buff());
		if (_err(this->layer_m.draw_layers(this->window.frame_buff(), this->window.get_width(), this->window.get_height()))) {
			goto _fatal;
		}

		if (_err(firefly::graphics::pipe_unload_all(&__ffly_grp__))) {
			goto _fatal;
		}

		this->window.display();
		this->frame_c++;
	} while(!to_shutdown);
	return FFLY_SUCCESS;
	_fatal:
	return err;
}
