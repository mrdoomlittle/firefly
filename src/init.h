# ifndef __ffly__init__h
# define __ffly__init__h
# include "ffint.h"
struct init_arg {
	ff_u8_t flags;
} __attribute__((packed));


extern struct init_arg *__init_arg__;
void ffly_init_run(void);
void ffly_init(ff_u8_t);
# endif /*__ffly__init__h*/
