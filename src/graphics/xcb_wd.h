# ifndef __ffly__xcb__wd__h
# define __ffly__xcb__wd__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/errno.h"
# include <X11/Xlib.h>
# include <X11/Xlib-xcb.h>
# include <xcb/xproto.h>
# include <xcb/xcb.h>
# include <xcb/xcb_icccm.h>
# include <GL/glx.h>
# include <GL/gl.h>
# include "../system/flags.h"
# include "../types/byte_t.h"
# include "../system/buff.h"

struct ffly_xcb_wd {
	mdl_uint_t width, height;
	mdl_u16_t mn_width, mn_height;
	mdl_u16_t mx_width, mx_height;
	Display *d;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_window_t w;
	ffly_byte_t *frame_buff;
	ffly_flag_t flags;
	struct ffly_buff event_buff;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_xcb_wd_begin(struct ffly_xcb_wd*, mdl_u16_t, mdl_u16_t, char const*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__xcb__wd__h*/
