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
		if (!wk-> wk_state)
			pthread_cond_wait(&wk-> cond, &wk-> mutex);

		if (task_handle::to_shutdown) break;

		if (wk-> wk_state == 0) wk-> wk_state = 1;

		_timer.begin();

		for (uint_t tsk_id = 0; tsk_id != wk-> tsk_indx.size(); tsk_id++)
			wk-> tsk_indx[tsk_id].tsk_call(wk-> tsk_indx[tsk_id].voidptr);

		_timer.end();
		wk-> latency = _timer.delta<std::chrono::microseconds>();
		pthread_mutex_unlock(&wk-> mutex);
	} while(!task_handle::to_shutdown);

	_this-> active_wks--;
	printf("task worker has been killed %d.\n", wk_id);
}
}
}
}
