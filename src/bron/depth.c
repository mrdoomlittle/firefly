# include "depth.h"
# include "context.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_set.h"
void bron_dpb_alloc(void) {
	ff_uint_t ms;//memsize
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	ctx->dpbuf = (ff_u16_t*)__ffly_mem_alloc(ms = ((ctx->dpbs = (ctx->width*ctx->height))*sizeof(ff_u16_t)));
	ffly_mem_set(ctx->dpbuf, 0, ms);
}

void bron_dpb_free(void) {
	__ffly_mem_free(BRON_CONTEXT->dpbuf);
}

void bron_dpb_save(void) {
	BRON_CONTEXT->driver.dpbuf_write(BRON_CONTEXT->dpbs, 0, BRON_CONTEXT->dpbuf);
}
