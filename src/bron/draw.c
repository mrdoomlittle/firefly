# include "draw.h"
# include "context.h"
void static
_bron_draw(ff_u16_t __buf, ff_u32_t __n) {
	BRON_CONTEXT->driver.draw(__buf, __n);
}

void ffly_bron_draw(ff_u16_t __buf, ff_u32_t __n) {
	_bron_draw(__buf, __n);
}
