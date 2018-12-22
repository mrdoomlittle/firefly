# include "context.h"
struct ff_context *__ffctx__;
#define MAXCTX 2
static struct ff_context ctxstack[MAXCTX];
static ff_uint_t off = 0;
struct ff_context* ff_ctx_new(void) {
	return (ctxstack+off++);
}
