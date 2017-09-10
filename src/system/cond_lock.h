# ifndef __ffly__cond__lock__h
# define __ffly__cond__lock__h
# include "../types/cond_lock_t.h"
# define FFLY_COND_LOCK_INIT 0
void extern ffly_cond_lock_wait(ffly_cond_lock_t*);
void extern ffly_cond_lock_signal(ffly_cond_lock_t*);
# endif /*__ffly__cond__lock__h*/
