# ifndef __ffly__alloc__pixelmap__h
# define __ffly__alloc__pixelmap__h
# include "../ffint.h"
# include "../types/pixelmap_t.h"

# ifdef __cplusplus
extern "C" {
# endif
ffly_1d_pm_t ffly_alloc_pixelmap(ff_uint_t, ff_uint_t, ff_uint_t, ff_u8_t);
# ifdef __cplusplus
}
# include "../types/dsize_t.hpp"
# include "mem_alloc.h"
namespace mdl {
namespace firefly {
namespace memory {
types::_1d_pm_t static __inline__ alloc_pixelmap(uint_t __xa_len, uint_t __ya_len, uint_t __za_len, u8_t __chn_c = 4) {
	return ffly_alloc_pixelmap(__xa_len, __ya_len, __za_len, __chn_c);
}

// ignore this
__inline__ static types::_1d_pm_t alloc_pixelmap(uint_t __xa_len, u8_t __chn_c = 4) {
	return alloc_pixelmap(__xa_len, 1, 1, __chn_c);
}

__inline__ static types::_1d_pm_t alloc_pixelmap(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	return alloc_pixelmap(__xa_len, __ya_len, 1, __chn_c);
}

__inline__ static types::_1d_pm_t alloc_1d_pm(uint_t __xa_len, u8_t __chn_c = 4) {
	return alloc_pixelmap(__xa_len, __chn_c);
}

__inline__ static types::_2d_pm_t alloc_2d_pm(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	types::_2d_pm_t pm = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) pm[ya] = alloc_1d_pm(__xa_len, __chn_c);
	return pm;
}

__inline__ static types::_2d_pm_t alloc_2d_cmem_pm(uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	types::_1d_pm_t lpm = (types::_1d_pm_t)alloc_pixelmap(__xa_len, __ya_len, __chn_c);
	types::_2d_pm_t pm = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) pm[ya] = lpm + (ya * __xa_len * __chn_c);
	return pm;
}

__inline__ static types::_2d_pm_t make_2d_pm(types::_1d_pm_t __1d_pm, uint_t __xa_len, uint_t __ya_len, u8_t __chn_c = 4) {
	types::_2d_pm_t _2d_pm = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
	for (uint_t ya{}; ya != __ya_len; ya ++) _2d_pm[ya] = __1d_pm + (ya * __xa_len * __chn_c);
	return _2d_pm;
}

__inline__ static types::_3d_pm_t make_3d_pm(types::_1d_pm_t __1d_pm, uint_t __xa_len, uint_t __ya_len, uint_t __za_len, u8_t __chn_c = 4) {
	types::_3d_pm_t _3d_pm = (types::_3d_pm_t)mem_alloc(__za_len * sizeof(types::_2d_pm_t));
	for (uint_t za{}; za != __za_len; za ++) {
		_3d_pm[za] = (types::_2d_pm_t)mem_alloc(__ya_len * sizeof(types::_1d_pm_t));
		for (uint_t ya{}; ya != __ya_len; ya ++) _3d_pm[za][ya] = __1d_pm + (za * ((__xa_len * __ya_len) * __chn_c)) + (ya * __xa_len * __chn_c);
	}
	return _3d_pm;
}

__inline__ static types::_1d_pm_t alloc_pixelmap(types::_3d_dsize_t<> __dsize, u8_t __chn_c = 4) {
	return alloc_pixelmap(__dsize.xaxis_len, __dsize.yaxis_len, __dsize.zaxis_len, __chn_c);
}

__inline__ static types::_1d_pm_t alloc_pixelmap(types::_2d_dsize_t<> __dsize, u8_t __chn_c = 4) {
	return alloc_pixelmap(__dsize.xaxis_len, __dsize.yaxis_len, __chn_c);
}
}
}
}
# endif
# endif /*__ffly__alloc__pixelmap__h*/
