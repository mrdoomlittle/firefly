# include "canvas.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_canvasp ffly_canvas_new(void) {
	ffly_canvasp cv;

	cv = (ffly_canvasp)__ffly_mem_alloc(sizeof(struct ffly_canvas));

	
	return cv;
}

void ffly_canvas_fillrect(ffly_canvasp __cv, ff_uint_t __x, ff_uint_t __y, ff_uint_t __width, ff_uint_t __height, ff_u8_t *__colour) {

}

void ffly_canvas_destroy(ffly_canvasp __cv) {
}
