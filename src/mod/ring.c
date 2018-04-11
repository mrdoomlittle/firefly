# include "ring.h"
# include "../call.h"
# include "pipe.h"
# include "../system/pipe.h"
# include "../ffly_def.h"
# include "../types/err_t.h"
void static
ring_printf(void *__ret, void *__params) {
	ffly_pipe_wr64l(*(mdl_u64_t*)__params, ffmod_pipeno());
}

void static
ring_malloc(void *__ret, void *__params) {
	ffly_err_t err;
	ffly_pipe_write(__params, sizeof(mdl_uint_t), ffmod_pipeno());
	*(void**)__ret = (void*)ffly_pipe_rd64l(ffmod_pipeno(), &err); 
}

void static
ring_free(void *__ret, void *__params) {
	ffly_pipe_wr64l(*(mdl_u64_t*)__params, ffmod_pipeno());
}


void static
ring_dcp(void *__ret, void *__params) {
	void *dst = *(void**)__params;
	void *src = *(void**)((mdl_u8_t*)__params+8);
	mdl_uint_t *n = (mdl_uint_t*)((mdl_u8_t*)__params+16);

	ffly_pipe_wr64l((mdl_u64_t)src, ffmod_pipeno());
	ffly_pipe_write(n, sizeof(mdl_uint_t), ffmod_pipeno());
	ffly_pipe_read(dst, *n, ffmod_pipeno());
}

void static
ring_scp(void *__ret, void *__params) {
	void *dst = *(void**)__params;
	void *src = *(void**)((mdl_u8_t*)__params+8);
	mdl_uint_t *n = (mdl_uint_t*)((mdl_u8_t*)__params+16);

	ffly_pipe_wr64l((mdl_u64_t)dst, ffmod_pipeno());
	ffly_pipe_write(n, sizeof(mdl_uint_t), ffmod_pipeno());
	ffly_pipe_write(src, *n, ffmod_pipeno());	
}

static void(*ring[])(void*, void*) = {
	ring_printf,
	NULL,
	ring_malloc,
	ring_free,
	NULL,
	ring_dcp,
	ring_scp
};

void ffmod_ring(mdl_u8_t __no, void *__ret, void *__params) {
	ffly_pipe_wr8l(__no, ffmod_pipeno());
	ring[__no](__ret, __params);
}
