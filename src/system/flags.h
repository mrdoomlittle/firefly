# ifndef __ffly__flags__h
# define __ffly__flags__h
# include <mdlint.h>
# include "../types/bool_t.h"
# include "../types/flag_t.h"
# define TW_FLG_ACTIVE 0b10000000
# ifdef __cplusplus
extern "C" {
# endif
ffly_bool_t ffly_is_flag(ffly_flag_t, ffly_flag_t);
void ffly_add_flag(ffly_flag_t*, ffly_flag_t, ffly_bool_t);
void ffly_rm_flag(ffly_flag_t*, ffly_flag_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__flags__h*/