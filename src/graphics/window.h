# ifndef __ffly__window__h
# define __ffly__window__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/flags.h"
# include "../types/bool_t.h"
# include "../system/mem_blk.h"
# include "../system/mutex.h"
# include "../types/event_t.h"
# include "../types/byte_t.h"
# ifdef __ffly_use_x11
#	include "x11_wd.h"
#	include "x11.h"
# elif __ffly_use_xcb
#	include "xcb_wd.h"
#	include "xcb.h"
# endif
struct ffly_wd {
# ifdef __ffly_use_x11
	struct ffly_x11_wd raw;
# elif __ffly_use_xcb
	struct ffly_xcb_wd raw;
# endif
	ffly_flag_t flags;
	struct ffly_mem_blk events;
};
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_wd_query_pointer(struct ffly_wd*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*);
ffly_err_t ffly_wd_display(struct ffly_wd*);
ffly_byte_t* ffly_wd_frame_buff(struct ffly_wd*);
ffly_flag_t* ffly_wd_flags(struct ffly_wd*);
ffly_err_t ffly_wd_open(struct ffly_wd*);
ffly_err_t ffly_wd_close(struct ffly_wd*);
ffly_err_t ffly_wd_init(struct ffly_wd*, mdl_u16_t, mdl_u16_t, char const*);
ffly_err_t ffly_wd_cleanup(struct ffly_wd*);
ffly_event_t* ffly_wd_poll_event(struct ffly_wd*, ffly_err_t*);
ffly_err_t ffly_wd_free_event(struct ffly_wd*, ffly_event_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__window__h*/
