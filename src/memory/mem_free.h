# ifndef __mem__free__h
# define __mem__free__h
# include <malloc.h>
# ifdef __WITH_MEM_TRACKER
#	include "../system/mem_tracker.h"
# endif
# ifdef __cplusplus
# include <cstddef>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
# ifdef __DEBUG_ENABLED
extern std::size_t free_bc;
extern std::size_t free_c;
# endif
# ifdef __WITH_MEM_TRACKER
void mem_free(void *__mem_ptr, bool __track_bypass = false);
# else
void mem_free(void *__mem_ptr);
# endif /*__WITH_MEM_TRACKER*/
}
}
}
extern "C" {
	extern size_t ffly_mem_free_bc;
	extern size_t ffly_mem_free_c;
}
# else
# include <eint_t.h>
# include <stddef.h>
# ifdef __DEBUG_ENABLED
extern size_t ffly_mem_free_bc;
extern size_t ffly_mem_free_c;
# endif
# ifdef __WITH_MEM_TRACKER
void ffly_mem_free(void *, mdl_u8_t);
# else
void ffly_mem_free(void *);
# endif /*__WITH_MEM_TRACKER*/
# endif

# if defined(__WITH_MEM_TRACKER) && !defined(__cplusplus)
#	ifdef __cplusplus
#		define __ffly_mem_free(__MEM_PTR) mdl::firefly::memory::mem_free(__MEM_PTR, 0) 
#	else
#		define __ffly_mem_free(__MEM_PTR) ffly_mem_free(__MEM_PTR, 0)
#	endif
# else
#	ifdef __cplusplus
#		define __ffly_mem_free(__MEM_PTR) mdl::firefly::memory::mem_free(__MEM_PTR) 
#   else
#		define __ffly_mem_free(__MEM_PTR) ffly_mem_free(__MEM_PTR)
#	endif
# endif

# endif /*__mem__free__h*/
