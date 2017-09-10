# ifndef __ffly__task__t__h
# define __ffly__task__t__h

typedef struct {
	void*(*call)(void*);
	void *arg_p;
} ffly_task_t;

# endif /*__ffly__task__t__h*/
