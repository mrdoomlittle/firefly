# include "ffly_studio.hpp"
# include "main.h"
# include "system/event.h"
# include "system/event_kind.h"
ffly_err_t ffmain(int __argc, char const **__argv) {
	io::cout << "...." << ffly_endl;
	graphics::window window;
	window.init(600, 600, "firefly studio");
	window.open();

	ffly_err_t err;
	types::event_t *event;
	while(1) {
		if (window.poll_event(event, err)) {
			if (event->kind == _ffly_wd_ek_closed) {
				break;
			}
			window.free_event(event);
		}
	}

	window.close();
	window.de_init();
}
