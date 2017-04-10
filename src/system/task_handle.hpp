# ifndef __task__handle__hpp
# define __task__handle__hpp
# include "vector.hpp"
# include <eint_t.hpp>
# include "errno.h"
# include <pthread.h>
# include "../types/err_t.h"
# include <cstdio>
# include <atomic>
# include <errno.h>
# include <semaphore.h>
# include "../types/vp_pair_t.h"
# include "timer.hpp"
namespace mdl {
namespace firefly {
namespace system {
class task_handle;
extern void* task_worker(void *__task_handle);

class task_handle {
	friend void* task_worker(void *__task_handle);
	public:

	struct task_t {
		void *(* tsk_call)(void *);
		void *voidptr;
	};

	types::err_t init(uint_t __tsk_workers);
	types::err_t de_init();
	struct tsk_worker_t {
		pthread_t thread;
		uint_t id, latency;
		int8_t wk_state = 0;
		pthread_cond_t cond;
		pthread_mutex_t mutex;
		types::vp_pair_t arg;
		vector<task_t> tsk_indx;
	};

	static bool to_shutdown;
	std::atomic<uint_t> active_wks;
	types::err_t handle(void *(* __tsk)(void *), void *__voidptr, bool __begin = false) {
		task_t task = {
			.tsk_call = __tsk,
			.voidptr = __voidptr
		};

		uint_t wk_id;
		uint_t llatency = this-> wk_indx.begin()-> latency, llwk_id = 0;
		for (wk_id = 0; wk_id != this-> wk_indx.size(); wk_id ++) {
			if (this-> wk_indx[wk_id].wk_state == 0) break;

			if (wk_id != 0) {
				if (this-> wk_indx[wk_id].latency < llatency) {
					llwk_id = wk_id;
				}
			}

			if (wk_id == this-> wk_indx.size() - 1) {
				wk_id = llwk_id;
				break;
			}
		}

		this-> wk_indx[wk_id].tsk_indx.push_back() = task;
		if (__begin) pthread_cond_signal(&this-> wk_indx[wk_id].cond);
		printf("task was handed to worker %d\n", wk_id);
		return FFLY_SUCCESS;
	}

	void wake_wk(uint_t __wk_id) {
		pthread_cond_signal(&this-> wk_indx[__wk_id].cond);
	}

	private:
	vector<tsk_worker_t> wk_indx;
};
}
}
}

# endif /*__task__handle__hpp*/
