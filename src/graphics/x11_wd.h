# ifndef __ffly__x11__wd__h
# define __ffly__x11__wd__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../types/byte_t.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <stdio.h>
struct ffly_x11_wd {
	char const *title;
	mdl_u16_t width, height;
	mdl_u16_t mn_width, mn_height;
	mdl_u16_t mx_width, mx_height;
	ffly_byte_t *frame_buff;
	Display *d;
	Window w;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_x11_wd_init(struct ffly_x11_wd*, mdl_u16_t, mdl_u16_t, char const*);
ffly_err_t ffly_x11_wd_open(struct ffly_x11_wd*);
ffly_err_t ffly_x11_wd_close(struct ffly_x11_wd*);
ffly_err_t ffly_x11_wd_cleanup(struct ffly_x11_wd*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__x11__wd__h*/
