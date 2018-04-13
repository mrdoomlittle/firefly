# ifndef __ffly__mutex__h
# define __ffly__mutex__h
# include "../types.h"
# define FFLY_MUTEX_INIT 0
# ifdef __cplusplus
extern "C" {
# endif
void ffly_mutex_lock(ff_mlock_t*);
void ffly_mutex_unlock(ff_mlock_t*);
ff_err_t ffly_mutex_trylock(ff_mlock_t*); 
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mutex__h*/
