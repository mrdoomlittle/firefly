# include "layer.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_layerp ffly_layer_creat(ff_u16_t __width, ff_u16_t __height, ff_u8_t __format) {
	ffly_layerp ret = (ffly_layerp)__ffly_mem_alloc(sizeof(struct ffly_layer));
	ret->width = __width;
	ret->height = __height;
	ret->x = 0;
	ret->y = 0;
	ret->colour_format = __format;
	ret->pixelmap = __ffly_mem_alloc(__width*__height
	return ret;
}

void ffly_layer_del(ffly_layerp __layer) {
	__ffly_mem_free(__layer);
}

ff_err_t
ffly_layer_draw() {

}
