# ifndef __task__handle__hpp
# define __task__handle__hpp
# include "vector.hpp"
# include <eint_t.hpp>
# include "errno.h"
# include <pthread.h>
# include "../types/err_t.hpp"

namespace mdl {
namespace firefly {
namespace system {
class task_handle;
extern void* task_worker(task_handle *__task_handle);

class task_handle {
	types::err_t init(uint_t __tsk_workers);
	struct task_t {
		void *voidptr;
	};

	struct tsk_worker_t {
		pthread_t thread;
	};

	type::err_t handle(void *(* __task)(task_t *));
	private:
	vector<tsk_worker_t> wk_indx;
};
}
}
}

# endif /*__task__handle__hpp*/
