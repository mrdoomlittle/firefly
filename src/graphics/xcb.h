# ifndef __ffly__xcb__h
# define __ffly__xcb__h
# include "xcb_m.h"
# include "../ffint.h"
# include "../types.h"
# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_xcb_query_pointer(struct ffly_xcb_ctx*, ff_i16_t*, ff_i16_t*, ff_i16_t*, ff_i16_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__xcb__h*/
