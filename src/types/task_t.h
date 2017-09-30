# ifndef __ffly__task__t__h
# define __ffly__task__t__h
# include "../system/atomic.h"
typedef struct {
	void*(*call)(void*);
	void *arg_p;
	ffly_atomic_uint_t *f;
} ffly_task_t;

# endif /*__ffly__task__t__h*/
