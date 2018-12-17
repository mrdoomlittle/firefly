# include "window.h"
# include <unistd.h>
# include "../system/pipe.h"
# include "../types.h"
# include "../system/io.h"
# include "../system/event.h"
# include "../duct.h"
# include "../system/nanosleep.h"
# include "../dep/mem_set.h"
# include "../types/wd_event_t.h"
# define WIDTH 448
# define HEIGHT 448
int main() {
	ffly_io_init();
	ff_err_t err;
	ff_dcp c;
	c = ff_duct(0, FF_PIPE_SHMM);
	duct(c, open);
//	if (err != FFLY_SUCCESS) {
//		goto _fail;	
//	}

	ffly_event_t *event;
	struct ffly_wd window;
	ffly_wd_init(&window, WIDTH, HEIGHT, "Hello");
	ffly_wd_open(&window);
	ffly_printf("waiting.\n");
	duct(c, connect);
	ffly_printf("connected.\n");
	ffly_wd_display(&window);
//	ff_u8_t i;
//	i = 0;
_again:		
	while(1) {
		event = ffly_wd_poll_event(&window, &err);
		if (!event) break;
		ffly_printf("event..., %u - close: %u\n", event->kind, _ffly_wd_ek_closed);
		if (event->kind == _ffly_wd_ek_closed) {
			ffly_wd_free_event(&window, event);
			ff_event_free(event);
			ff_duct_exit(c);
			goto _end;
		} else if (event->kind == _ffly_wd_ek_btn_press || event->kind == _ffly_wd_ek_btn_release) {
			ffly_wd_event_t *e = (ffly_wd_event_t*)event->data;
			ffly_printf("button press/release. %d X %d\n", e->x, e->y);
			ff_duct_event(c, event);
		}

		ffly_wd_free_event(&window, event);
		ff_event_free(event);
		ffly_nanosleep(0, 30000000);
	}

	ff_duct_get_frame(c, ffly_wd_frame_buff(&window), WIDTH, HEIGHT, 4);
	ff_duct_done(c);
	ffly_wd_display(&window);

/*
	printf("frame:\n");
	ffly_mem_set(ffly_wd_frame_buff(&window), i, WIDTH*HEIGHT*4);
	if (i>=255) {
		i = 0;
	} else
		i++;
	ffly_wd_display(&window);
*/
	goto _again;
_end:
	ffly_wd_close(&window);
	ffly_wd_cleanup(&window);

	duct(c, close);
_fail:
	ffly_io_closeup();
}
