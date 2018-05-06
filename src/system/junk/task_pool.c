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
    __pool->workers = (ffly_task_workerpp)__ffly_mem_alloc(__max*sizeof(ffly_task_workerp));
    *__pool->workers = NULL;
    __pool->fresh = __pool->workers;
    return FFLY_SUCCESS;
}

ffly_err_t static add_worker(ffly_task_poolp __pool) {
    ffly_err_t err;
    if ((__pool->fresh-__pool->workers) >= __pool->max_workers) {
        ffly_fprintf(ffly_err, "max workers reached.\n");
        return FFLY_FAILURE;
    }

    *(__pool->fresh++) = ffly_task_worker(&err);
    if (_err(err)) {
        ffly_fprintf(ffly_err, "failed to create worker.\n");
        return err;
    }
    *__pool->fresh = NULL;
    return FFLY_SUCCESS;
}

mdl_i8_t static proxy(void *__arg_p) {

}

ffly_err_t ffly_task_pool_add(ffly_task_poolp __pool, mdl_i8_t(*__call)(void*), void *__arg_p) {
    ffly_err_t err;
    ffly_taskp task = ffly_alloc_task();
    task->call = __call;
    task->arg_p = __arg_p;
    if (!*__pool->workers) {
        if (_err(err = add_worker(__pool))) {
            ffly_fprintf(ffly_err, "failed to add worker.\n");
            return err;
        }
    }

    ffly_task_workerp low, worker;
    ffly_task_workerpp itr = __pool->workers;
    low = *itr;
    while(*itr != NULL) {
        worker = *itr;
        ffly_fprintf(ffly_log, "latency: %lu\n", worker->latency);
        if (worker->latency < low->latency && worker->task_c < low->task_c)
            low = worker;
        if (worker->latency < max_latency && worker->task_c < max_tasks) {
            ffly_fprintf(ffly_log, "added task, %u.\n", worker->tid);
            ffly_add_task(worker, task);
            break;
        } else {
            if (*(itr+1) == NULL) {
                if (_err(err = add_worker(__pool))) {
                    ffly_fprintf(ffly_err, "failed to add worker.\n");
                    if (low != NULL) {
                        ffly_add_task(low, task);
                        return FFLY_SUCCESS;
                    }
                    return err;
                }
            }                
            ffly_fprintf(ffly_log, "worker latency too high, skipping...\n");
        }
        itr++;
    }
    return FFLY_SUCCESS;
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
    return FFLY_SUCCESS;
}
