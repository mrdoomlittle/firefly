# include "frame_buff.h"
# include "context.h"
# include "plate.h"
ff_u32_t static
_bron_fb_new(ff_uint_t __width, ff_uint_t __height) {
	ff_u32_t fb;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	fb = bron_plate_alloc();
	ctx->driver.fb_new(__width, __height, fb);
	return fb;
}

void static
_bron_fb_destroy(ff_u32_t __fb) {
	BRON_CONTEXT->driver.fb_destroy(__fb);
	bron_plate_free(__fb);
}

void static
_bron_fb_set(ff_u32_t __fb) {
	BRON_CONTEXT->driver.fb_set(__fb);
}

ff_u32_t ffly_bron_fb_new(ff_uint_t __width, ff_uint_t __height) {
	return _bron_fb_new(__width, __height);
}

void ffly_bron_fb_destroy(ff_u32_t __fb) {
	_bron_fb_destroy(__fb);
}

void ffly_bron_fb_set(ff_u32_t __fb) {
	_bron_fb_set(__fb);
}
