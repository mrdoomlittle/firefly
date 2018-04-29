# ifndef __ffly__camera__h
# define __ffly__camera__h
# include "../ffint.h"
# include "../types.h"
# include "../uni.h"
# include "../types/pixelmap_t.h"
# define ffly_set_camerax(__camera, __x) \
    __camera.x = __x;
# define ffly_set_cameray(__camera, __y) \
    __camera.y = __y;
struct ffly_camera {
    ff_uint_t width, height;
    ffly_pixelmap_t pixels;
    ff_uint_t x, y, z;
    ffly_unip uni;
};

typedef struct ffly_camera* ffly_camerap;
# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_camera_init(ffly_camerap, ff_uint_t, ff_uint_t);
ff_err_t ffly_camera_print(ffly_camerap);
ff_err_t ffly_camera_handle(ffly_camerap);
ff_err_t ffly_camera_bind(ffly_camerap, ffly_unip);
ff_err_t ffly_camera_draw(ffly_camerap, ff_byte_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_camera_de_init(ffly_camerap);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace ui {
struct camera {
    types::err_t init(ff_uint_t __width, ff_uint_t __height) {
        return ffly_camera_init(&this->raw, __width, __height);
    }
    types::err_t handle() {
        return ffly_camera_handle(&this->raw);
    }
    types::err_t bind(uni& __uni) {
        return ffly_camera_bind(&this->raw, &__uni.raw);
    }
    types::err_t draw(types::byte_t *__dst, uint_t __width, uint_t __height, uint_t __x, uint_t __y) {
        return ffly_camera_draw(&this->raw, __dst, __width, __height, __x, __y);
    }
    void setx(uint_t __x){ffly_set_camerax(this->raw, __x);}
    void sety(uint_t __y){ffly_set_cameray(this->raw, __y);}
    void __inline__ set_coords(uint_t __x, uint_t __y) {
        this->setx(__x);
        this->sety(__y);
    }
    struct ffly_camera raw;
};
}
}
}
# endif
# endif /*__ffly__camera__h*/
