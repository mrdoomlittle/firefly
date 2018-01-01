# ifndef __ffly__camera__h
# define __ffly__camera__h
# include <mdlint.h>
# include "../uni.h"
# include "../types/err_t.h"
# include "../types/pixelmap_t.h"
# define ffly_set_camerax(__camera, __x) \
    __camera.xa = __x;
# define ffly_set_cameray(__camera, __y) \
    __camera.ya = __y;
struct ffly_camera {
    mdl_uint_t width, height;
    ffly_pixelmap_t pixels;
    mdl_uint_t xa, ya;
    ffly_unip uni;
};

typedef struct ffly_camera* ffly_camerap;
ffly_err_t ffly_camera_init(ffly_camerap, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_camera_print(ffly_camerap);
ffly_err_t ffly_camera_handle(ffly_camerap);
ffly_err_t ffly_camera_bind(ffly_camerap, ffly_unip);
# endif /*__ffly__camera__h*/
