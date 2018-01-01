# ifndef __ffly__key__event__h
# define __ffly__key__event__h
# include "../types/keycode_t.h"
# include <mdlint.h>
struct ffly_key_event {
    ffly_keycode_t keycode;
    mdl_u8_t type;

};

# endif /*__ffly__key__event__h*/
