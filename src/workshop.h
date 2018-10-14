# ifndef __ffly__workshop__h
# define __ffly__workshop__h
# include "ffint.h"
# include "types.h"
# include "ui/btn.h"
# include "ui/window.h"
# include "pallet.h"
# define WIDTH ws_width
# define HEIGHT ws_height
extern ff_uint_t ws_width;
extern ff_uint_t ws_height;
struct ff_workshop {
	ffly_gui_btnp fontforge;
	struct ffly_gui_window window;

	void(*cubuf[20])(void);
	void(**cu)(void);
};

extern void(*tick)(void);
struct ff_workshop extern workshop;
void ffly_workshop_start();
void ffly_workshop_init();
void ffly_workshop_de_init();
# endif /*__ffly__workshop__h*/
