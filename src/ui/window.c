# include "window.h"
# include "../pixel.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../graphics/draw.h"
# include "../graphics/fill.h"
# include "../pallet.h"
void ffly_gui_window_init(ffly_gui_windowp __wd,
	ff_u16_t __width, ff_u16_t __height,
	ff_u16_t __x, ff_u16_t __y)
{
	__wd->width = __width;
	__wd->height = __height;
	__wd->x = __x;
	__wd->y = __y;
	__wd->pixels = (ff_u8_t*)__ffly_mem_alloc(__width*__height*PIXEL_BLOCK);
//	ffly_resource_creat(&__wd->exit_icon);
//	__wd->exit_btn = ffly_gui_btn_creat(ffly_resource_get(__wd->exit_icon), );
	ffly_pallet_init(&__wd->pallet, __width, __height, _ffly_tile_64);
//	ffly_pixfill(__wd->pixels, __width*__height, ffly_colour(0, 0, 255, 255));

//	ffly_pallet_update(&__wd->pallet, __wd->pixels, __wd->width, __wd->height);
}

void ffly_gui_window_write(ffly_gui_windowp __wd, ff_u8_t *__src,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ffly_pallet_write(&__wd->pallet, __src, __width, __height, __x, __y);
//	ffly_pixdraw(__x, __y, __wd->pixels, __wd->width, __src, __width, __height);
}

void ffly_gui_window_update(ffly_gui_windowp __wd) {
//	ffly_pallet_update(&__wd->pallet, __wd->pixels, __wd->width, __wd->height);
}

void ffly_gui_window_draw(ffly_gui_windowp __wd) {
	ffly_pallet_draw(&__wd->pallet, __wd->x, __wd->y);
}

void ffly_gui_window_de_init(ffly_gui_windowp __wd) {
	ffly_pallet_de_init(&__wd->pallet);
	__ffly_mem_free(__wd->pixels);
}

