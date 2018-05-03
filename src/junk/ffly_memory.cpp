# include "ffly_memory.hpp"
mdl::firefly::types::err_t mdl::ffly_memory::init() {
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_memory::de_init() {
# ifdef __WITH_MEM_TRACKER
/*
	for (;;) {
		void *nxt_ptr = ffly_mem_track_get_nxt(&__ffly_mem_track__);
		if (nxt_ptr == NULL) break;
		firefly::memory::mem_free(nxt_ptr);
		printf("freed memory with ptr: %p\n", nxt_ptr);
	}
*/
# endif
	return FFLY_SUCCESS;
}
