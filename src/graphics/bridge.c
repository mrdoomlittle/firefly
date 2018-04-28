# include "window.h"
# include <unistd.h>
# include "../system/pipe.h"
# include "../types.h"
# include "../system/io.h"
# include "../system/event_kind.h"
# include "../system/event.h"
int main() {
	ffly_io_init();
	ff_err_t err;
	ff_uint_t pipe;
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);
	ff_uint_t shm_id = ffly_pipe_get_shmid(pipe);
	ffly_printf("shmid: %u\n", shm_id);

	ffly_event_t *event;
	struct ffly_wd window;
	ffly_wd_init(&window, 400, 400, "Hello");
	ffly_wd_open(&window);
	ffly_printf("waiting.\n");
	ffly_pipe_listen(pipe);
	ffly_printf("connected.\n");
_again:		
/*
	while(1) {
		event = ffly_wd_poll_event(&window, &err);
		if (!event) continue;
		if (event->kind == _ffly_wd_ek_btn_press) {
			ffly_wd_free_event(&window, event);
			ffly_free_event(event);
			goto _end;
		} else if (event->kind == _ffly_wd_ek_key_press) {
			ffly_wd_free_event(&window, event);
			ffly_free_event(event);
			break;
		}

		ffly_wd_free_event(&window, event);
		ffly_free_event(event);
	}
*/
	if (ffly_pipe_rd8l(pipe, &err))
		goto _end;
	ffly_pipe_read(ffly_wd_frame_buff(&window), (400*400)*4, pipe);
	ffly_wd_display(&window);
	goto _again;
_end:
	ffly_wd_close(&window);
	ffly_wd_cleanup(&window);

	ffly_pipe_close(pipe);
	ffly_io_closeup();
}
