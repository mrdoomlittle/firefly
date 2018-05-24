# include "layer.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "pixel.h"
ffly_layerp ffly_layer_creat(ff_u16_t __width, ff_u16_t __height) {
	ffly_layerp ret = (ffly_layerp)__ffly_mem_alloc(sizeof(struct ffly_layer));
	ret->width = __width;
	ret->height = __height;
	ret->size = __width*__height;
	ret->x = 0;
	ret->y = 0;
	ret->pixels = __ffly_mem_alloc(__width*__height*PIXEL_BLOCK);
	return ret;
}

void ffly_layer_del(ffly_layerp __layer) {
	__ffly_mem_free(__layer->pixels);
	__ffly_mem_free(__layer);
}
# include "graphics/draw.h"
ff_err_t
ffly_layer_draw(ffly_layerp __layer, ff_u8_t *__dst, ff_uint_t __width) {
	ffly_pixdraw(__layer->x, __layer->y, __dst, __width, __layer->pixels, __layer->width, __layer->height);
}
