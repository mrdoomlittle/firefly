# ifndef __ffly__x11__h
# define __ffly__x11__h
# include "x11_wd.h"
# include "../types/err_t.h"
# include <mdlint.h>
# include "../types/keycode_t.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_x11_query_pointer(struct ffly_x11_wd*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*, mdl_i16_t*);
ffly_keycode_t ffly_x11_convert_keycode(int unsigned);
mdl_u8_t ffly_x11_convert_btnno(int unsigned);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__x11__h*/
