# ifndef __ffly__sr__context__h
# define __ffly__sr__context__h
# include "types.h"
# include "framebuff.h"
struct sr_context {
	/*
		the current frame in the making
	*/
	struct sr_framebuff *fb;
};

void sr_ctx_destroy(void);
void sr_ctx_new(void);
void sr_setctx(void);

void sr_start(void);
void sr_finish(void);
extern struct sr_context *sr_ctx;
# endif /*__ffly__sr__context__h*/
