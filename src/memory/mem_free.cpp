# include "mem_free.h"
# ifdef __DEBUG_ENABLED
std::size_t mdl::firefly::memory::free_amount;
std::size_t mdl::firefly::memory::free_count;
# endif

void mdl::firefly::memory::mem_free(void *__mem) {
# ifdef __DEBUG_ENABLED
	free_count ++;
	free_amount += *(uint_t *)(__mem - sizeof(uint_t));
	__mem -= sizeof(uint_t);
# endif
	std::free(__mem);
}
