# include "pipe.h"
# include "../system/pipe.h"
ff_uint_t static pipe;
ff_uint_t ffmod_pipe_shmid(void) {
	return ffly_pipe_get_shmid(pipe);
}

ff_uint_t ffmod_pipeno(void) {
	return pipe;
}

void ffmod_pipe(void) {
	ff_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);
}

void ffmod_pipe_open(ff_uint_t __shm_id) {
	ff_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_SHMM, __shm_id, &err);
}

void ffmod_pipe_close(void) {
	ffly_pipe_close(pipe);
}
