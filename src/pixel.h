# ifndef __ffly__pixel__h
# define __ffly__pixel__h
# include "ffint.h"
# define PIXEL_BLOCK 4
# define setpix(__dst, __r, __g, __b, __a) \
	*(__dst) = __r; \
	(__dst)[1] = __g; \
	(__dst)[2] = __b; \
	(__dst)[3] = __a

void ffly_g_pixdraw(ff_u32_t, ff_u32_t, ff_u8_t*, ff_u32_t, ff_u32_t);
void ffly_g_pixfill(ff_u32_t, ff_u8_t*);
# endif /*__ffly__pixel__h*/
