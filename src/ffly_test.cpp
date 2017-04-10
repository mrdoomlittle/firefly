# include <cstdio>
# include "cuda_helper.hpp"
# include <eint_t.hpp>
# include "system/task_handle.hpp"
using namespace mdl::firefly::system;

void *tsk_0(void *d) {
	printf("t0\n");
	usleep(100000);
}

void *tsk_1(void *d) {
	printf("t1\n");
	usleep(10000);
}

void *tsk_2(void *d) {
	printf("t2\n");
	usleep(100000);
}

int main() {
	task_handle _task_handle;
	_task_handle.init(5);

	_task_handle.handle(&tsk_0, NULL);
	_task_handle.handle(&tsk_1, NULL);
	_task_handle.handle(&tsk_2, NULL);

	_task_handle.wake_wk(0);
	_task_handle.wake_wk(1);
	usleep(5000000);
	_task_handle.de_init();
}
