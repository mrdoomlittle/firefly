# ifndef __ffly__cond__lock__h
# define __ffly__cond__lock__h
# include "../types.h"
# define FFLY_COND_LOCK_INIT 0x0
void ffly_cond_lock_wait(ff_cond_lock_t*);
void ffly_cond_lock_signal(ff_cond_lock_t*);
# endif /*__ffly__cond__lock__h*/
