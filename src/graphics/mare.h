# ifndef __ffly__graphics__mare__h
# define __ffly__graphics__mare__h
# include "../ffint.h"
struct ffly_mdd {
	void(*display_open)(void*);
	void(*display_close)(void*);
	void(*init)(void*);
	void(*de_init)(void*);
	void(*cleanup)(void*);
	void(*window_creat)(void*, ff_uint_t, ff_uint_t, char const*);
	void(*window_destroy)(void*);
	void(*window_display)(void*);
	void(*get)(ff_u8_t, long long, void*);
};

typedef struct ffly_mctx {
	struct ffly_mdd driver;
	void *context;
} *ffly_mctxp;

# define mare_func(__ctx, __func, ...) \
	((__ctx)->driver.__func)
# define mare_display_open(__ctx) \
	mare_func(__ctx, display_open)((__ctx)->context)
# define mare_display_close(__ctx) \
	mare_func(__ctx, display_close)((__ctx)->context)
# define mare_init(__ctx) \
	mare_func(__ctx, init)((__ctx)->context)
# define mare_de_init(__ctx) \
	mare_func(__ctx, de_init)((__ctx)->context)
# define mare_cleanup(__ctx) \
	mare_func(__ctx, cleanup)((__ctx)->context)
# define mare_window_creat(__ctx, ...) \
	mare_func(__ctx, window_creat)((__ctx)->context, __VA_ARGS__)
# define mare_window_destroy(__ctx) \
	mare_func(__ctx, window_destroy)((__ctx)->context)
# define mare_window_display(__ctx) \
	mare_func(__ctx, window_display)((__ctx)->context)
# define mare_get(__ctx, __what, __arg) \
	mare_func(__ctx, get)(__what, (long long)(__arg), (__ctx)->context)

# define _MARE_GET_FRAME_BUFF	0x00
# define _MARE_GET_WIDTH		0x01
# define _MARE_GET_HEIGHT		0x02

# define mare(__driver) \
	ffly_mare(__driver)
void ffly_mare_x11(ffly_mctxp);
void ffly_mare_xcb(ffly_mctxp);
void ffly_mare_slurry(ffly_mctxp);
enum {
	_mare_x11,
	_mare_xcb,
	_mare_slurry
};

ffly_mctxp ffly_mare(ff_u8_t);
void ffly_mare_cleanup(ffly_mctxp);
# endif /*__ffly__graphics__mare__h*/
