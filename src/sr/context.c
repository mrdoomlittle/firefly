# include "context.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "raise.h"
# include "../system/io.h"
struct sr_context *sr_ctx;
void
sr_ctx_new(void) {
	struct sr_context *ctx;

	ctx = (struct sr_context*)__ffly_mem_alloc(sizeof(struct sr_context));

	*((struct sr_context**)stack_at(*(ff_u16_t*)sr_raise_p)) = ctx;
	ffly_printf("new context: %p\n", ctx);
}

void
sr_ctx_destroy(void) {
	struct sr_context *ctx;
	ctx = *(struct sr_context**)stack_at(*(ff_u16_t*)sr_raise_p);
	__ffly_mem_free(ctx);
}

void
sr_setctx(void) {
	sr_ctx = *(struct sr_context**)stack_at(*(ff_u16_t*)sr_raise_p);
	ffly_printf("set context to : %p\n", sr_ctx);
}

void
sr_start(void) {
	ffly_printf("start.\n");
	ffly_printf("FB: %p, width: %u, height: %u\n", sr_ctx->fb, sr_ctx->fb->width, sr_ctx->fb->height);
}

void
sr_finish(void) {
	ffly_printf("finish.\n");
}
