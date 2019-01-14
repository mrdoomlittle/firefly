# include "context.h"
struct bron_context __ffly_grctx__;


void static
_bron_setctx(ff_u16_t __ctx) {
	BRON_CONTEXT->driver.setctx(__ctx);
}

ff_u16_t static
_bron_ctx_new(void) {
	ff_u16_t ct;
	ct = BRON_CONTEXT->stack;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	ctx->stack+=sizeof(void*);
	ctx->driver.ctx_new(ct);
	ctx->ex = 0;
	ctx->ey = 0;
	ctx->ez = 0;
	ctx->a = 1;
	ctx->b = 1;
	ctx->c = 1;
	ctx->fov = 1;
	ctx->depth = 0;
	ctx->m = bron_matrix_new();
	return ct;
}

void static
_bron_ctx_destroy(ff_u16_t __ctx) {
	BRON_CONTEXT->driver.ctx_destroy(__ctx);
	bron_matrix_destroy(BRON_CONTEXT->m);
}

void ffly_bron_setctx(ff_u16_t __ctx) {
	_bron_setctx(__ctx);
}

ff_u16_t ffly_bron_ctx_new(void) {
	return _bron_ctx_new();
}

void ffly_bron_ctx_destroy(ff_u16_t __ctx) {
	_bron_ctx_destroy(__ctx);
}
