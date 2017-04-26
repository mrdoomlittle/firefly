//# include "mem_allocr.h"
//# include "mem_tracker.h"
# include <stdio.h>
# include "../ffly_memory.hpp"
# include "../ffly_system.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/conv_dsm.h"
int main() {
	mdl::ffly_system::init();

	int *x = (int *)mdl::firefly::memory::mem_alloc(2048 * sizeof(int));
	int *y = (int *)mdl::firefly::memory::mem_alloc(455 * sizeof(int));

	ffly_mem_track_dmp_ptr_lst(&__ffly_mem_track__);


	printf("memory usage: %dbits\n", mdl::firefly::data::conv_dsm<mdl::uint_t>(mdl::ffly_memory::get_mem_usage(), mdl::firefly::data::BYTE_TO_BIT));

	mdl::firefly::memory::mem_free(x);
	mdl::firefly::memory::mem_free(y);
	mdl::ffly_system::de_init();
/*
	struct ffly_mem_allocr_t allocr;

	allocr.stack_size = 80;
	void *_my_data = NULL;

	ffly_mem_allocr_init(&allocr);
	ffly_mem_allocr_alloc(&allocr, &_my_data, sizeof(mdl_uint_t));

	mdl_uint_t *my_data = (mdl_uint_t *)_my_data;

	printf("%d\n", *my_data);
	*my_data = 21299;
	printf("%d\n", *my_data);

	ffly_mem_allocr_de_init(&allocr);
*/
}
