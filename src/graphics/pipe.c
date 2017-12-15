# include "pipe.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../system/errno.h"
# include "../system/io.h"
ffly_err_t ffly_grp_prepare(struct ffly_grp *__pipe, mdl_u16_t __bufsize) {
	if ((__pipe->job_buff = (struct ffly_grj**)__ffly_mem_alloc(__bufsize*sizeof(struct ffly_grj*))) == NULL) {
		ffly_fprintf(ffly_err, "failed to allocate memory for job buffer.\n");
		return FFLY_FAILURE;
	}
	__pipe->fresh_job = __pipe->job_buff;
	__pipe->bufsize = __bufsize;
	__pipe->sig = FFLY_SIG_INIT;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_grp_inject(struct ffly_grp *__pipe, struct ffly_grj *__job) {
	ffly_mutex_lock(&__pipe->mutex);
	if (__pipe->fresh_job >= __pipe->job_buff+__pipe->bufsize) {
		ffly_fprintf(ffly_err, "job buffer full.\n");
		ffly_mutex_unlock(&__pipe->mutex);
		return FFLY_FAILURE;
	}
/*
	if (__pipe->fresh_job == __pipe->job_buff+__pipe->bufsize) {
		ffly_mutex_unlock(&__pipe->mutex);
		ffly_sigwait(&__pipe->sig);
		ffly_mutex_lock(&__pipe->mutex);
	}
*/
	*(__pipe->fresh_job++) = __job;
	ffly_mutex_unlock(&__pipe->mutex);
	return FFLY_SUCCESS;
}

ffly_bool_t ffly_grp_buff_empty(struct ffly_grp *__pipe) {
	return ((__pipe->job_buff-__pipe->fresh_job) == 0);
}

# include "../system/err.h"
// buffer will be emptyed and all jobs will get processed
ffly_err_t ffly_grp_unload_all(struct ffly_grp *__pipe) {
	ffly_err_t err;
	ffly_mutex_lock(&__pipe->mutex);
	struct ffly_grj **itr = __pipe->job_buff;
	while(itr < __pipe->fresh_job) {
		if (_err(err = ffly_grj_prosess(*(itr++)))) {
			ffly_fprintf(ffly_err, "failed to prossess graphics job.\n");
			break;
		}
	}

	__pipe->fresh_job = __pipe->job_buff;
	ffly_mutex_unlock(&__pipe->mutex);
//	ffly_signal(&__pipe->sig);
	if (_err(err)) return err;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_grp_cleanup(struct ffly_grp *__pipe) {
	__ffly_mem_free(__pipe->job_buff);
	return FFLY_SUCCESS;
}

struct ffly_grp __ffly_grp__;
