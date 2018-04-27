# ifndef __ffly__xcb__wd__h
# define __ffly__xcb__wd__h
# include "../ffint.h"
# include "../types.h"
# include "../system/errno.h"
# include <X11/Xlib.h>
# include <X11/Xlib-xcb.h>
# include <xcb/xproto.h>
# include <xcb/xcb.h>
# include <xcb/xcb_icccm.h>
# include <GL/glx.h>
# include <GL/gl.h>
struct ffly_xcb_wd {
	char const *title;
	ff_uint_t width, height;
	ff_u16_t mn_width, mn_height;
	ff_u16_t mx_width, mx_height;
	Display *d;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_window_t w;
	ff_byte_t *frame_buff;
	GLXWindow glx_window;
	GLXContext glx_ct;
	GLXDrawable glx_dr;
};

# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_xcb_wd_init(struct ffly_xcb_wd*, ff_u16_t, ff_u16_t, char const*);
ff_err_t ffly_xcb_wd_open(struct ffly_xcb_wd*);
ff_err_t ffly_xcb_wd_close(struct ffly_xcb_wd*);
ff_err_t ffly_xcb_wd_cleanup(struct ffly_xcb_wd*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__xcb__wd__h*/
