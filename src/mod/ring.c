# include "ring.h"
# include "../call.h"
# include "pipe.h"
# include "../system/pipe.h"
# include "../ffly_def.h"
void static
ring_malloc(void *__ret, void *__params) {
	ffly_pipe_write(__params, sizeof(mdl_uint_t), ffmod_pipeno());
	*(void**)__ret = (void*)ffly_pipe_rd64l(ffmod_pipeno()); 
}

void static
ring_free(void *__ret, void *__params) {
	ffly_pipe_wr64l(*(mdl_u64_t*)__params, ffmod_pipeno());
}

static void(*ring[])(void*, void*) = {
	NULL,
	NULL,
	ring_malloc,
	ring_free,
	NULL
};

void ffmod_ring(mdl_u8_t __no, void *__ret, void *__params) {
	ffly_pipe_wr8l(__no, ffmod_pipeno());
	ring[__no](__ret, __params);
}
