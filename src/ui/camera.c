# include "camera.h"
# include "../system/errno.h"
# include "../types/byte_t.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
ffly_err_t ffly_camera_init(ffly_camerap __camera, mdl_uint_t __width, mdl_uint_t __height) {
    __camera->pixels = (ffly_byte_t*)__ffly_mem_alloc(__width*__height*4);
    __camera->width = __width;
    __camera->height = __height;
    return FFLY_SUCCESS;
}

void static print_pixel(ffly_byte_t *__pixel) {
    mdl_u8_t gray = (__pixel[0]+__pixel[1]+__pixel[2])/3;
    char c[] = {'-', '+', '=', '#'};
    double max = 255.0/sizeof(c);
    printf("%c", c[(mdl_u8_t)(gray/max)]);
}

ffly_err_t ffly_camera_print(ffly_camerap __camera) {
    mdl_uint_t xa, ya = 0;
    while(ya != __camera->height) {
        xa = 0;
        while(xa != __camera->width) {
            print_pixel(__camera->pixels+((xa+(ya*__camera->width))*4));
            xa++;
        }
        printf("\n");
        ya++;
    }

    return FFLY_SUCCESS;
}

ffly_err_t ffly_camera_handle(ffly_camerap __camera) {
    ffly_uni_pixcopy(__camera->uni, __camera->pixels, __camera->width, __camera->height, 1, __camera->xa, __camera->ya, 0);
}

ffly_err_t ffly_camera_bind(ffly_camerap __camera, ffly_unip __uni) {
    __camera->uni = __uni;
}
