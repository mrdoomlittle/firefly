# ifndef __mem__free__h
# define __mem__free__h
# ifdef __cplusplus
	# include <cstdlib>
	# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef DEBUG_ENABLED
	extern std::size_t free_amount;
	extern std::size_t free_count;
# endif
void mem_free(void *__mem);
}
}
}
# endif
# endif /*__mem__free__h*/
