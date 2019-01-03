# ifndef __ffly__ui__window__h
# define __ffly__ui__window__h
# include "../ffint.h"
# include "btn.h"
# include "../resource.h"
# include "../pallet.h"
#define FFLY_UI_WD_HOG	0x01
#define FF_UI_WS_AREA	0x01
#define FF_UI_WS_POS	0x02
#define _ui_window(__wd, __what)\
	(__wd)->__what
#define _ui_window_w(__wd)	_ui_window(width)
#define _ui_window_h(__wd)	_ui_window(height)
#define _ui_window_x(__wd)	_ui_window(x)
#define _ui_window_y(__wd)	_ui_window(y)

#define _ui_window_destroy	ffly_ui_window_destroy
#define _ui_window_creat	ffly_ui_window_creat
#define _ui_window_init		ffly_ui_window_init
#define _ui_window_write	ffly_ui_window_write
#define _ui_window_de_init	ffly_ui_window_de_init
#define _ui_window_set		ffly_ui_window_set
#define _ui_window_compose	ffly_ui_window_compose

struct ffly_ui_window_struc {
	ff_u16_t x, y;
	ff_u16_t width, height;
};

/*
	TODO:
		pallet to be replaces with frame buffer that will then write to pallet 
*/
struct ffly_ui_window {
	ff_u16_t x, y;
	ff_u16_t width, height;
//	ff_u8_t *pixels;

	ff_u8_t bits;
	ff_u8_t flags;
	ff_rs_t exit_icon; 
	ffly_ui_btnp exit_btn;

	struct ffly_pallet pallet;
	ff_u32_t co, ch;


	long long arg;
	void(*exit)(long long);
};

typedef struct ffly_ui_window* ffly_ui_windowp;

ffly_ui_windowp ffly_ui_window_creat(ff_u8_t);
void ffly_ui_window_set(long long, ffly_ui_windowp, ff_u8_t);
void ffly_ui_window_compose(ffly_ui_windowp);
void ffly_ui_window_init(ffly_ui_windowp, ff_u8_t);
void ffly_ui_window_write(ffly_ui_windowp, ff_u8_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_ui_window_update(ffly_ui_windowp);
void ffly_ui_window_draw(ffly_ui_windowp);
void ffly_ui_window_de_init(ffly_ui_windowp);
void ffly_ui_window_destroy(ffly_ui_windowp);
# endif /*__ffly__ui__window__h*/
