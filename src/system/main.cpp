# include "io.h"
# include "task_pool.h"
# include "thread.h"
# include <unistd.h>
# include "task_pool.h"
# include "mutex.h"
# include <pthread.h>
# include "signal.h"
# include "atomic.h"
# include "time.h"
# include "mem_tracker.h"
# include "mem_allocr.h"
# include "mem_blk.h"
mdl::firefly::system::atomic_t x;
ffly_atomic_uint_t f = 0;
void *call(void *__arg_p) {
	usleep(10000);
	x++;
	usleep(200);
//	x = x;
}

void a() {
	usleep(1000);
}
void b(mdl_uint_t __a1) {ffly_printf(stdout, "%u\n", __a1);}
void c(mdl_uint_t __a1, mdl_uint_t __a2) {ffly_printf(stdout, "%u, %u\n", __a1, __a2);}
void d(mdl_uint_t __a1, mdl_uint_t __a2, mdl_uint_t __a3) {ffly_printf(stdout, "%u, %u, %u\n", __a1, __a2, __a3);}
void e(mdl_uint_t __a1, mdl_uint_t __a2, mdl_uint_t __a3, mdl_uint_t __a4) {ffly_printf(stdout, "%u, %u, %u, %u\n", __a1, __a2, __a3, __a4);}
# include "futex.h"
ffly_mutex_t mutex = FFLY_MUTEX_INIT;
void* test(void *__arg) {
	mdl_uint_t i = 0;
	void *p;
	while(i++ < 400) {
		ffly_mutex_lock(&mutex);
		ffly_printf(stdout, "%u\n", i);
		p = malloc(1);
		free(p);
		ffly_mutex_unlock(&mutex);
	}

	ffly_printf(stdout, "??? - %u.\n", i);
}
int main() {
	ffly_mem_track_init(&__ffly_mem_track__);

	ffly_tid_t tid1, tid2;
	ffly_thread_create(&tid1, &test, NULL);
	ffly_thread_create(&tid2, &test, NULL);
	while(ffly_thread_alive(tid1));
	while(ffly_thread_alive(tid2));
//	mdl::firefly::system::thread ta(&a, tid);
//	mdl::firefly::system::thread tc(&a, tid);
//	while(ffly_thread_alive(tid));
//	mdl::firefly::system::thread tb(&b, (mdl_uint_t)1, tid);
//	while(ffly_thread_alive(tid));
//	mdl::firefly::system::thread tc(&c, (mdl_uint_t)1, (mdl_uint_t)2, tid);
//	while(ffly_thread_alive(tid));
//	mdl::firefly::system::thread td(&d, (mdl_uint_t)1, (mdl_uint_t)2, (mdl_uint_t)3,  tid);
//	mdl::firefly::system::thread te(&e, (mdl_uint_t)1, (mdl_uint_t)2, (mdl_uint_t)3, (mdl_uint_t)4, tid);
	ffly_thread_cleanup();
	ffly_printf(stdout, "all good.\n");
	fflush(stdout);
	return 0;
//	while(1);
/*
	struct ffly_mem_allocr ma;


	ffly_mem_allocr_init(&ma, 8099);

	void *p[12];
	mdl_u8_t o = 0;
	for (;o != 4;o++) {
	mdl_u8_t i;

	printf("----------- alloc\n");
	i = 0;
	for (;i != sizeof(p)/sizeof(void*);i++) {
		ffly_mem_allocr_alloc(&ma, &p[i], sizeof(mdl_u64_t));
		printf("%p\n", p[i]);
		*(mdl_u64_t*)p[i] = 21299+i;
	}

	printf("----------- free\n");
	i = 0;
	for (;i != sizeof(p)/sizeof(void*);i++) {
		printf(">>>>>>>val: %lu\n", *(mdl_u64_t*)p[i]);
		ffly_mem_allocr_free(&ma, p[i]);
	}

	printf("----------- alloc\n");
	i = 0;
	for (;i != sizeof(p)/sizeof(void*);i++) {
		ffly_mem_allocr_alloc(&ma, &p[i], 20);
		printf("%p\n", p[i]);
	}
	}
*/
//	ffly_mem_track_de_init(&__ffly_mem_track__);
/*
	struct ffly_mem_blk mem_blk;
	ffly_mem_blk_init(&mem_blk, 26, sizeof(mdl_u8_t));

	void *p1, *p2;

	p1 = ffly_mem_blk_alloc(&mem_blk);
	*(mdl_u8_t*)p1 = 212;
	ffly_mem_blk_free(&mem_blk, p1);
	p2 = ffly_mem_blk_alloc(&mem_blk);
	printf("%u\n", *(mdl_u8_t*)p2);
 
	ffly_mem_blk_de_init(&mem_blk);
	ffly_mem_track_de_init(&__ffly_mem_track__);
*/
//# ifndef __WITH_TASK_POOL
//	struct ffly_task_pool __task_pool__;
//# endif
//	mdl_u8_t *dst = (mdl_u8_t*)malloc(5000000);
//	mdl_u8_t *src = (mdl_u8_t*)malloc(5000000);
//	mdl_uint_t size = 5000000;

//	src[600] = 212;
//	struct ffly_task_pool __task_pool__;
//	ffly_mem_track_init(&__ffly_mem_track__);
//	void *p = __ffly_mem_alloc(100);


//	ffly_mem_track_dmp_ptr_lst(&__ffly_mem_track__);
//	ffly_task_pool_init(&__task_pool__, 12);

//	for (mdl_u8_t i = 0; i != 8; i++)
//		ffly_task_pool_add(&__task_pool__, &call, 0, 1, &f);

//	mdl_u64_t begin = ffly_get_us_tp();
//	for (mdl_uint_t c = 0; c != 8; c++) {
//	ffly_mem_cpy(dst, src, size);
//	}

//	printf("%u, %lu microseconds\n", dst[600], ffly_get_us_tp()-begin);

//	usleep(2000000);
//	printf("hello\n");
//	ffly_task_pool_de_init(&__task_pool__);
//	ffly_tid_t tid;
//	ffly_thread_create(&tid, &call, NULL);
//	usleep(5000000);

//	printf("sending sig.\n");
//	ffly_signal(&sig);
//	printf("send sig.\n");

//	usleep(5000000);
//	ffly_thread_cleanup();

//	ffly_mem_track_de_init(&__ffly_mem_track__);
//	free(dst);
//	free(src);
//	printf("itr count: %lu\n", x.val());
/*
	ffly_tid_t tids[20];
	for (mdl_uint_t i = 0; i != 20; i++) {
		ffly_thread_create(&tids[i], NULL, NULL);
		printf("%u\n", tids[i]);
	}
*/
}
