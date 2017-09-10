# include "io.h"
# include "task_pool.h"
# include "thread.h"
# include <unistd.h>
# include "task_pool.h"
# include "mutex.h"
# include <pthread.h>
# include "signal.h"

ffly_sig_t sig = 0;
void *call(void *__arg_p) {
	printf("waiting for signal.\n");
	ffly_sigwait(&sig);
	printf("signaled.\n");
}

int main() {
//	struct ffly_task_pool task_pool;
///	ffly_task_pool_init(&task_pool, 4);
//	ffly_task_pool_de_init(&task_pool);
	ffly_tid_t tid;
	ffly_thread_create(&tid, &call, NULL);
	usleep(5000000);

	printf("sending sig.\n");
	ffly_signal(&sig);
	printf("send sig.\n");

	usleep(5000000);
	ffly_thread_cleanup();

/*
	ffly_tid_t tids[20];
	for (mdl_uint_t i = 0; i != 20; i++) {
		ffly_thread_create(&tids[i], NULL, NULL);
		printf("%u\n", tids[i]);
	}
*/
}
