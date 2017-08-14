//# include "mem_allocr.h"
//# include "mem_tracker.h"
# include <stdio.h>
# include "mem_allocr.h"
# include "timer.hpp"
# include <cstdlib>
# include "smem_buff.h"
# include "queue.hpp"
# include "vec.h"
int main() {
	mdl::firefly::system::vec<mdl::uint_t> _vec;
	_vec.resize(100);

	for (int x = 0; x != 88; x ++)
		_vec.push_back(x);

	for (int x = 0; x != 88; x ++)
		printf("%u\n", _vec.pop_back());
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
