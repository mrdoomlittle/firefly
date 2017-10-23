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

int main() {
	ffly_mem_track_init(&__ffly_mem_track__);
/*
	struct ffly_mem_allocr ma;


	ffly_mem_allocr_init(&ma, 1024);

	void *p[80];
	mdl_u16_t a = 2, i = 0, o = 0;
	for (;a != 512; a*=2, i++) {
		printf("alloced %u bytes.\n", a);
		ffly_mem_allocr_alloc(&ma, &p[i], a);
	}


	for (;o != i; o++) {
		ffly_mem_allocr_free(&ma, p[o]);
	}

	void *mem;
	ffly_mem_allocr_alloc(&ma, &mem, 256);

*/
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
