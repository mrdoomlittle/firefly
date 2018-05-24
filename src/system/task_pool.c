# include "task_pool.h"
# include "../memory/mem_alloc.h"
# include "err.h"
# include "io.h"

ff_err_t static add_servant(ffly_task_poolp);
void ffly_task_pool_init(ffly_task_poolp __pool, ff_uint_t __servants) {
	__pool->servants = (ffly_servantp*)__ffly_mem_alloc(__servants*sizeof(ffly_servantp));
	__pool->fresh = __pool->servants;
	add_servant(__pool);
}

ff_err_t
add_servant(ffly_task_poolp __pool) {
	if (__pool->fresh-__pool->servants >= max_servants) {
		ffly_fprintf(ffly_err, "max servants reached.\n");
		reterr;
	}

	ff_err_t err;
	*(__pool->fresh++) = ffly_servant(&err);
	*__pool->fresh = NULL;
	if (_err(err)) {

	}

	retok;
}

ff_err_t ffly_task_pool_add(ffly_task_poolp __pool, ff_i8_t(*__func)(void*), void *__arg_p) {
	ffly_taskp task = (ffly_taskp)__ffly_mem_alloc(sizeof(struct ffly_task));
	task->func = __func;
	task->arg_p = __arg_p;

	ffly_servantp low = NULL, serv;
	ffly_servantp *itr = __pool->servants;
	low = *(itr++);
	while(itr != __pool->fresh) {
		serv = *(itr++);
		ffly_fprintf(ffly_log, "latency: %u-ms\n", serv->latency);
		if (serv->latency<low->latency) {
			low = serv;
		}

		if (serv->latency<max_latency) {
			ffly_servant_task_add(serv, task);
			goto _end;
		}
	}

	if (!*itr) {
		if (low->latency>=max_latency) {
			if (_ok(add_servant(__pool))) {
				ffly_servant_task_add(*itr, task);
				goto _end;
			}
		}
		ffly_servant_task_add(low, task);
	}
_end:
	retok;
}

void ffly_task_pool_cleanup(ffly_task_poolp __pool) {
	ffly_servantp *cur = __pool->servants;
	while(cur != __pool->fresh)
		ffly_servant_kill(*(cur++));
	__ffly_mem_free(__pool->servants);
}

struct ffly_task_pool __ffly_task_pool__;
