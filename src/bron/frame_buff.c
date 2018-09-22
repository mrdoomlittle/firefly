# include "frame_buff.h"
# include "context.h"
ff_u16_t static
_bron_fb_new(ff_uint_t __width, ff_uint_t __height) {
	ff_u16_t fb;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	fb = ctx->stack;
	ctx->stack+=8;
	ctx->driver.fb_new(__width, __height, fb);
	return fb;
}

void static
_bron_fb_destroy(ff_u16_t __fb) {
	BRON_CONTEXT->driver.fb_destroy(__fb);
}

void static
_bron_fb_set(ff_u16_t __fb) {
	BRON_CONTEXT->driver.fb_set(__fb);
}

ff_u16_t ffly_bron_fb_new(ff_uint_t __width, ff_uint_t __height) {
	return _bron_fb_new(__width, __height);
}

void ffly_bron_fb_destroy(ff_u16_t __fb) {
	_bron_fb_destroy(__fb);
}

void ffly_bron_fb_set(ff_u16_t __fb) {
	_bron_fb_set(__fb);
}
