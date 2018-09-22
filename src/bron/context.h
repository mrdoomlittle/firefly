# ifndef __ffly__bron__context__h
# define __ffly__bron__context__h
# include "../ffint.h"
# include "../types.h"
# include "driver.h"
# define BRON_CONTEXT \
	(&__ffly_grctx__)
# define bron_setctx \
	ffly_bron_setctx
# define bron_ctx_new \
	ffly_bron_ctx_new
# define bron_ctx_destroy \
	ffly_bron_ctx_destroy
/*
	for later

	TODO:
		remove .stack from context???
		or make it easer to manage 
*/
# define bron_context \
	ff_bron_context

struct ff_bron_context {
	struct bron_driver driver;
	ff_uint_t stack;
};

void ffly_bron_setctx(ff_u16_t);
ff_u16_t ffly_bron_ctx_new(void);
void ffly_bron_ctx_destroy(ff_u16_t);
extern struct bron_context __ffly_grctx__;
# endif /*__ffly__bron__context__h*/
