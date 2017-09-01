# ifndef __make__2darr__hpp
# define __make__2darr__hpp
# include <mdlint.h>
# include <cstdio>
# include <boost/cstdint.hpp>
# include "../memory/mem_alloc.h"
# include "../system/errno.h"
namespace mdl {
namespace firefly {
namespace data {
template<typename _T>
_T ** make_2darr(uint_t __rows, uint_t __cols, _T *__data, uint_t __dsize, boost::int8_t &__any_error, bool __hard_copy = false, uint_t __offset = 0) {
	_T **data = (_T **)memory::mem_alloc(__cols * sizeof(_T *));

	if ((__cols * __rows) != (__dsize / (__offset == 0? 1 : __offset))) {
		fprintf(stderr, "can't create 2d array. Both 1d and 2d array's must have the same amount of elements.\n");
		__any_error = FFLY_FAILURE;
		return nullptr;
	}

	if (!__hard_copy) {
		uint_t t = __offset == 0? 1 : __offset;
		for (uint_t o = 0, r = 0; o != __dsize; o += t) {
			data[r] = __data + (o + (r * __rows));
			r ++;
		}

		__any_error = FFLY_SUCCESS;
		return data;
	} else {
		uint_t t = __offset == 0? 1 : __offset, l = 0;
		for (uint_t o = 0; o != __cols; o ++) {
			data[o] = (_T *)memory::mem_alloc(__rows * sizeof(_T));
			for (uint_t r = 0; r != __rows; r ++) {
				data[o][r] = __data[l];
				l += t;
			}
		}

		__any_error = FFLY_SUCCESS;
		return data;
	}

	__any_error = FFLY_FAILURE;
	return nullptr;
}

}
}
}

# endif /*__make__2darr__hpp*/
