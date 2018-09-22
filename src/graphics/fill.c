# include "fill.h"
# include "pipe.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../bron/pixel.h"
ff_err_t ffly_pixfill(ff_u32_t __npix, ffly_colour_t __colour, ff_u32_t __off) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixfill(__npix, __colour, __off));
}

/*
	only temp

	TODO:
		need a graphics stack for colour and shit alike why?

		because we dont know how many channels there are going to be
		so pass the pointer and colour format.
*/

ff_u8_t static cbuf[64];
ff_u8_t static *n = cbuf;
ff_err_t __ffly_pixfill(ff_u32_t __npix, ffly_colour_t __colour, ff_u32_t __off) {
	ffly_printf("pixfill\n");
	if (n+sizeof(ffly_colour_t) > cbuf+64) {
		n = cbuf;
	}
	*(ffly_colour_t*)n = __colour;
	bron_pixfill(__npix, n);
	n+=sizeof(ffly_colour_t);
	return FFLY_SUCCESS;
}
