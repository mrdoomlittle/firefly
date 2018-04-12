# include "window.h"
# include <unistd.h>
# include "../system/pipe.h"
# include "../types/err_t.h"
# include "../system/io.h"
int main() {
	ffly_io_init();
	ffly_err_t err;
	mdl_uint_t pipe;
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);
	mdl_uint_t shm_id = ffly_pipe_get_shmid(pipe);
	ffly_printf("shmid: %u\n", shm_id);

	struct ffly_wd window;
	ffly_wd_init(&window, 400, 400, "Hello");
	ffly_wd_open(&window);
	usleep(5000000);
	ffly_wd_close(&window);
	ffly_wd_cleanup(&window);

	ffly_pipe_close(pipe);

	ffly_io_closeup();
}
