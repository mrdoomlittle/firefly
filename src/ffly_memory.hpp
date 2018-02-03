# ifndef __ffly__memory__hpp
# define __ffly__memory__hpp
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include <mdlint.h>
# include "types/err_t.h"
# include "system/errno.h"
# include "types/size_t.h"
# include <cstdint>
void inline* operator new(std::size_t __size) {
	return mdl::firefly::memory::mem_alloc(__size);
}

void inline operator delete(void *__p) {
	mdl::firefly::memory::mem_free(__p);
}
namespace mdl {
namespace ffly_memory {
# ifdef __DEBUG_ENABLED
uint_t __inline__ get_mem_usage() {
	return ffly_mem_alloc_bc - ffly_mem_free_bc;}
# endif
firefly::types::err_t init();
firefly::types::err_t de_init();
}
}

# endif /*__ffly__memory__hpp*/
