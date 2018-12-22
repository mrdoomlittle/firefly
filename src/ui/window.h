# ifndef __ffly__gui__window__h
# define __ffly__gui__window__h
# include "../ffint.h"
# include "btn.h"
# include "../resource.h"
# include "../pallet.h"
#define FFLY_UI_WD_HOG 0x01
#define _ui_window(__wd, __what)\
	(__wd)->__what
#define _ui_window_w(__wd)	_ui_window(width)
#define _ui_window_h(__wd)	_ui_window(height)
#define _ui_window_x(__wd)	_ui_window(x)
#define _ui_window_y(__wd)	_ui_window(y)

#define _ui_window_init(...)\
	ffly_gui_window_init(__VA_ARGS__)
#define _ui_window_write(...)\
	ffly_gui_window_write(__VA_ARGS__)
#define _ui_window_de_init(...)\
	ffly_gui_window_de_init(__VA_ARGS__)
struct ffly_gui_window {
	ff_u16_t x, y;
	ff_u16_t width, height;
	ff_u8_t *pixels;

	ff_u8_t flags;
	ff_rs_t exit_icon; 
	ffly_gui_btnp exit_btn;

	struct ffly_pallet pallet;
	ff_u32_t co, ch;

	void *arg_p;
	void(*exit)(void*);
};

typedef struct ffly_gui_window* ffly_gui_windowp;

void ffly_gui_window_init(ffly_gui_windowp, ff_u16_t, ff_u16_t, ff_u8_t, ff_u16_t, ff_u16_t);
void ffly_gui_window_write(ffly_gui_windowp, ff_u8_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_gui_window_update(ffly_gui_windowp);
void ffly_gui_window_draw(ffly_gui_windowp);
void ffly_gui_window_de_init(ffly_gui_windowp);
# endif /*__ffly__gui__window__h*/
