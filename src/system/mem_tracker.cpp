# include "mem_tracker.hpp"
mdl::firefly::types::err_t mdl::firefly::system::mem_tracker::alloc(void *__mem_ptr, std::size_t __mem_size) {
	std::size_t mem_size = __mem_size;
# ifdef __DEBUG_ENABLED


# endif

	__mem_ptr = memory::mem_realloc(__mem_ptr, mem_size);

}





mdl::firefly::system::mem_tracker __mem_tracker__;


