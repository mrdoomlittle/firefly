# include "task_handle.hpp"
bool mdl::firefly::system::task_handle::to_shutdown = false;

mdl::firefly::types::err_t mdl::firefly::system::task_handle::init(uint_t __tsk_workers) {
	this-> wk_indx.resize(__tsk_workers);
	this-> active_wks = 0;
	this-> inited = true;
	printf("creating workers.\n");
	for (uint_t wk_id = 0; wk_id != __tsk_workers; wk_id ++) {
		new (this-> wk_indx.begin() - wk_id) task_handle::tsk_worker_t();
		pthread_t& th = this-> wk_indx[wk_id].thread;
		this-> wk_indx[wk_id].wk_state = false;
		this-> wk_indx[wk_id].id = wk_id;
		this-> wk_indx[wk_id].cond = PTHREAD_COND_INITIALIZER;
		this-> wk_indx[wk_id].mutex = PTHREAD_MUTEX_INITIALIZER;
		this-> wk_indx[wk_id].dmutex = PTHREAD_MUTEX_INITIALIZER;
		this-> wk_indx[wk_id].arg.first = &this-> wk_indx[wk_id].id;
		this-> wk_indx[wk_id].arg.second = this;

		if (pthread_create(&th, NULL, &task_worker, &this-> wk_indx[wk_id].arg) != 0) {
			fprintf(stderr, "task_handle: failed to create pthread, errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	printf("waiting for all workers.\n");
	while(this-> active_wks != __tsk_workers) {}
	printf("all workers are now active and waiting for tasks.\n");

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::system::task_handle::de_init() {
	task_handle::to_shutdown = true;
	printf("killing all workers.\n");
	uint_t active_wks = this-> active_wks;

	for (uint_t wk_id = 0; wk_id != active_wks + 1; wk_id ++) {
		if (!this-> wk_indx[wk_id].wk_state)
			pthread_cond_signal(&this-> wk_indx[wk_id].cond);
	}

	while (this-> active_wks != 0){}

	printf("all workers have been killed\n");
	return FFLY_SUCCESS;
}

mdl::firefly::system::task_handle ___task_handle__;




