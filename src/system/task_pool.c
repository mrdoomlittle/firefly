# include "task_pool.h"
# include "../types/task_worker_t.h"
# include "thread.h"
# include "../types/byte_t.h"
# include "../data/mem_set.h"
# include "../types/id_t.h"
# include "../data/pair.h"
# include "flags.h"
# include "../types/task_t.h"
ffly_err_t ffly_task_pool_init(struct ffly_task_pool *__task_pool, mdl_uint_t __wk_c) {
	ffly_mem_set((void*)&__task_pool->wk, 0, sizeof(struct ffly_vec));
	ffly_vec_init(&__task_pool->wk, sizeof(ffly_task_worker_t));
	ffly_vec_resize(&__task_pool->wk, __wk_c);

	__task_pool->active_workers = 0;
	ffly_task_worker_t *begin = (ffly_task_worker_t*)ffly_vec_begin(&__task_pool->wk);
	for (ffly_task_worker_t *itr = begin; itr < begin+ffly_vec_size(&__task_pool->wk); itr++) {
		__ffly_id_t id = ((ffly_byte_t*)itr-(ffly_byte_t*)begin)/sizeof(ffly_task_worker_t);
		itr->id = id;
		ffly_queue_init(&itr->s_call, sizeof(ffly_task_t));
		itr->flags = TW_FLG_ACTIVE;
		ffly_thread_create(&itr->t, &ffly_task_worker, (void*)ffly_mk_pair((void*)itr, (void*)__task_pool));
	}

	while(ffly_atomic_get(&__task_pool->active_workers) != __wk_c);
}

ffly_err_t ffly_task_pool_de_init(struct ffly_task_pool *__task_pool) {
	ffly_task_worker_t *begin = (ffly_task_worker_t*)ffly_vec_begin(&__task_pool->wk);
	for (ffly_task_worker_t *itr = begin; itr < begin+ffly_vec_size(&__task_pool->wk); itr++) {
		ffly_rm_flag(&itr->flags, TW_FLG_ACTIVE);
	}

	while(ffly_atomic_get(&__task_pool->active_workers) != 0);
	ffly_vec_de_init(&__task_pool->wk);
}
