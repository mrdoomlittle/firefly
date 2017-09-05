# include <pthread.h>
# include "thread.h"
ffly_err_t ffly_thread_create(void*(*__p)(void*), void *__arg) {
	pthread_t t;
	pthread_create(&t, NULL, __p, __arg);
}
