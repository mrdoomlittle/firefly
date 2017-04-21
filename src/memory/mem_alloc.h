# ifndef __mem__alloc__h
# define __mem__alloc__h
# include <malloc.h>
# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __DEBUG_ENABLED
extern std::size_t alloc_amount;
extern std::size_t alloc_count;
# endif
void* mem_alloc(std::size_t __bytes);
}
}
}
# endif
# endif /*__mem__alloc__h*/
