# ifndef __ffly__context__h
# define __ffly__context__h
# include "ffint.h"
# include "types.h"
# include "driver.h"
# define G_CONTEXT \
	(&__ffly_grctx__)

/*
	for later

	TODO:
		remove .stack from context???
		or make it easer to manage 
*/
struct ff_context {
	struct ff_driver driver;
	ff_uint_t stack;
};

void ffly_g_setctx(ff_u16_t);
ff_u16_t ffly_g_ctx_new(void);
void ffly_g_ctx_destroy(ff_u16_t);
extern struct ff_context __ffly_grctx__;
# endif /*__ffly__context__h*/
