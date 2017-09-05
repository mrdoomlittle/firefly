//# include "mem_allocr.h"
//# include "mem_tracker.h"
# include <stdio.h>
# include "mem_allocr.h"
# include "timer.hpp"
# include <cstdlib>
//# include "smem_buff.h"
//# include "queue.hpp"
//# include "vec.h"
//# include "buff.h"
# include "queue.h"
# include <string.h>
# include "io.h"
# include "set.hpp"
# include "mem_tracker.h"
# include "vec.h"
# include <unistd.h>
int main() {
	ffly_mem_track_init(&__ffly_mem_track__);

	mdl::firefly::system::set<mdl_uint_t>* tst = new mdl::firefly::system::set<mdl_uint_t>();
	tst->insert(2);
	tst->insert(1);
	tst->insert(2);
	tst->insert(9);
	tst->insert(9);

	tst->erase(tst->end());
	tst->erase(tst->end());
	tst->erase(tst->end());

	for (mdl::firefly::system::set<mdl_uint_t>::iterator itr = tst->begin(); itr != tst->end()+1;) {
		printf("%u\n", *itr);

		itr++;
	}

	delete tst;

/*
	struct ffly_vec my_vec;

	my_vec.flags = VEC_AUTO_RESIZE | VEC_BLK_CHAIN | VEC_UUU_BLKS;
	ffly_vec_init(&my_vec, sizeof(mdl_uint_t));

	for (int i = 0; i != 11; i++) {
		mdl_uint_t *x;
		ffly_vec_push_back(&my_vec, (void**)&x);
		*x = i;
	}

	void *p = ffly_vec_last(&my_vec);
//	ffly_vec_itr(&my_vec, (void**)&p, VEC_ITR_DOWN, 6);
	ffly_vec_del(&my_vec, p);

//	printf("%u\n", ffly_vec_size(&my_vec));
//	mdl_uint_t *x;
//	ffly_vec_push_back(&my_vec, (void**)&x);
//	*x = 21;
//	ffly_vec_push_back(&my_vec, (void**)&x);
//	*x = 31;

	void *itr = ffly_vec_first(&my_vec);
	while(itr != NULL) {
		printf("value: %u\n", *(mdl_u8_t*)itr);
		ffly_vec_itr(&my_vec, (void**)&itr, VEC_ITR_DOWN, 1);
	}

	ffly_vec_de_init(&my_vec);
*/
/*
	mdl_uint_t *p;
	struct ffly_vec my_vec;
	ffly_vec_init(&my_vec, sizeof(mdl_uint_t));
	ffly_vec_resize(&my_vec, 12);

	for (int i = 0; i != 11; i++) {
	mdl_uint_t *x;
	ffly_vec_push_back(&my_vec, (void**)&x);
	*x = i;

	if (i >= 6 && i <= 12) {
		p = x;
		ffly_vec_del(&my_vec, (void*)x);
	}
	}

	//ffly_vec_del(&my_vec, (void*)p);


	mdl_uint_t *itr = (mdl_uint_t*)ffly_vec_rbegin(&my_vec);
	while(itr != NULL) {
		usleep(1000000);
		printf("%u\n", *itr);

		ffly_vec_itr(&my_vec, (void**)&itr, VEC_ITR_DOWN);
	}
*/
//	ffly_mem_track_de_init(&__ffly_mem_track__);


//	mdl::firefly::system::io::printf(stdout, "hello\n");
/*
	ffly_mem_track_init(&__ffly_mem_track__);

	mdl::firefly::system::queue<mdl::uint_t> my_queue;

	my_queue.push(21);
	my_queue.push(29);
	my_queue.push(9);

	mdl::i8_t o;
	printf("%u", my_queue.pop(o));
	printf("%u", my_queue.pop(o));
	printf("%u\n", my_queue.pop(o));
*/
//	printf("size: %u\n", ffly_queue_size(&q));

//	for (mdl_uint_t ic = 0; ic != size/2; ic++)
//		ffly_queue_push(&q, (void*)&ic);

//	printf("size: %u\n", ffly_queue_size(&q));
/*
	struct ffly_buff b;

	ffly_buff_init(&b, 12, sizeof(mdl_uint_t));


	mdl_uint_t put = 21299;
	ffly_buff_put(&b, &put);

	ffly_buff_incr(&b);

	mdl_uint_t get = 0;
	ffly_buff_get(&b, &get);
	printf("%u\n", get);

	ffly_buff_de_init(&b);
*/
/*
	mdl::firefly::system::vec<mdl::uint_t> _vec;
	_vec.resize(100);

	for (int x = 0; x != 88; x ++)
		_vec.push_back(x);

	for (int x = 0; x != 88; x ++)
		printf("%u\n", _vec.pop_back());
*/
/*
	ffly_smem_buff_t *smem_buff = ffly_smem_buff_ninst(12, sizeof(int));
	int x = 21;
	for (int l = 0; l != 20; l ++) {
		if (ffly_smem_buff_push(smem_buff, &x) != FFLY_SUCCESS) {
			printf("errno: %d\n", ffly_errno);
			break;
		}
	}

	int o = 0;
	ffly_smem_buff_pop(smem_buff, (void *)&o);
	printf("%d\n", o);
*/
/*
	struct ffly_mem_allocr allocr;
	ffly_mem_allocr_init(&allocr, 200);

	void *ptr, *p;
	mdl::firefly::system::timer timer;

	ffly_mem_allocr_alloc(&allocr, &ptr, 4);
	ffly_mem_allocr_free(&allocr, ptr);

	timer.begin();
*/
//op
/*
	ffly_mem_allocr_alloc(&allocr, &p, 5);
	ffly_mem_allocr_alloc(&allocr, &ptr, 5);
	ffly_mem_allocr_free(&allocr, ptr);


	

	ffly_mem_allocr_alloc(&allocr, &ptr, sizeof(mdl_uint_t));
//	ffly_mem_allocr_free(&allocr, ptr);

	timer.end();

	ffly_mem_allocr_de_init(&allocr);

	printf("time taken: %d\n", timer.delta<std::chrono::nanoseconds>());
*/
/*
	printf("mem track: used: %d - freed: %d\n", ffly_mem_alloc_bc, ffly_mem_free_bc);

	struct ffly_mem_allocr allocr;

	allocr.stack_size = 40;
	void *_my_data = NULL;


	ffly_mem_track_init(&__ffly_mem_track__);
	ffly_mem_allocr_init(&allocr);

	void *t;
	ffly_mem_allocr_alloc(&allocr, &_my_data, sizeof(mdl_uint_t));

	mdl::firefly::system::timer timer;

	timer.begin();
	ffly_mem_allocr_alloc(&allocr, &_my_data, sizeof(mdl_uint_t));
	timer.end();

	printf("time taken: %d\n", timer.delta<std::chrono::nanoseconds>());
	mdl_uint_t *my_data = (mdl_uint_t *)_my_data;
	printf("%d\n", *my_data);
	*my_data = 21299;
	printf("%d\n", *my_data);

	ffly_mem_allocr_de_init(&allocr);
	ffly_mem_track_de_init(&__ffly_mem_track__);

	printf("mem track: used: %d - freed: %d\n", ffly_mem_alloc_bc, ffly_mem_free_bc);
*/
}
