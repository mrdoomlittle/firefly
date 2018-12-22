# ifndef __ffly__workshop__h
# define __ffly__workshop__h
# include "ffint.h"
# include "types.h"
# include "pallet.h"
#define WIDTH ws_width
#define HEIGHT ws_height
extern ff_uint_t ws_width;
extern ff_uint_t ws_height;
struct ff_workshop {
	void(*de_init)(void);
	void(*get)(ff_u8_t, long long, void*);
};

extern void(*tick)(void);
struct ff_workshop extern workshop;
# endif /*__ffly__workshop__h*/
