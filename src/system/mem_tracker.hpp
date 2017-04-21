# ifndef __mem__tracker__hpp
# define __mem__tracker__hpp
# include "../types/err_t.h"
# include "vector.hpp"
# include "../memory/mem_free.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
namespace mdl {
namespace firefly {
namespace system {
class mem_tracker {
	types::err_t alloc(void *__mem_ptr, std::size_t __mem_size);
	types::err_t free(void *__mem_ptr, std::size_t __mem_size);

	typedef struct {

	} mem_info_t;

	private:
	vector<void *> mem_ptrs;
	vector<mem_info_t> mem_info;
};

extern mem_tracker __mem_tracker__;
}
}
}

# endif /*__mem__tracker__hpp*/
