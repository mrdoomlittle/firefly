# include "task_handle.hpp"
namespace mdl {
namespace firefly {
namespace system {
void* task_worker(void *__arg) {
	types::vp_pair_t *arg = (types::vp_pair_t *)__arg;

	task_handle *_this = (task_handle *)arg-> second;
	uint_t wk_id = *(uint_t *)arg-> first;

	task_handle::tsk_worker_t *wk = &_this-> wk_indx[wk_id];

	_this-> active_wks++;
	printf("task worker has started with id: %d.\n", wk_id);

	timer _timer;
	do {
		pthread_mutex_lock(&wk-> mutex);
		while (!wk-> wk_state)
			pthread_cond_wait(&wk-> cond, &wk-> mutex);

		if (task_handle::to_shutdown) break;

		_timer.begin();

		if (!wk-> tsk_queue.empty()) {
			wk-> tsk_queue.front().tsk_call(wk-> tsk_queue.front().voidptr);
			wk-> tsk_queue.pop_front();
		}

		for (uint_t tsk_id = 0; tsk_id != wk-> tsk_indx.size(); tsk_id++)
			wk-> tsk_indx[tsk_id].tsk_call(wk-> tsk_indx[tsk_id].voidptr);

		_timer.end();
		wk-> latency = _timer.delta<std::chrono::microseconds>();
		if (wk-> tsk_indx.empty() && wk-> tsk_queue.empty()) wk-> wk_state = false;
		pthread_mutex_unlock(&wk-> mutex);
		__asm__("nop");
	} while(!task_handle::to_shutdown);

	_this-> active_wks--;
	printf("task worker has been killed %d.\n", wk_id);
}
}
}
}
