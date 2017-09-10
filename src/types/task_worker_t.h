# ifndef __ffly__task__worker__t__h
# define __ffly__task__worker__t__h
# include "id_t.h"
# include "../system/queue.h"
# include "flag_t.h"
# include "thread.h"
# include "../system/mutex.h"
# include "../system/signal.h"
typedef struct {
	ffly_sig_t sig;
	ffly_tid_t t;
	ffly_mutex_t mutex;
	ffly_flag_t flags;
	__ffly_id_t id;
	struct ffly_queue s_call;
} ffly_task_worker_t;

# endif /*__ffly__task__worker__t__h*/
