# ifndef __ffly__window__h
# define __ffly__window__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/flags.h"
# include "../types/bool_t.h"
# include "../system/buff.h"
# include "../system/mem_blk.h"
# ifdef __USING_X11
#	include "x11_wd.h"
# elif __USING_XCB
#	include "xcb_wd.h"
# endif
# define EVENT_BUFF_SIZE 20
struct ffly_wd {
# ifdef __USING_X11
	struct ffly_x11_wd raw;
# elif __USING_XCB
	struct ffly_xcb_wd raw;
# endif
	ffly_flag_t flags;
	struct ffly_buff event_buff;
	struct ffly_mem_blk event_data;
};
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_wd_display(struct ffly_wd*);
ffly_byte_t* ffly_wd_frame_buff(struct ffly_wd*);
ffly_flag_t* ffly_wd_flags(struct ffly_wd*);
ffly_err_t ffly_wd_open(struct ffly_wd*);
ffly_err_t ffly_wd_close(struct ffly_wd*);
ffly_err_t ffly_wd_init(struct ffly_wd*, mdl_u16_t, mdl_u16_t, char const*);
ffly_err_t ffly_wd_cleanup(struct ffly_wd*);
ffly_err_t ffly_wd_begin(struct ffly_wd*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__window__h*/
