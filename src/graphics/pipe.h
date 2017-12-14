# ifndef __ffly__graphics__pipe__h
# define __ffly__graphics__pipe__h
# include "../types/err_t.h"
# include "../system/mutex.h"
# include <mdlint.h>
# include "job.h"
# include "../system/signal.h"
# include "../types/bool_t.h"
struct ffly_grp {
	ffly_sig_t sig;
	mdl_u16_t bufsize;
	ffly_mutex_t mutex;
	struct ffly_grj **job_buff;
	struct ffly_grj **fresh_job;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_bool_t ffly_grp_buff_empty(struct ffly_grp*);
ffly_err_t ffly_grp_prepare(struct ffly_grp*, mdl_u16_t);
ffly_err_t ffly_grp_inject(struct ffly_grp*, struct ffly_grj*);
ffly_err_t ffly_grp_unload_all(struct ffly_grp*);
ffly_err_t ffly_grp_cleanup(struct ffly_grp*);
extern struct ffly_grp __ffly_grp__;
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
static ffly_err_t(*pipe_prepare)(struct ffly_grp*, mdl_u16_t) = &ffly_grp_prepare;
static ffly_err_t(*pipe_inject)(struct ffly_grp*, struct ffly_grj*) = &ffly_grp_inject;
static ffly_err_t(*pipe_unload_all)(struct ffly_grp*) = &ffly_grp_unload_all;
static ffly_err_t(*pipe_cleanup)(struct ffly_grp*) = &ffly_grp_cleanup;
struct pipe {
	types::err_t inject(struct ffly_grj& __job) {
		return pipe_inject(&this->raw, &__job);
	}

	types::err_t unload_all() {
		return pipe_unload_all(&this->raw);
	}

	types::err_t prepare(mdl_u16_t __bufsize) {
		return pipe_prepare(&this->raw, __bufsize);
	}

	types::err_t cleanup() {
		return pipe_cleanup(&this->raw);
	}

	private:
	struct ffly_grp raw;
};
}
}
}
# endif

# endif /*__ffly__graphics__pipe__h*/
