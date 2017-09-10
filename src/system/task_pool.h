# ifndef __ffly__task__pool__h
# define __ffly__task__pool__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "queue.h"
# include "vec.h"
# include "atomic.h"
void extern* ffly_task_worker(void*);
struct ffly_task_pool {
	ffly_atomic_uint_t active_workers;
	struct ffly_vec wk;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_task_pool_init(struct ffly_task_pool*, mdl_uint_t);
ffly_err_t ffly_task_pool_de_init(struct ffly_task_pool*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__task__pool__h*/
