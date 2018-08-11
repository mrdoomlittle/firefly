# ifndef __ffly__init__h
# define __ffly__init__h
# include "ffint.h"
# include "types.h"

#define FF_INIT_SCHED	0
#define FF_INIT_RESV	1
struct init_arg {
	ff_u8_t flags;
} __attribute__((packed));


extern struct init_arg *__init_arg__;
void ffly_init_run(void);
ff_err_t ffly_init(ff_u8_t);
# endif /*__ffly__init__h*/
