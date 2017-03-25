# ifndef __alloc__pixmap__hpp
# define __alloc__pixmap__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include "mem_alloc.h"
# include "../types/pixmap_t.h"
namespace mdl {
namespace firefly {
namespace memory {
types::pixmap_t alloc_pixmap(uint_t __xlen, uint_t __ylen, uint_t __zlen, uint_t __block_len = 4);
}
}
}

# endif /*__alloc__pixmap__hpp*/
