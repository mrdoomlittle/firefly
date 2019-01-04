# ifndef __ffly__canvas__h
# define __ffly__canvas__h
/*
	i dont know if this is what a canvas is soo yea
*/

typedef struct ffly_canvas {
	ff_uint_t width, height;
} *ffly_canvasp;

ffly_canvasp ffly_canvas_new(void);
void ffly_canvas_fillrect(ffly_canvasp, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_u8_t);
void ffly_canvas_destroy(ffly_canvasp);
# endif /*__ffly__canvas__h*/
