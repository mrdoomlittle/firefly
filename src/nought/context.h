# ifndef __ffly__nt__context__h
# define __ffly__nt__context__h
# include "types.h"
# include "framebuff.h"
# include "renderbuff.h"
# include "view.h"
struct nt_context {
	/*
		the current frame in the making
	*/
	struct nt_framebuff *fb;

	struct nt_viewport viewport;

	// first renderbuffer
	struct nt_renderbuff *rb_top, *rb;
};

void nt_ctx_destroy(void);
void nt_ctx_new(void);
void nt_setctx(void);

void nt_start(void);
void nt_finish(void);
extern struct nt_context *nt_ctx;
# endif /*__ffly__nt__context__h*/
