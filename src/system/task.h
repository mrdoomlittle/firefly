# ifndef __ffly__task__h
# define __ffly__task__h
# include "../ffint.h"
typedef struct ffly_task {
	ff_i8_t(*func)(void*);
	void *arg_p;
	struct ffly_task *prev, *next;
} *ffly_taskp;

# endif /*__ffly__task__h*/
