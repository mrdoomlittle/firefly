# include "window.h"
# include "../pixel.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../graphics/draw.h"
# include "../graphics/fill.h"
# include "../pallet.h"
# include "../graphics/chamber.h"
#define WD_AREA		0x01
#define WD_POS		0x02

#define isbit(__bits, __bit)\
	(((__bits)&(__bit)) == (__bit))


#define setarea(__wd, __width, __height)\
	__wd->bits |= WD_AREA;\
	__wd->width = __width;\
	__wd->height = __height;
#define setpos(__wd, __x, __y)\
	__wd->bits |= WD_POS;\
	__wd->x = __x;\
	__wd->y = __y;

void static draw(long long __arg) {
	ffly_ui_windowp w;
	w = (ffly_ui_windowp)__arg;
	ffly_pallet_draw(&w->pallet, w->x, w->y);
}

void ffly_ui_window_set(long long __struc, ffly_ui_windowp __wd, ff_u8_t __flags) {
	struct ffly_ui_window_struc *srt;
	srt = (struct ffly_ui_window_struc*)__struc;
	if (isbit(__flags, FF_UI_WS_AREA)) {
		setarea(__wd, srt->width, srt->height);
	}
	if (isbit(__flags, FF_UI_WS_POS)) {
		setpos(__wd, srt->x, srt->y)
	}
}

void ffly_ui_window_compose(ffly_ui_windowp __wd) {
	if (!isbit(__wd->bits, WD_POS|WD_AREA)) {
		ffly_printf("sorry but the button pos or area has not been set.\n");
		return;
	}

	ffly_pallet_init(&__wd->pallet, __wd->width, __wd->height, _ffly_tile_64);
	
}

ffly_ui_windowp ffly_ui_window_creat(ff_u8_t __flags) {
	ffly_ui_windowp w;
	w = (ffly_ui_windowp)__ffly_mem_alloc(sizeof(struct ffly_ui_window));
	ffly_ui_window_init(w, __flags);
	return w;
}

void ffly_ui_window_destroy(ffly_ui_windowp __wd) {
	ffly_ui_window_de_init(__wd);
	__ffly_mem_free(__wd);
}

void ffly_ui_window_init(ffly_ui_windowp __wd, ff_u8_t __flags) {
	__wd->width = 0;
	__wd->height = 0;
	__wd->x = 0;
	__wd->y = 0;
	__wd->bits = 0x00;
///	__wd->pixels = (ff_u8_t*)__ffly_mem_alloc(__width*__height*PIXEL_BLOCK);
	__wd->flags = __flags;
	if ((__flags&FFLY_UI_WD_HOG)>0) {
		__wd->co = ffly_chamber_add(draw, (long long)__wd, __wd->ch = __ffly__chamber__);
	}
}

void ffly_ui_window_write(ffly_ui_windowp __wd, ff_u8_t *__src,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ffly_pallet_write(&__wd->pallet, __src, __width, __height, __x, __y);
}

void ffly_ui_window_update(ffly_ui_windowp __wd) {
}

void ffly_ui_window_draw(ffly_ui_windowp __wd) {
	ffly_pallet_draw(&__wd->pallet, __wd->x, __wd->y);
}

void ffly_ui_window_de_init(ffly_ui_windowp __wd) {
	ffly_pallet_de_init(&__wd->pallet);
	if ((__wd->flags&FFLY_UI_WD_HOG)>0) {
		ffly_chamber_rm(__wd->ch, __wd->co);
	}
//	__ffly_mem_free(__wd->pixels);
}

