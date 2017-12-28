# include <mdlint.h>
# include <stdio.h>
# include <sys/mman.h>
# include <unistd.h>
# include "../mutex.h"
# include <sys/types.h>
# include "../errno.h"
# define pp 200
ffly_mutex_t *mutex;
mdl_uint_t *ii = 0;

void call() {
	mdl_uint_t i = 0;
	while(i != pp) {
		usleep(5999);
		if (ffly_mutex_trylock(mutex) == FFLY_SUCCESS) {
			(*ii)++;
		ffly_mutex_unlock(mutex);
			i++;
		}
	}
}

# define no_process 20

int main() {
	mutex = mmap(NULL, sizeof(ffly_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	ii = mmap(NULL, sizeof(mdl_uint_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	*mutex = FFLY_MUTEX_INIT;
	mdl_uint_t p = 0;
	while(p != no_process) {
		pid_t child = fork();
		if (child == 0) {
			call();
			_exit(0);
		}
		p++;
	}

	while(*ii != (no_process*pp)) {
		usleep(1000);
		ffly_mutex_lock(mutex);
		printf("%u\n", *ii);
		ffly_mutex_unlock(mutex);
	}
	munmap(mutex, sizeof(ffly_mutex_t));
	munmap(ii, sizeof(mdl_uint_t));
}
