# ifndef __ffly__gui__window__h
# define __ffly__gui__window__h
# include "../ffint.h"
# include "btn.h"
# include "../resource.h"
# include "../pallet.h"
struct ffly_gui_window {
	ff_u16_t x, y;
	ff_u16_t width, height;
	ff_u8_t *pixels;

	ff_rs_t exit_icon; 
	ffly_gui_btnp exit_btn;

	struct ffly_pallet pallet;

	void *arg_p;
	void(*exit)(void*);
};

typedef struct ffly_gui_window* ffly_gui_windowp;

void ffly_gui_window_init(ffly_gui_windowp, ff_u16_t, ff_u16_t, ff_u16_t, ff_u16_t);
void ffly_gui_window_write(ffly_gui_windowp, ff_u8_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_gui_window_update(ffly_gui_windowp);
void ffly_gui_window_draw(ffly_gui_windowp);
void ffly_gui_window_de_init(ffly_gui_windowp);

# define gui_window_width(__wd) \
	(__wd)->width
# define gui_window_height(__wd) \
	(__wd)->height
# define gui_window_x(__wd) \
	(__wd)->x
# define gui_window_y(__wd) \
	(__wd)->y
# endif /*__ffly__gui__window__h*/
