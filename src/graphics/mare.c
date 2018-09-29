# include "mare.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../ffly_def.h"
static void(*load[])(ffly_mctxp) = {
#ifndef __ffly_use_x11
	NULL,
#else
	ffly_mare_x11,
#endif
#ifndef __ffly_use_xcb
	NULL
#else
	ffly_mare_xcb
# endif
};

ffly_mctxp ffly_mare(ff_u8_t __driver) {
	ffly_mctxp m;
	m = (ffly_mctxp)__ffly_mem_alloc(sizeof(struct ffly_mctx));
	load[__driver](m);
	return m;
}

void ffly_mare_cleanup(ffly_mctxp __m) {
	__ffly_mem_free(__m);
}
