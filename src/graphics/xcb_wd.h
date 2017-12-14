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
# include "../types/byte_t.h"
struct ffly_xcb_wd {
	char const *title;
	mdl_uint_t width, height;
	mdl_u16_t mn_width, mn_height;
	mdl_u16_t mx_width, mx_height;
	Display *d;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_window_t w;
	ffly_byte_t *frame_buff;
	GLXWindow glx_window;
	GLXContext glx_ct;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_xcb_wd_init(struct ffly_xcb_wd*, mdl_u16_t, mdl_u16_t, char const*);
ffly_err_t ffly_xcb_wd_open(struct ffly_xcb_wd*);
ffly_err_t ffly_xcb_wd_close(struct ffly_xcb_wd*);
ffly_err_t ffly_xcb_wd_cleanup(struct ffly_xcb_wd*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__xcb__wd__h*/
