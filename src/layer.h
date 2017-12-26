# ifndef __ffly__layer__h
# define __ffly__layer__h
# include <mdlint.h>
# include "types/pixelmap_t.h"
struct ffly_layer {
    mdl_u16_t width, height, xa, ya;
    ffly_pixelmap_t pixelmap; 
};

typedef struct ffly_layer* ffly_layerp;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_layer layer;
}
}
# endif
# endif /*__ffly__layer__h*/
