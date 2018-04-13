# ifndef __ffly__flags__h
# define __ffly__flags__h
# include "../ffint.h"
# include "../types.h"
# define TW_FLG_ACTIVE 0x1
# ifdef __cplusplus
extern "C" {
# endif
ff_bool_t ffly_is_flag(ff_flag_t, ff_flag_t);
void ffly_add_flag(ff_flag_t*, ff_flag_t, ff_bool_t);
void ffly_rm_flag(ff_flag_t*, ff_flag_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
static ff_bool_t(*is_flag)(ff_flag_t, ff_flag_t) = &ffly_is_flag;
static void(*add_flag)(ff_flag_t*, ff_flag_t, ff_bool_t) = &ffly_add_flag;
static void(*rm_flag)(ff_flag_t*, ff_flag_t) = &ffly_rm_flag;
}
}
}
# endif
# endif /*__ffly__flags__h*/
