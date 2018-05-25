# define __ffly_debug_enabled
# include "types.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"
# include "system/nanosleep.h"
# include "ctl.h"
# include "string.h"
/*
	only for testing allocr need more woking on
*/
ffly_potp pot;
void *p = NULL;

void *thr(void *__arg_p) {
/*
	free(malloc(200));
	free(p);
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
	//free(malloc(200));
	
	ffly_ctl(ffly_malc, _ar_unset, 0);
*/
ffly_printf("hello.\n");
}


# include "system/errno.h"
# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
# include "linux/wait.h"
ff_i8_t run = -1;
void sig(int __no) {
	run = 0;
}
/*
void restore();
__asm__("restore:mov $15,%rax\n\t"
		"syscall");
*/

ff_i8_t test(void *__arg_p) {
	ffly_printf("....\n");
	//ffly_nanosleep(1, 0);
	return -1;
}
# include "signal.h"
# include "maths/round.h"
# include "maths/floor.h"
# include "maths/ceil.h"
# include "maths/sin.h"
# include "maths/cos.h"
# include "system/errno.h"
# include "linux/limits.h"
# include "system/sched.h"
# include "linux/time.h"
# include "system/servant.h"
# include "system/task_pool.h"
# include "stores.h"
# include "system/tls.h"
# include "linux/prctl.h"
# include "system/queue.h"
void showfsreg() {
	void *p = NULL;
	if (arch_prctl(ARCH_GET_GS, &p) == -1) {
		ffly_printf("error, %s\n", strerror(errno));	
	}
	ffly_printf("fs: %p\n", p);
}
ff_uint_t tls_test;
# include "linux/mman.h"
# include "system/thread.h"
void *tls;
void* th(void *__arg) {
}
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_queue queue;
	ffly_queue_init(&queue, 1);
	ff_u8_t a;
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_de_init(&queue);
//tls = p;
//	ff_tid_t id;
//	ffly_thread_create(&id, th, NULL);
//	ffly_thread_wait(id);
//ls_test = ffly_tls_alloc(sizeof(ff_uint_t));


//	struct ffly_task_pool pool;
//	ffly_task_pool_init(&pool, 4);

//	ff_u8_t i = 0;

//	while(i++ != 20) {
//		ffly_task_pool_add(&pool, test, NULL);
//	}
//	ffly_nanosleep(1, 0);

//	ffly_task_pool_cleanup(&pool);
//	p = malloc(200);
//	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
//	ffly_tid_t id;
//	ffly_thread_create(&id, thr, NULL);
//	ffly_thread_wait(id);
/*
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);	
	while(run<0);
	ffly_printf("bye.\n");
*/
/*
	double val = 1;
	double cos = 0, sin = 0; 
	cos = ffly_cos(val);
	sin = ffly_sin(val);

	ff_u64_t adr;
	__asm__("movq %%rsp, %0\n\t" : "=m"(adr));
	adr = (adr+(16-1))&-16;
	__asm__("movq %0, %%rsp\n\t" : : "m"(adr));
	ffly_printf("cos: %f, sin: %f\n", cos, sin);
*/
/*
	__linux_pid_t pid = fork();
	if (pid == 0) {
		ffly_nanosleep(2, 0);
		exit(0);
	}

	wait4(pid, NULL, __WALL, NULL);
	ffly_printf("report: %s\n", strerror(errno));
*/
/*
	char buf[PATH_MAX];
	getcwd(buf, PATH_MAX);
	ffly_printf("dir: %s\n", buf);
*/
/*
	ffly_schedule(test, NULL, 1000);
	ff_uint_t i = 0;

	while(i++ != 10000) {
		ffly_sched_clock_tick(1);
		ffly_scheduler_tick();
		ffly_nanosleep(0, 1000000);
	}

	ffly_scheduler_de_init();
*/
//	ff_stores_connect();
//	ff_stores_login();
//	char *motd = (char*)ff_stores_get(_ff_stores_motd);
//	ffly_printf("motd: %s\n", motd);
//	__ffly_mem_free(motd);
//	ff_stores_logout();
//	ff_stores_disconnect();
	
	ffly_arstat();
}
