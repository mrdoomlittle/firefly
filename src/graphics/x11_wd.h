# ifndef __ffly__x11__wd__h
# define __ffly__x11__wd__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../types/byte_t.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <stdio.h>
# include "../types/off_t.h"
# include "../system/flags.h"
# include "../system/buff.h"
# include "../types/wd_ed_t.h"
# include "../system/mem_blk.h"
struct ffly_x11_wd {
	mdl_uint_t xa_len, ya_len;
	ffly_off_t mn_xal_off, mn_yal_off;
	ffly_off_t mx_xal_off, mx_yal_off;
	ffly_byte_t *frame_buff;
	Display *d;
	Window w;
	ffly_flag_t flags;
	struct ffly_buff event_buff;
	struct ffly_mem_blk event_data;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_x11_wd_begin(struct ffly_x11_wd*, mdl_u16_t, mdl_u16_t, char const*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__x11__wd__h*/
