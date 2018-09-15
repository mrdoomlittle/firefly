# include "pixel.h"
# include "context.h"
void ffly_g_pixdraw(ff_u32_t __x, ff_u32_t __y, ff_u8_t *__pixels,
	ff_u32_t __width, ff_u32_t __height)
{
	G_CONTEXT->driver.pixdraw(__x, __y, __pixels, __width, __height);
}

void ffly_g_pixfill(ff_u32_t __npix, ff_u8_t *__colour) {
	G_CONTEXT->driver.pixfill(__npix, __colour);
}
