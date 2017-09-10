# include <mdlint.h>
# include "io.h"
# include "../types/task_worker_t.h"
# include "flags.h"
# include "../data/pair.h"
# include "task_pool.h"
# include "../memory/mem_free.h"
# include "atomic.h"
# include "queue.h"
# include "cond_lock.h"
# include "mutex.h"
void* ffly_task_worker(void *__argp) {
	struct ffly_pair *arg = (struct ffly_pair*)__argp;
	ffly_task_worker_t *c = (ffly_task_worker_t*)arg->p1;
	struct ffly_task_pool *task_pool = (struct ffly_task_pool*)arg->p2;
	ffly_atomic_incr(&task_pool->active_workers);
	ffly_printf(stdout, "worker with id{%u} is now alive.\n", c->id);
	mdl_u8_t is_work = 0;
	while(ffly_is_flag(c->flags, TW_FLG_ACTIVE)) {
		if (!is_work) {
			while(ffly_queue_size(&c->s_call) < 1) {
				usleep(10000);
			}
			is_work = 1;
		}


		ffly_mutex_lock(&c->mutex);

		


		ffly_mutex_unlock(&c->mutex);
	}

	ffly_printf(stdout, "worker with id{%u} has been killed.\n", c->id);
	__ffly_mem_free(__argp);
	ffly_atomic_decr(&task_pool->active_workers);
}
