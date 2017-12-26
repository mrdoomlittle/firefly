# ifndef __ffly__xcb__h
# define __ffly__xcb__h
# include "xcb_wd.h"
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_xcb_query_pointer(struct ffly_xcb_wd*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__xcb__h*/
