# include "context.h"
struct ff_context __ffly_grctx__;

void ffly_g_setctx(ff_u16_t __ctx) {
	G_CONTEXT->driver.setctx(__ctx);
}

ff_u16_t ffly_g_ctx_new(void) {
	ff_u16_t ct;
	ct = G_CONTEXT->stack;
	struct ff_context *ctx;
	ctx = G_CONTEXT;
	ctx->stack+=sizeof(void*);
	ctx->driver.ctx_new(ct);
	return ct;
}

void ffly_g_ctx_destroy(ff_u16_t __ctx) {
	G_CONTEXT->driver.ctx_destroy(__ctx);
}
