# include "task_pool.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "err.h"
# include "io.h"
# include "errno.h"
# include "task.h"
# include "nanosleep.h"
struct ffly_task_pool __ffly_task_pool__;
ffly_err_t ffly_task_pool_init(ffly_task_poolp __pool, mdl_uint_t __max) {
    __pool->max_workers = __max;
    __pool->workers = (ffly_task_workerp*)__ffly_mem_alloc(sizeof(ffly_task_workerp));
    *__pool->workers = NULL;
    __pool->fresh = __pool->workers;
}

ffly_err_t ffly_task_pool_add(ffly_task_poolp __pool, mdl_i8_t(*__call)(void*), void *__arg_p) {
    ffly_err_t err;
    ffly_taskp task = ffly_alloc_task();
    task->call = __call;
    task->arg_p = __arg_p;
    if (!*__pool->workers) {
        ffly_fprintf(ffly_out, "creating task worker.\n");
        *__pool->fresh = ffly_task_worker(&err);
        if (_err(err)) {
            return err;
        }
        *(++__pool->fresh) = NULL;
    }

    ffly_task_workerp *itr = __pool->workers;
    while(*itr != NULL) {
        if ((*itr)->latency < max_latency) {
            ffly_fprintf(ffly_out, "added task.\n");
            ffly_add_task(*itr, task);
        } else
            ffly_fprintf(ffly_out, "worker latency too high, skipping...\n");
        itr++;
    }
}

ffly_err_t ffly_task_pool_cleanup(ffly_task_poolp __pool) {
    if (*__pool->workers != NULL) {
        ffly_task_workerp *itr = __pool->workers;
        while(*itr != NULL) {
            ffly_add_flag(&(*itr)->flags, FF_TWK_FLG_KILL, 0);
            while(!ffly_is_flag((*itr)->flags, FF_TWK_FLG_DEAD)) {
                ffly_nanosleep(0, 10000);
            }
            itr++;
        }
    }

    __ffly_mem_free(__pool->workers);
}
