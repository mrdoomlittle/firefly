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
	if (arch_prctl(ARCH_GET_FS, &p) == -1) {
		ffly_printf("error, %s\n", strerror(errno));	
	}
	ffly_printf("fs: %p\n", p);
}
ff_uint_t tls_test;
# include "linux/mman.h"
# include "system/thread.h"
# include "clock.h"
# include "pallet.h"
void *tls;
void* th(void *__arg) {
}
# include "cache.h"
# include "resource.h"
# include "location.h"
# include "system/config.h"
# include "storage/reservoir.h"
# include "version.h"
# include "env.h"
# include "stdio.h"
# include "linux/termios.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct termios term, old;
	tcgetattr(ffly_in->fd, &term);
	old = term;
	term.c_lflag &= ~(ICANON|ECHO);

	tcsetattr(ffly_in->fd, &term);
	ff_u8_t b;
	read(ffly_in->fd, &b, 1);
	printf("---> %c\n", b);
	tcsetattr(ffly_in->fd, &old);

/*
	ffly_scheduler_init(0);	
	ff_uint_t const c = 20;
	ff_u32_t s[c];

	ff_uint_t i = 0;
	while(i != c) {
		s[i] = ffly_schedule(NULL, NULL, 0);
		i++;
	}

	i = 0;
	while(i != c) {
		if (i != 12) 
			ffly_sched_rm(s[i]);
		i++;
	}

	i = 0;
	while(i != c) {
		s[i] = ffly_schedule(NULL, NULL, 0);
		i++;
	}

	ffly_scheduler_de_init();
*/
/*
	ffly_set_cache_dir("../cache");
	ffly_cache_prepare(20);

	ff_rid_t r;
	ffly_resource_creat(&r);
	ffly_resource_move("test.res");
*/
//	struct ffly_pallet pallet;
//	ffly_pallet_init(&pallet, 20, 20, _ffly_tile_64);
/*
	ff_uint_t i = 0;
	while(i++ != 300) {
		ffly_sched_clock_tick(1);
		ffly_scheduler_tick();
 		ffly_nanosleep(0, 1000000);
		ffly_clock_tick();
		ffly_printf("clock: %u\n", clock);
		ffly_resource_load_bin(r, "test.res");
	}

    ffly_scheduler_de_init();
	ffly_cache_cleanup();
	*/
//	ffly_pallet_de_init(&pallet);
//	ffly_resource_cleanup();
	//__ffly_mem_alloc(10);
/*
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
*/
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
/*
	if (sysconf_db_loaded == -1) {
		ffly_printf("config not loaded.\n");
		goto _fault;
	}

	if (_err(ff_stores_connect(__ffly_sysconf__.db.ip_addr,
								__ffly_sysconf__.db.port,
								__ffly_sysconf__.db.enckey)))
	{
		ff_location_list();
		goto _fault;		
	}
	
	if (_err(ff_stores_login(__ffly_sysconf__.db.user, __ffly_sysconf__.db.passwd))) {
		ff_location_list();
		goto _fault;
	}
	char *motd = (char*)ff_stores_get(_ff_stores_motd);
	ffly_printf("motd: %s\n", motd);
	__ffly_mem_free(motd);
	ff_stores_logout();
	ff_stores_disconnect();
*/
/*
	// might break
	ffly_scheduler_init(SCHED_CORRODE);
	
	struct ffly_reservoir res;
	ffly_reservoir_init(&res, RESV_CORRODE, "test.resv");

	void *r0, *r1, *r2;
	r0 = ffly_reservoir_alloc(&res, 1024);
	r1 = ffly_reservoir_alloc(&res, 1024);
	r2 = ffly_reservoir_alloc(&res, 1024);

	char buf[1024];
	ffly_reservoir_write(&res, r0, buf, 0, 1024);
	ffly_reservoir_write(&res, r1, buf, 0, 1024);
	ffly_reservoir_write(&res, r2, buf, 0, 1024);

	ffly_nanosleep(2, 0);

//	ffly_reservoir_de_init(&res);
//	ffly_scheduler_de_init();

	ffly_nanosleep(2, 0);
*/
}
