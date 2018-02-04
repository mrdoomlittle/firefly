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
		usleep(599);
//		if (ffly_mutex_trylock(mutex) == FFLY_SUCCESS) {
        ffly_mutex_lock(mutex);
			usleep(200);
			(*ii)++;
		ffly_mutex_unlock(mutex);
			i++;
//		}
	}
}

# include <linux/sched.h>
# include "../../linux/types.h"
# include "../../linux/signal.h"
int ffly_kill(__linux_pid_t, int);
__linux_pid_t ffly_fork();
# define no_process 8
void ffly_exit(int);
__linux_pid_t ffly_getpid();
mdl_i64_t ffly_clone(mdl_u64_t, mdl_u64_t, int*, int*, mdl_u64_t);

void test() {
	printf("Hello\n");	

	ffly_exit(SIGKILL);
}

int main() {
	void *stack;
	stack = mmap(NULL, 2048, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	*(void**)((mdl_u8_t*)stack+(2048-8)) = (void*)test;

	int a, b;
	mdl_u64_t r = ffly_clone(CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS, (mdl_u8_t*)stack+2048-8, NULL, NULL, 0);
	printf("bye. %u\n", r);	
	while(1);
/*
	mutex = mmap(NULL, sizeof(ffly_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	ii = mmap(NULL, sizeof(mdl_uint_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	*mutex = FFLY_MUTEX_INIT;

    ffly_mutex_lock(mutex);
    ffly_mutex_unlock(mutex);
    printf("init.\n");
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
*/
}
