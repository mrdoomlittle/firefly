# ifndef __alloc__pixmap__hpp
# define __alloc__pixmap__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include "mem_alloc.h"
# include "../types/pixmap_t.h"
# include "../types/dsize_t.hpp"
namespace mdl {
namespace firefly {
namespace memory {
types::pixmap_t alloc_pixmap(uint_t __xaxis_len, uint_t __yaixs_len, uint_t __zaxis_len, uint_t __block_len = 4);

__inline__ types::pixmap_t alloc_pixmap(uint_t __xaxis_len, uint_t __yaxis_len) {
	return alloc_pixmap(__xaxis_len, __yaxis_len, 1);
}

__inline__ types::pixmap_t alloc_pixmap(types::_3d_dsize_t<> __dsize) {
	return alloc_pixmap(__dsize.xaxis_len, __dsize.yaxis_len, __dsize.zaxis_len);
}

__inline__ types::pixmap_t alloc_pixmap(types::_2d_dsize_t<> __dsize) {
	return alloc_pixmap(__dsize.xaxis_len, __dsize.yaxis_len);
}
}
}
}

# endif /*__alloc__pixmap__hpp*/
