# ifndef __ffly__task__pool__h
# define __ffly__task__pool__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "queue.h"
# include "vec.h"
# include "atomic.h"
# include "../types/id_t.h"
# include "../types/bool_t.h"
void extern* ffly_task_worker(void*);
struct ffly_task_pool {
	ffly_atomic_uint_t active_workers;
	struct ffly_vec wk;
	__ffly_id_t least_latency;
	mdl_uint_t latency;
	ffly_bool_t inited;
};

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __WITH_TASK_POOL
struct ffly_task_pool __task_pool__;
# endif
ffly_err_t ffly_task_pool_init(struct ffly_task_pool*, mdl_uint_t);
ffly_err_t ffly_task_pool_de_init(struct ffly_task_pool*);
ffly_err_t ffly_task_pool_add(struct ffly_task_pool*, void*(*)(void*), void*, ffly_bool_t, ffly_atomic_uint_t*);
# ifdef __cplusplus
}
# endif
ffly_bool_t __inline__ static ffly_is_task_pool_inited(struct ffly_task_pool *__task_pool) {
	return __task_pool->inited;
}
# endif /*__ffly__task__pool__h*/
