# ifndef __ffly__x11__wd__h
# define __ffly__x11__wd__h
# include "../ffint.h"
# include "../types.h"
# include "../system/errno.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <stdio.h>
struct ffly_x11_wd {
	char const *title;
	ff_u16_t width, height;
	ff_u16_t mn_width, mn_height;
	ff_u16_t mx_width, mx_height;
	ff_byte_t *frame_buff;
	Display *d;
	Window w;
};

# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_x11_wd_init(struct ffly_x11_wd*, ff_u16_t, ff_u16_t, char const*);
ff_err_t ffly_x11_wd_open(struct ffly_x11_wd*);
ff_err_t ffly_x11_wd_close(struct ffly_x11_wd*);
ff_err_t ffly_x11_wd_cleanup(struct ffly_x11_wd*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__x11__wd__h*/
