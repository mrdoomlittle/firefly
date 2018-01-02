# ifndef __ffly__task__pool__h
# define __ffly__task__pool__h
# include "../types/err_t.h"
# include "task_worker.h"
# define max_latency 1000000
struct ffly_task_pool {
    ffly_task_workerp *workers;    
    ffly_task_workerp *fresh;
    mdl_uint_t max_workers;
};

typedef struct ffly_task_pool* ffly_task_poolp;
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_task_pool_init(ffly_task_poolp, mdl_uint_t);
ffly_err_t ffly_task_pool_cleanup(ffly_task_poolp);
ffly_err_t ffly_task_pool_add(ffly_task_poolp, mdl_i8_t(*)(void*), void*);
# ifdef __ffly_use_task_pool
extern struct ffly_task_pool __ffly_task_pool__; 
# endif
# ifdef __cplusplus
}
# endif
# endif /*__ffly__task__pool__h*/
