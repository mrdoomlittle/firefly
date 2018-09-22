# include "pixel.h"
# include "context.h"
void static
_bron_pixdraw(ff_u32_t __x, ff_u32_t __y, ff_u8_t *__pixels,
    ff_u32_t __width, ff_u32_t __height) {
	BRON_CONTEXT->driver.pixdraw(__x, __y, __pixels, __width, __height);
}

void static
_bron_pixfill(ff_u32_t __npix, ff_u8_t *__colour) {
	BRON_CONTEXT->driver.pixfill(__npix, __colour);
}

void ffly_bron_pixdraw(ff_u32_t __x, ff_u32_t __y, ff_u8_t *__pixels,
	ff_u32_t __width, ff_u32_t __height)
{
	_bron_pixdraw(__x, __y, __pixels,
				__width, __height);
}

void ffly_bron_pixfill(ff_u32_t __npix, ff_u8_t *__colour) {
	_bron_pixfill(__npix, __colour);
}
