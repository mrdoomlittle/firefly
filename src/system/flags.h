# ifndef __ffly__flags__h
# define __ffly__flags__h
# include <mdlint.h>
# include "../types/bool_t.h"
typedef mdl_u8_t ffly_flag_t;

# ifdef __cplusplus
extern "C" {
# endif
ffly_bool_t ffly_is_flag(ffly_flag_t, ffly_flag_t);
void ffly_add_flag(ffly_flag_t*, ffly_flag_t, ffly_bool_t);
void ffly_rm_flag(ffly_flag_t*, ffly_flag_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
typedef ffly_flag_t flag_t;
}
}
}
# endif
# endif /*__ffly__flags__h*/
