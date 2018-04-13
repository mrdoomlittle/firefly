# ifndef __ffly__linux__input__h
# define __ffly__linux__input__h
# include "../ffint.h"
# include "time.h"

struct input_event {
	struct timeval time;
	ff_u16_t type;
	ff_u16_t code;
	ff_s32_t value ;
};

# endif /*__ffly__linux__input__h*/
