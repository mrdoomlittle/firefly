# include "graphics.h"
# include "types.h"
# include "graphics/frame_buff.h"
ff_uint_t static width;
ff_uint_t static height;
void ff_set_frame_size(ff_uint_t __width, ff_uint_t __height) {
	width = __width;
	height = __height;
}

void ff_graphics_init() {
	ff_err_t err;
	__frame_buff__ = ffly_frame_buff_creat(width, height, 4, &err);	
}

void ff_graphics_de_init() {
	ffly_frame_buff_del(__frame_buff__);
}
