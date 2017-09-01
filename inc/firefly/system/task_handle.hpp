# ifndef __task__handle__hpp
# define __task__handle__hpp
# include "vec.h"
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
# include <iostream>
# include <deque>
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
		tsk_worker_t() :tsk_queue(){
		}
		pthread_t thread;
		uint_t id, latency;
		std::atomic<int8_t> wk_state;
		pthread_cond_t cond;
		pthread_mutex_t mutex, dmutex;
		types::vp_pair_t arg;
		std::deque<task_t> tsk_queue;
		vec<task_t> tsk_indx;
	};

	bool inited = false;
	static bool to_shutdown;
	std::atomic<uint_t> active_wks;
	types::err_t handle(void *(* __tsk)(void *), void *__voidptr, bool __wake = false, bool __one_call = false) {
		task_t task = {
			.tsk_call = __tsk,
			.voidptr = __voidptr
		};

		uint_t ltq = this-> wk_indx.begin()-> tsk_queue.size();
		uint_t wk_id, ltsk_c = this-> wk_indx.begin()-> tsk_indx.size();
		uint_t llatency = this-> wk_indx.begin()-> latency, llwk_id = 0, ltwk_id = 0, ltq_wd_id = 0;
		for (wk_id = 0; wk_id != this-> wk_indx.size(); wk_id ++) {
			if (wk_id != 0) {
				if (__one_call) {
					if (this-> wk_indx[wk_id].tsk_queue.size() < ltq) {
						ltq_wd_id = wk_id;
						ltq = this-> wk_indx[wk_id].tsk_queue.size();
					}
				} else {
					if (this-> wk_indx[wk_id].tsk_indx.size() < ltsk_c) {
						ltwk_id = wk_id;
						ltsk_c = this-> wk_indx[wk_id].tsk_indx.size();
					}
				}

				if (this-> wk_indx[wk_id].latency < llatency) {
					llwk_id = wk_id;
					llatency = this-> wk_indx[wk_id].latency;
				}
			}

			if (wk_id == this-> wk_indx.size() - 1) {
				if (this-> wk_indx[llwk_id].wk_state == 0 && this-> wk_indx[ltwk_id].wk_state == 0) {
					if (__one_call) {
						if (this-> wk_indx[llwk_id].tsk_indx.size() < this-> wk_indx[ltq_wd_id].tsk_indx.size())
							wk_id = llwk_id;
						else
							wk_id = ltq_wd_id;
						break;
					} else {
						if (this-> wk_indx[llwk_id].tsk_indx.size() < this-> wk_indx[ltwk_id].tsk_indx.size())
							wk_id = llwk_id;
						else
							wk_id = ltwk_id;
						break;
					}
				}


				if (this-> wk_indx[llwk_id].latency < this-> wk_indx[ltwk_id].latency)
					wk_id = llwk_id;
				else
					wk_id = ltwk_id;
				break;
			}
		}

	//	printf("mutex begin.\n");
		pthread_mutex_lock(&this-> wk_indx[wk_id].mutex);
		//if (!this-> wk_indx[wk_id].wk_state) {
			this-> wk_indx[wk_id].wk_state = true;
			pthread_cond_signal(&this-> wk_indx[wk_id].cond);
		//}

		if (__one_call)
			this-> wk_indx[wk_id].tsk_queue.push_back(task);
		else
			this-> wk_indx[wk_id].tsk_indx.push_back() = task;

		pthread_mutex_unlock(&this-> wk_indx[wk_id].mutex);
	//	printf("mutex end.\n");
	//	usleep(1);
		printf("handled to worker with id %d\n", wk_id);
		return FFLY_SUCCESS;
	}

	private:
	vec<tsk_worker_t> wk_indx;
};
extern task_handle __task_handle__;
}
}
}

# endif /*__task__handle__hpp*/
