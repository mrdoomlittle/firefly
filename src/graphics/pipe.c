# include "pipe.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
ffly_err_t ffly_grp_prepare(struct ffly_grp *__pipe, mdl_u16_t __bufsize) {
	__pipe->job_buff = (struct ffly_grj**)__ffly_mem_alloc(__bufsize*sizeof(struct ffly_grj*));
	__pipe->fresh_job = __pipe->job_buff;
}

ffly_err_t ffly_grp_inject(struct ffly_grp *__pipe, struct ffly_grj *__job) {

	ffly_mutex_lock(&__pipe->mutex);
	*(__pipe->fresh_job++) = __job;
	ffly_mutex_unlock(&__pipe->mutex);
}

// buffer will be emptyed and all jobs will be done
ffly_err_t ffly_grp_unload_all(struct ffly_grp *__pipe) {
	struct ffly_grj **itr = __pipe->job_buff;
	while(itr < __pipe->fresh_job)
		ffly_grj_prosess(*(itr++));

	__pipe->fresh_job = __pipe->job_buff;
}

ffly_err_t ffly_grp_cleanup(struct ffly_grp *__pipe) {
	__ffly_mem_free(__pipe->job_buff);
}

struct ffly_grp __ffly_grp__;
