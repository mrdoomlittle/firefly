# ifndef __ffly__task__worker__h
# define __ffly__task__worker__h
# include "task.h"
# include "flags.h"
# include "thread.h"
# include "mutex.h"
# include "atomic.h"
# define FF_TWK_FLG_KILL 0x1
# define FF_TWK_FLG_DEAD 0x2
struct ffly_task_worker {
    ffly_taskp top;
    ffly_flag_t flags;
    ffly_tid_t tid;
    mdl_u64_t latency;
    ffly_mutex_t mutex;
    ffly_atomic_uint_t task_c;
};

typedef struct ffly_task_worker* ffly_task_workerp;
typedef ffly_task_workerp* ffly_task_workerpp;
ffly_task_workerp ffly_task_worker(ffly_err_t*);
void ffly_add_task(ffly_task_workerp, ffly_taskp);
void ffly_rm_task(ffly_task_workerp, ffly_taskp);
# endif /*__ffly__task__worker__h*/
