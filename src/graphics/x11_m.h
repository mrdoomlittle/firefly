# ifndef __ffly__x11__m__h
# define __ffly__x11__m__h
# include "../ffint.h"
# include "../types.h"
# include "../system/errno.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <stdio.h>

struct ffly_x11_ctx {
	char const *title;
	ff_u16_t width, height;
	ff_u16_t mn_width, mn_height;
	ff_u16_t mx_width, mx_height;
	ff_byte_t *frame_buff;
	Display *d;
	Window w;
	GLXContext glx_ct;
};


# ifdef __cplusplus
extern "C" {
# endif

# ifdef __cplusplus
}
# endif
# endif /*__ffly__x11__m__h*/
