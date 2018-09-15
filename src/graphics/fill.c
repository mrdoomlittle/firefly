# include "fill.h"
# include "pipe.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../pixel.h"
ff_err_t ffly_pixfill(ff_u32_t __npix, ffly_colour_t __colour, ff_u32_t __off) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixfill(__npix, __colour, __off));
}

/*
	only temp

	TODO:
		need a graphics stack or colour and shit alike why?

		because we dont know how many channels there are going to be
		so pass the pointer and colour format.
*/
ff_err_t __ffly_pixfill(ff_u32_t __npix, ffly_colour_t __colour, ff_u32_t __off) {
	ffly_printf("pixfill2\n");
	ff_u8_t static dfc[24];
	*(ffly_colour_t*)dfc = __colour;
	ffly_g_pixfill(__npix, dfc);
	return FFLY_SUCCESS;
}
