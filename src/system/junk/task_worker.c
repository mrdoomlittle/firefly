# include "task_worker.h"
# include "../data/pair.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/bzero.h"
# include "errno.h"
# include "time.h"
# include "io.h"
# include "nanosleep.h"
void static lock_task(ffly_taskp);
void static unlock_task(ffly_taskp);
void static* handle(void *__arg_p) {
    ffly_fprintf(ffly_out, "task worker running.\n");
    ffly_task_workerp arg = (ffly_task_workerp)__arg_p;
    mdl_u64_t begin = ffly_get_us();
    do {
        arg->latency = ffly_get_us()-begin;

        begin = ffly_get_us();
        ffly_taskp task, next;
        _again:
        task = arg->top;
        if (!task) {
            ffly_nanosleep(0, 100);
            if (ffly_is_flag(arg->flags, FF_TWK_FLG_KILL))
                break;
            goto _again;
        }
        while(task != NULL) {
            next = task->next;
            if (!task->call(task->arg_p)) {
                ffly_rm_task(arg, task);
                __ffly_mem_free(task);
            }
            task = next;
        }
    } while(!ffly_is_flag(arg->flags, FF_TWK_FLG_KILL));
    ffly_add_flag(&arg->flags, FF_TWK_FLG_DEAD, 0);
}

ffly_task_workerp ffly_task_worker(ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    ffly_task_workerp p = (ffly_task_workerp)__ffly_mem_alloc(sizeof(struct ffly_task_worker));
    ffly_bzero(p, sizeof(struct ffly_task_worker));
    p->mutex = FFLY_MUTEX_INIT;
    p->top = NULL;
    *__err = ffly_thread_create(&p->tid, &handle, p);
    return p;  
}

void lock_task(ffly_taskp __task) {
    ffly_mutex_lock(&__task->mutex);
}

void unlock_task(ffly_taskp __task) {
    ffly_mutex_unlock(&__task->mutex);
}

void ffly_add_task(ffly_task_workerp __worker, ffly_taskp __task) {
    __task->mutex = FFLY_MUTEX_INIT;
    __task->next = NULL;
    __task->prev = NULL;
    ffly_mutex_lock(&__worker->mutex);
    if (!__worker->top) {
        __worker->top = __task;
    } else {
        ffly_taskp top = __worker->top;

        top->prev = __task;
        __task->next = top;

        __worker->top = __task;

    }
    ffly_mutex_unlock(&__worker->mutex);
    ffly_atomic_incr(&__worker->task_c);
}

void ffly_rm_task(ffly_task_workerp __worker, ffly_taskp __task) {
    ffly_mutex_lock(&__worker->mutex);
    if (__task == __worker->top) {
        if (__task->next != NULL)
            __task->next->prev = NULL;
        __worker->top = __task->next;
        ffly_mutex_unlock(&__worker->mutex);
    } else {
        ffly_mutex_unlock(&__worker->mutex);
        ffly_taskp prev = __task->prev, next = __task->next;
        if (prev != NULL)
            prev->next = next;
        if (next != NULL)
            next->prev = prev;
    }
    ffly_atomic_decr(&__worker->task_c);
}
