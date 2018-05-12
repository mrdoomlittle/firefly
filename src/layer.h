# ifndef __ffly__layer__h
# define __ffly__layer__h
# include "ffint.h"
# include "types.h"
# include "types/pixelmap_t.h"
typedef struct ffly_layer {
    ff_u16_t width, height, x, y;
    ffly_pixelmap_t pixelmap;
	ff_u8_t colour_format;
} ffly_layerp;


ffly_layerp ffly_layer_creat(ff_u16_t, ff_u16_t);
void ffly_layer_del(ffly_layerp);
typedef struct ffly_layer* ffly_layerp;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_layer layer;
}
}
# endif
# endif /*__ffly__layer__h*/
