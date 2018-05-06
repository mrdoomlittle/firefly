# ifndef __ffly__servant__h
# define __ffly__servant__h
# include "task.h"
# include "mutex.h"
# include "thread.h"
# include "../types.h"
# include "../linux/time.h"
# define FF_SERVANT_KILL 0x1
# define FF_SERVANT_DEAD 0x2

typedef struct ffly_servant {
	ffly_taskp top;	
	ff_mlock_t lock;
	ff_flag_t flags;
	ff_tid_t tid;
	ff_u64_t latency;
} *ffly_servantp;

void ffly_servant_task_add(ffly_servantp, ffly_taskp);
void ffly_servant_task_rm(ffly_servantp, ffly_taskp);
ffly_servantp ffly_servant(ff_err_t*);
void ffly_servant_kill(ffly_servantp);
# endif /*__ffly__servant__h*/
