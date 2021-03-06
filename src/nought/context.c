# include "context.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "raise.h"
# include "../system/io.h"
# include "../dep/mem_set.h"
struct nt_context *nt_ctx;

/*
	camera pos is 0,0 pointing to 1, 1
*/
void
nt_ctx_new(void) {
	struct nt_context *ctx;

	ctx = (struct nt_context*)__ffly_mem_alloc(sizeof(struct nt_context));

	*((struct nt_context**)stack_at(*(ff_u16_t*)nt_raise_p)) = ctx;
	ffly_printf("new context: %p\n", ctx);
	ctx->viewport.x = 0;
	ctx->viewport.y = 0;
	ctx->rb_top = NULL;
	ctx->rb = NULL;
}

void
nt_ctx_destroy(void) {
	struct nt_context *ctx;
	ctx = *(struct nt_context**)stack_at(*(ff_u16_t*)nt_raise_p);
	__ffly_mem_free(ctx);
}

void
nt_setctx(void) {
	nt_ctx = *(struct nt_context**)stack_at(*(ff_u16_t*)nt_raise_p);
	ffly_printf("set context to : %p\n", nt_ctx);
}

void
nt_start(void) {
	ffly_printf("start.\n");
	ffly_printf("FB: %p, width: %u, height: %u\n", nt_ctx->fb, nt_ctx->fb->width, nt_ctx->fb->height);
	ffly_mem_set(nt_ctx->fb->dpb, 255, nt_ctx->fb->width*nt_ctx->fb->height*sizeof(ff_u16_t));
}

void
nt_finish(void) {
	ffly_printf("finish.\n");
}
