# ifndef __ffly__slurry__event__h
# define __ffly__slurry__event__h
# include "../ffint.h"
struct s_button {
	ff_int_t x, y;
};

typedef struct s_event {
	ff_u8_t type;
	struct s_button button;
} *s_eventp;

enum {
	_s_event_msg,
	_s_button_press,
	_s_button_release,
	_s_event_unknown
};

# endif /*__ffly_slurry__event__h*/
