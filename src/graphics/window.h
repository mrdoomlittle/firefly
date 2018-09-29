# ifndef __ffly__window__h
# define __ffly__window__h
# include "../ffint.h"
# include "../types.h"
# include "../system/flags.h"
# include "../system/pool.h"
# include "../system/mutex.h"
# include "../event.h"
# ifdef __ffly_use_x11
#	include "x11_m.h"
#	include "x11.h"
# elif __ffly_use_xcb
#	include "xcb_m.h"
#	include "xcb.h"
# endif
# include "mare.h"
struct ffly_wd {
	ffly_mctxp m;
	ff_flag_t flags;
	struct ffly_pool events;
};
# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_wd_query_pointer(struct ffly_wd*, ff_i16_t*, ff_i16_t*, ff_i16_t*, ff_i16_t*);
ff_err_t ffly_wd_display(struct ffly_wd*);
ff_byte_t* ffly_wd_frame_buff(struct ffly_wd*);
ff_flag_t* ffly_wd_flags(struct ffly_wd*);
ff_err_t ffly_wd_open(struct ffly_wd*);
ff_err_t ffly_wd_close(struct ffly_wd*);
ff_err_t ffly_wd_init(struct ffly_wd*, ff_u16_t, ff_u16_t, char const*);
ff_err_t ffly_wd_cleanup(struct ffly_wd*);
ffly_event_t* ffly_wd_poll_event(struct ffly_wd*, ff_err_t*);
ff_err_t ffly_wd_free_event(struct ffly_wd*, ffly_event_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__window__h*/
