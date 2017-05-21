//# include "mem_allocr.h"
//# include "mem_tracker.h"
# include <stdio.h>
# include "mem_allocr.h"
# include "timer.hpp"
# include <cstdlib>
# include "smem_buff.h"
int main() {
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

	struct ffly_mem_allocr_t allocr;

	allocr.stack_size = 180;
	void *_my_data = NULL;

	ffly_mem_allocr_init(&allocr);

	void *t;
	ffly_mem_allocr_alloc(&allocr, &_my_data, sizeof(mdl_uint_t));
//	ffly_mem_allocr_free(&allocr, _my_data);

	    mdl::firefly::system::timer timer;
	timer.begin();
	ffly_mem_allocr_alloc(&allocr, &_my_data, sizeof(mdl_uint_t));
	timer.end();

	printf("time taken: %d\n", timer.delta<std::chrono::nanoseconds>());
	printf("memory used: %d, memory free: %d\n", ffly_mem_allocr_mem_used(&allocr), ffly_mem_allocr_mem_free(&allocr));
	mdl_uint_t *my_data = (mdl_uint_t *)_my_data;
	printf("%d\n", *my_data);
	*my_data = 21299;
	printf("%d\n", *my_data);

	ffly_mem_allocr_de_init(&allocr);

	printf("mem track: used: %d - freed: %d\n", ffly_mem_alloc_bc, ffly_mem_free_bc);
}
