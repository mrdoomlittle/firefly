# include "ffint.h"
# include "types.h"
# include "system/vec.h"
# include "system/buff.h"
# include "ffly_memory.hpp"
void *__gxx_personality_v0 = NULL;
void *_Unwind_Resume = NULL;
using namespace ff;
int val;
struct test {
	int& get() {
		return val;
	}

};
# include "dep/mem_cpy.h"
void _main() {
	
}

extern "C" {
ff_err_t ffmain(int __argc, char const *__argv[]) {
	_main();
/*
	ffly_mem_track_init(&__ffly_mem_track__);
	struct ffly_ck chunk_keeper;
	ffly_ck_init(&chunk_keeper, 16, 16, 16);

	mdl_uint_t const s = 20;
	ffly_id_t ids[s];

	for (mdl_uint_t ic = 0; ic != s; ic++) {
		ffly_ck_create(&chunk_keeper, &ids[ic], 0);
		printf("chunk_id: %u\n", *ids[ic]);
	}

	for (mdl_uint_t ic = 0; ic != 2/s; ic++) {
		ffly_ck_del(&chunk_keeper, ids[ic], 1);
	}

	printf("\n\n\n");

	for (mdl_uint_t ic = 0; ic != s/2; ic++) {
		ffly_ck_create(&chunk_keeper, &ids[ic], 1);
		printf("chunk_id: %u\n", *ids[ic]);
	}

	ffly_ck_de_init(&chunk_keeper);

	mdl::firefly::chunk_keeper ck(16, 16, 16);
	mdl::uint_t const s = 20;
	mdl::firefly::types::id_t ids[s];
	for (mdl_uint_t ic = 0; ic != s; ic++) {
		ck.create(ids[ic], 0);
		printf("chunk_id: %u\n", *ids[ic]);
	}

	for (mdl_uint_t ic = 0; ic != 2/s; ic++) {
		ck.del(ids[ic], 0);
	}

	for (mdl_uint_t ic = 0; ic != s/2; ic++) {
		ck.create(ids[ic], 0);
		printf("chunk_id: %u\n", *ids[ic]);
	}

	ck.de_init();
	ffly_mem_track_de_init(&__ffly_mem_track__);

	printf("mem used: %u, mem freed: %u\n", ffly_mem_alloc_bc, ffly_mem_free_bc);

	mdl::firefly::uni_manager uni_manager(12,12,12);
	uni_manager.init(2,2,2);
	mdl::firefly::types::coords_t<> coords = {8, 8, 0};

	uni_manager.chunk_pixmap(coords);
*/
/*
	mdl::firefly::chunk_manager chunk_manager(12, 12, 12);

	mdl::uint_t *chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 0, 0, 0);
	printf("chunk_id: %d\n", *chunk_id);
	chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 12, 0, 0);
    printf("chunk_id: %d\n", *chunk_id);


	printf("%d\n", *chunk_manager.coords_to_id(11,0,0));
*/
}
}
