# ifndef __ffly__workshop__h
# define __ffly__workshop__h
# include "ffint.h"
# include "types.h"
# include "ui/btn.h"
# include "ui/window.h"
# include "pallet.h"
# define WIDTH 448
# define HEIGHT 448

struct ff_workshop {
	ffly_gui_btnp fontforge;
	struct ffly_gui_window window;
};

extern void(*tick)(void);
struct ff_workshop extern workshop;
void ffly_workshop_start();
void ffly_workshop_init();
void ffly_workshop_de_init();
# endif /*__ffly__workshop__h*/
