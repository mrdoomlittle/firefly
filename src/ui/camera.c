# include "camera.h"
# include "../system/errno.h"
# include "../types/byte_t.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../data/mem_set.h"
# include "../data/bcopy.h"
ffly_err_t ffly_camera_init(ffly_camerap __camera, mdl_uint_t __width, mdl_uint_t __height) {
    mdl_uint_t size;
    __camera->pixels = (ffly_byte_t*)__ffly_mem_alloc((size = (__width*__height*4)));
    __camera->width = __width;
    __camera->height = __height;
    __camera->x = 0;
    __camera->y = 0;
    __camera->z = 0;
    ffly_mem_set(__camera->pixels, 0x0, size);
    return FFLY_SUCCESS;
}

void static print_pixel(ffly_byte_t *__pixel) {
    mdl_u8_t gray = (__pixel[0]+__pixel[1]+__pixel[2])/3;
    char c[] = {'-', '+', '=', '#'};
    double max = 255.0/sizeof(c);
    ffly_printf("%c", c[(mdl_u8_t)(gray/max)]);
}

ffly_err_t ffly_camera_print(ffly_camerap __camera) {
    mdl_uint_t x, y = 0;
    while(y != __camera->height) {
        x = 0;
        while(x != __camera->width) {
            print_pixel(__camera->pixels+((x+(y*__camera->width))*4));
            x++;
        }
        ffly_printf("\n");
        y++;
    }

    return FFLY_SUCCESS;
}

# ifndef __ffly_testing
#   include "../graphics/draw.h"
# endif
ffly_err_t ffly_camera_draw(ffly_camerap __camera, ffly_byte_t *__dst, mdl_uint_t __width, mdl_uint_t __height, mdl_uint_t __x, mdl_uint_t __y) {
# ifdef __ffly_testing
    mdl_uint_t x, y = 0;
    while(y != __camera->height) {
        x = 0;
        while(x != __camera->width) {
            ffly_bcopy(__dst+(((x+__x)+((y+__y)*__width))*4), __camera->pixels+((x+(y*__camera->width))*4), 4);
            x++;
        }
        y++;
    }
# else
    ffly_pixeldraw(0, 0, __dst, __width, __camera->pixels, __camera->width, __camera->height);
# endif
}

ffly_err_t ffly_camera_handle(ffly_camerap __camera) {
    ffly_mem_set(__camera->pixels, 0, __camera->width*__camera->height*4);
    ffly_uni_frame(__camera->uni, __camera->pixels, __camera->width, __camera->height, 1, __camera->x, __camera->y, 0);
}

ffly_err_t ffly_camera_bind(ffly_camerap __camera, ffly_unip __uni) {
    __camera->uni = __uni;
}
