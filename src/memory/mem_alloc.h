# ifndef __mem__alloc__h
# define __mem__alloc__h
# ifdef __cplusplus
	# include <cstdlib>
	# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef DEBUG_ENABLED
	extern std::size_t alloc_amount;
	extern std::size_t alloc_count;
# endif
void* mem_alloc(std::size_t __bytes);
}
}
}
# endif
# endif /*__mem__alloc__h*/
