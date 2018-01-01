# ifndef __ffly__wd__event__t__h
# define __ffly__wd__event__t__h
# include <mdlint.h>
# include "../types/keycode_t.h"
typedef struct {
	ffly_keycode_t keycode, btn;
	mdl_i16_t x, y;
} ffly_wd_event_t;
#ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_wd_event_t wd_event_t;
}
}
}
# endif
# endif /*__ffly__wd__event__t*/
