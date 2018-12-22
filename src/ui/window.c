# include "window.h"
# include "../pixel.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../graphics/draw.h"
# include "../graphics/fill.h"
# include "../pallet.h"
# include "../graphics/chamber.h"
void static draw(long long __arg) {
	ffly_gui_windowp w;
	w = (ffly_gui_windowp)__arg;
	ffly_pallet_draw(&w->pallet, w->x, w->y);
}

void ffly_gui_window_init(ffly_gui_windowp __wd, ff_u16_t __width, ff_u16_t __height,
	ff_u8_t __flags, ff_u16_t __x, ff_u16_t __y)
{
	__wd->width = __width;
	__wd->height = __height;
	__wd->x = __x;
	__wd->y = __y;
	__wd->pixels = (ff_u8_t*)__ffly_mem_alloc(__width*__height*PIXEL_BLOCK);
	__wd->flags = __flags;
	ffly_pallet_init(&__wd->pallet, __width, __height, _ffly_tile_64);
	if ((__flags&FFLY_UI_WD_HOG)>0) {
		__wd->co = ffly_chamber_add(draw, (long long)__wd, __wd->ch = __ffly__chamber__);
	}
}

void ffly_gui_window_write(ffly_gui_windowp __wd, ff_u8_t *__src,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ffly_pallet_write(&__wd->pallet, __src, __width, __height, __x, __y);
}

void ffly_gui_window_update(ffly_gui_windowp __wd) {
}

void ffly_gui_window_draw(ffly_gui_windowp __wd) {
	ffly_pallet_draw(&__wd->pallet, __wd->x, __wd->y);
}

void ffly_gui_window_de_init(ffly_gui_windowp __wd) {
	ffly_pallet_de_init(&__wd->pallet);
	if ((__wd->flags&FFLY_UI_WD_HOG)>0) {
		ffly_chamber_rm(__wd->ch, __wd->co);
	}
	__ffly_mem_free(__wd->pixels);
}

