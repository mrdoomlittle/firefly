# include "task_handle.hpp"
mdl::firefly::types::err_t mdl::firefly::system::task_handle::init(uint_t __tsk_workers) {
	this-> wk_indx.resize(__tsk_workers);
	for (uint_t tsk_worker = 0; tsk_worker != __tsk_workers; tsk_worker ++) {
		pthread_t& th = this-> wk_indx[tsk_worker].thread;
		pthread_create(&task_worker, NULL, th, this);
	}
}
