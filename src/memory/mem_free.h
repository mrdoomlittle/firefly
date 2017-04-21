# ifndef __mem__free__h
# define __mem__free__h
# include <malloc.h>
# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __DEBUG_ENABLED
extern std::size_t free_amount;
extern std::size_t free_count;
# endif
void mem_free(void *__mem);
}
}
}
# endif
# endif /*__mem__free__h*/
