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
	ff_u8_t*(*frame_buff)(void*);
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
# define mare_frame_buff(__ctx) \
	mare_func(__ctx, frame_buff)((__ctx)->context)


# define mare(__driver) \
	ffly_mare(__driver)
void ffly_mare_x11(ffly_mctxp);
void ffly_mare_xcb(ffly_mctxp);
enum {
	_mare_x11,
	_mare_xcb
};

ffly_mctxp ffly_mare(ff_u8_t);
void ffly_mare_cleanup(ffly_mctxp);
# endif /*__ffly__graphics__mare__h*/
