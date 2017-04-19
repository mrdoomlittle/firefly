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
types::_1d_pm_t alloc_pixmap(uint_t __xa_len, uint_t __ya_len, uint_t __za_len, u8_t __chn_c = 4);

__inline__ types::_1d_pm_t alloc_pixmap(uint_t __xa_len, u8_t __chn_c = 4) {
	return alloc_pixmap(__xa_len, 1, 1, __chn_c);
}

__inline__ types::_1d_pm_t alloc_pixmap(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	return alloc_pixmap(__xa_len, __ya_len, 1, __chn_c);
}

__inline__ types::_1d_pm_t alloc_1d_pm(uint_t __xa_len, u8_t __chn_c = 4) {
	return alloc_pixmap(__xa_len, __chn_c);
}

__inline__ types::_2d_pm_t alloc_2d_pm(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	types::_2d_pm_t pm = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) pm[ya] = alloc_1d_pm(__xa_len, __chn_c);
	return pm;
}

__inline__ types::_2d_pm_t alloc_2d_cmem_pm(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	types::_1d_pm_t lpm = (types::_1d_pm_t)alloc_pixmap(__xa_len, __ya_len, __chn_c);
	types::_2d_pm_t pm = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) pm[ya] = lpm + ya * __xa_len;
	return pm;
}

__inline__ types::_2d_pm_t make_2d_pm(types::_1d_pm_t __1d_pm, uint_t __xa_len, uint_t __ya_len) {
	types::_2d_pm_t pm = (types::_2d_pm_t)mem_alloc(__ya_len  * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) pm[ya] = &__1d_pm[ya * __xa_len];
	return pm;
}

__inline__ types::_1d_pm_t alloc_pixmap(types::_3d_dsize_t<> __dsize, u8_t __chn_c = 4) {
	return alloc_pixmap(__dsize.xaxis_len, __dsize.yaxis_len, __dsize.zaxis_len, __chn_c);
}

__inline__ types::_1d_pm_t alloc_pixmap(types::_2d_dsize_t<> __dsize, u8_t __chn_c = 4) {
	return alloc_pixmap(__dsize.xaxis_len, __dsize.yaxis_len, __chn_c);
}
}
}
}

# endif /*__alloc__pixmap__hpp*/
