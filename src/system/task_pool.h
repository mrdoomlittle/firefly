# ifndef __ffly__task__pool__h
# define __ffly__task__pool__h
# include "../ffint.h"
# include "servant.h"
# define max_servants 2
# define max_latency 1 //ms
typedef struct ffly_task_pool {
	ffly_servantp *servants;
	ffly_servantp *fresh;
} *ffly_task_poolp;

void ffly_task_pool_init(ffly_task_poolp, ff_uint_t);
void ffly_task_pool_cleanup(ffly_task_poolp);
ff_err_t ffly_task_pool_add(ffly_task_poolp, ff_i8_t(*)(void*), void*);
# endif /*__ffly__task__pool__h*/
