# include "frame_buff.h"
# include "context.h"
ff_u16_t ffly_g_fb_new(ff_uint_t __width, ff_uint_t __height) {
	ff_u16_t fb;

	fb = G_CONTEXT->stack;
	G_CONTEXT->stack+=sizeof(void*);
	G_CONTEXT->driver.fb_new(__width, __height, fb);
	return fb;
}

void ffly_g_fb_destroy(ff_u16_t __fb) {
	G_CONTEXT->driver.fb_destroy(__fb);
}

void ffly_g_fb_set(ff_u16_t __fb) {
	G_CONTEXT->driver.fb_set(__fb);
}
