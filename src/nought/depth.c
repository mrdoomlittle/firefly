# include "depth.h"
# include "raise.h"
# include "context.h"
# include "../dep/mem_cpy.h"
# include "../system/io.h"
ff_i8_t nt_dpbtas(ff_u16_t __depth, ff_u32_t __x, ff_u32_t __y) {
	struct nt_context *ctx;
	ctx = nt_ctx;
	ff_u16_t *v = ctx->fb->dpb+__x+(__y*ctx->fb->width);
	if (__depth<*v) {
//		ffly_printf("dif: %u.\n", (*v)-__depth);
		*v = __depth;
		return 0;
	} else {
//		ffly_printf("miss.\n");
	}
	return -1;
}

ff_u16_t nt_dpbuf_get(ff_u32_t __x, ff_u32_t __y) {
	return *(nt_ctx->fb->dpb+__x+(__y*nt_ctx->fb->width));
}

void nt_dpbuf_write(void) {
	ff_u32_t size;
	ff_u32_t offset;
	void *src;

	size = *(ff_u32_t*)nt_raise_p;
	offset = *(ff_u32_t*)(nt_raise_p+4);
	src = *(void**)(nt_raise_p+8);
	

	ff_u16_t *dst;
	dst = nt_ctx->fb->dpb+offset;
	ffly_mem_cpy(dst, src, size);
}
