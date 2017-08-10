# ifndef __ffly__memory__hpp
# define __ffly__memory__hpp
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include <cstddef>
# include <eint_t.hpp>
# include "types/err_t.h"
# include <cstdio>
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
