# ifndef __ffly__layer__h
# define __ffly__layer__h
# include "ffint.h"
# include "types.h"
typedef struct ffly_layer {
    ff_u16_t width, height, x, y;
    ff_u8_t *pixels;
	ff_uint_t size; 
} *ffly_layerp;

ffly_layerp ffly_layer_creat(ff_u16_t, ff_u16_t);
void ffly_layer_del(ffly_layerp);
ff_err_t ffly_layer_draw(ffly_layerp, ff_u8_t*, ff_uint_t);

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_layer layer;
}
}
# endif
# endif /*__ffly__layer__h*/
