# ifndef __ffly__nt__context__h
# define __ffly__nt__context__h
# include "types.h"
# include "framebuff.h"

struct nt_context {
	/*
		the current frame in the making
	*/
	struct nt_framebuff *fb;
};

void nt_ctx_destroy(void);
void nt_ctx_new(void);
void nt_setctx(void);

void nt_start(void);
void nt_finish(void);
extern struct nt_context *nt_ctx;
# endif /*__ffly__nt__context__h*/
