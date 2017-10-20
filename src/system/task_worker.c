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
# include "../types/task_t.h"
# include "time.h"
void* ffly_task_worker(void *__argp) {
	struct ffly_pair *arg = (struct ffly_pair*)__argp;
	ffly_task_worker_t *c = (ffly_task_worker_t*)arg->p1;
	struct ffly_task_pool *task_pool = (struct ffly_task_pool*)arg->p2;
	ffly_atomic_incr(&task_pool->active_workers);
	ffly_printf(stdout, "worker with id{%u} is now alive.\n", c->id);
	mdl_uint_t begin = 0;
	while(ffly_is_flag(c->flags, TW_FLG_ACTIVE)) {
		mdl_uint_t end = ffly_get_us_tp();
		c->latency = ffly_get_us_tp()-begin;
		if (c->latency < task_pool->latency)
			task_pool->least_latency = (c->id << 1)|1;
		begin = ffly_get_us_tp();

		ffly_mutex_lock(&c->mutex);
		if (ffly_vec_size(&c->tasks) != 0) {
			ffly_task_t *itr =  (ffly_task_t*)ffly_vec_begin(&c->tasks);
			while(itr != (ffly_task_t*)ffly_vec_end(&c->tasks)+1) {
				itr->call(itr->arg_p);
				itr++;
			}
		}

		ffly_mutex_unlock(&c->mutex);
		__asm__("nop");

		if (!ffly_queue_size(&c->s_call) && !ffly_vec_size(&c->tasks)) usleep(10000);
		if (ffly_queue_size(&c->s_call) > 0) goto _sk_cstmt;
		continue;
		_sk_cstmt:

		ffly_mutex_lock(&c->mutex);
		if (ffly_queue_size(&c->s_call) < 1) goto _sk_to_end;

		while(ffly_queue_size(&c->s_call) != 0) {
			ffly_task_t task;
			ffly_queue_pop(&c->s_call, &task);
			task.call(task.arg_p);
			if (task.f != NULL) ffly_atomic_incr(task.f);
		}

		_sk_to_end:
		ffly_mutex_unlock(&c->mutex);
		__asm__("nop");
	}

	ffly_printf(stdout, "worker with id{%u} has been killed.\n", c->id);
	if (__argp != NULL)
		__ffly_mem_free(__argp);
	ffly_atomic_decr(&task_pool->active_workers);
}
