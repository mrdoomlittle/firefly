# ifndef __ffly__mutex__h
# define __ffly__mutex__h
# include "../types/mutex_t.h"
# include "../types/err_t.h"
# define FFLY_MUTEX_INIT 0
# ifdef __cplusplus
extern "C" {
# endif
void ffly_mutex_lock(ffly_mutex_t*);
void ffly_mutex_unlock(ffly_mutex_t*);
ffly_err_t ffly_mutex_trylock(ffly_mutex_t*); 
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mutex__h*/
