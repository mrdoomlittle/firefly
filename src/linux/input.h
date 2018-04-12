# ifndef __ffly__linux__input__h
# define __ffly__linux__input__h
# include "time.h"
# include <mdlint.h>

struct input_event {
	struct timeval time;
	mdl_u16_t type;
	mdl_u16_t code;
	mdl_s32_t value ;
};

# endif /*__ffly__linux__input__h*/
