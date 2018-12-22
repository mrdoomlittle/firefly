# ifndef __ffly__context__h
# define __ffly__context__h
# include "bron/context.h"
# include "system/queue.h"
# include "graphics/frame_buff.h"
#define __ctx(__what)\
	__ffctx__->__what
struct ff_context {
	ff_u16_t vpw, vph;
	// high level frame buffer
	ffly_frame_buffp hl_fb;

	ff_u16_t ll_fb;
	struct ffly_queue event_queue;
};

struct ff_context* ff_ctx_new(void);

struct ff_context *__ffctx__;
# endif /*__ffly__context__h*/

