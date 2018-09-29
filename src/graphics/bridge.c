# include "window.h"
# include <unistd.h>
# include "../system/pipe.h"
# include "../types.h"
# include "../system/io.h"
# include "../system/event.h"
# include "../duct.h"
# include "../system/nanosleep.h"
# define WIDTH 448
# define HEIGHT 448
int main() {
	ffly_io_init();
	ff_err_t err;
	ff_dcp c;
	c = ff_duct_open(FF_PIPE_SHMM);
//	if (err != FFLY_SUCCESS) {
//		goto _fail;	
//	}

	ffly_event_t *event;
	struct ffly_wd window;
	ffly_wd_init(&window, WIDTH, HEIGHT, "Hello");
	ffly_wd_open(&window);
	ffly_printf("waiting.\n");
	ff_duct_connect(c);
	ffly_printf("connected.\n");
_again:		
	while(1) {
		event = ffly_wd_poll_event(&window, &err);
		if (!event) break;
		if (event->kind == _ffly_wd_ek_closed) {
			ffly_wd_free_event(&window, event);
			ff_event_free(event);
			ff_duct_exit(c);
			goto _end;
		} else if (event->kind == _ffly_wd_ek_btn_press || event->kind == _ffly_wd_ek_btn_release) {
			ff_duct_event(c, event);
		}

		ffly_wd_free_event(&window, event);
		ff_event_free(event);
		ffly_nanosleep(0, 30000000);
	}

	ff_duct_get_frame(c, window.raw.frame_buff, WIDTH, HEIGHT, 4);
	ff_duct_done(c);
	ffly_wd_display(&window);
	goto _again;
_end:
	ffly_wd_close(&window);
	ffly_wd_cleanup(&window);

	ff_duct_close(c);
_fail:
	ffly_io_closeup();
}
