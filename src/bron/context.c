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
	return ct;
}

void static
_bron_ctx_destroy(ff_u16_t __ctx) {
	BRON_CONTEXT->driver.ctx_destroy(__ctx);
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
