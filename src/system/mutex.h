# ifndef __ffly__mutex__h
# define __ffly__mutex__h
# include "../types/mutex_t.h"
# ifdef __cplusplus
extern "C" {
# endif
void extern ffly_mutex_lock(ffly_mutex_t*);
void extern ffly_mutex_unlock(ffly_mutex_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mutex__h*/
