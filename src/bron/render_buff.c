# include "render_buff.h"
# include "context.h"
# include "plate.h"

ff_u32_t ffly_bron_rb_new(ff_uint_t __width, ff_uint_t __height) {
	ff_u32_t rb;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	rb = bron_plate_alloc();
	ctx->driver.rb_new(__width, __height, rb);
	return rb;
}

void ffly_bron_rb_destroy(ff_u32_t __rb) {
	BRON_CONTEXT->driver.rb_destroy(__rb);
	bron_plate_free(__rb);
}

void ffly_bron_rb_bind(ff_u32_t __rb, ff_u32_t __fb) {
	BRON_CONTEXT->driver.rb_bind(__rb, __fb);	
}
