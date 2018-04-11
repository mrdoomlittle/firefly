# include "pipe.h"
# include "../system/pipe.h"
mdl_uint_t static pipe;
mdl_uint_t ffmod_pipe_shmid(void) {
	return ffly_pipe_get_shmid(pipe);
}

mdl_uint_t ffmod_pipeno(void) {
	return pipe;
}

void ffmod_pipe(void) {
	ffly_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);
}

void ffmod_pipe_open(mdl_uint_t __shm_id) {
	ffly_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_SHMM, __shm_id, &err);
}

void ffmod_pipe_close(void) {
	ffly_pipe_close(pipe);
}
