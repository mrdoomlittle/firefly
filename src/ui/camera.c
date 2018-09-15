# include "camera.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_set.h"
# include "../dep/bcopy.h"
ff_err_t ffly_camera_init(ffly_camerap __camera, ff_uint_t __width, ff_uint_t __height) {
	ff_uint_t size;
	ffly_pallet_init(&__camera->vis, __width, __height, _ffly_tile_32);
	__camera->width = __width;
	__camera->height = __height;
	__camera->x = 0;
	__camera->y = 0;
	__camera->z = 0;
	return FFLY_SUCCESS;
}

void ffly_camera_de_init(ffly_camerap __camera) {
	ffly_pallet_de_init(&__camera->vis);
}
/*
void static print_pixel(ff_byte_t *__pixel) {
	ff_u8_t gray = (__pixel[0]+__pixel[1]+__pixel[2])/3;
	char c[] = {'-', '+', '=', '#'};
	double max = 255.0/sizeof(c);
	ffly_printf("%c", c[(ff_u8_t)(gray/max)]);
}

ff_err_t ffly_camera_print(ffly_camerap __camera) {
	ff_uint_t x, y = 0;
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
*/
# include "../graphics/draw.h"
ff_err_t ffly_camera_draw(ffly_camerap __camera, ff_byte_t *__dst, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y) {
//	ffly_pixdraw(0, 0, __dst, __width, __camera->pixels, __camera->width, __camera->height);
}

ff_err_t ffly_camera_handle(ffly_camerap __camera) {
//	ffly_mem_set(__camera->pixels, 0, __camera->width*__camera->height*4);
//	ffly_uni_frame(__camera->uni, __camera->pixels, __camera->width, __camera->height, 1, __camera->x, __camera->y, 0);
}

ff_err_t ffly_camera_bind(ffly_camerap __camera, ffly_unip __uni) {
	__camera->uni = __uni;
}
