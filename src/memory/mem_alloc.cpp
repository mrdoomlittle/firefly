# include "mem_alloc.h"
# ifdef DEBUG_ENABLED
	std::size_t mdl::firefly::memory::alloc_amount;
	std::size_t mdl::firefly::memory::alloc_count;
# endif

void* mdl::firefly::memory::mem_alloc(std::size_t __bytes) {
# ifdef DEBUG_ENABLED
	void* mem = malloc(__bytes + sizeof(uint_t));
	*((uint_t *)mem) = __bytes;
	alloc_amount += __bytes;
	alloc_count ++;

	return mem + sizeof(uint_t);
# else
	return malloc(__bytes);
# endif
}
