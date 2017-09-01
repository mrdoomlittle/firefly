# ifndef __make__dynamic__hpp
# define __make__dynamic__hpp
# include "mem_alloc.h"
namespace mdl {
namespace firefly {
namespace memory {
template<typename _T, std::size_t _N>
_T * make_dynamic(_T __data[_N]) {
	_T *to_return = (_T *)mem_alloc(sizeof(_N *sizeof(_T)));

	for (std::size_t o = 0; o != _N; o ++)
		to_return[o] = __data[o];

	return to_return;
}

template<typename _T, std::size_t _N, std::size_t _N1>
_T **make_dynamic(_T __data[_N][_N1]) {
	_T **to_return = (_T **)mem_alloc(_N * sizeof(_T *));
	for (std::size_t o = 0; o != _N; o ++)
		to_return[o] = (_T *)mem_alloc(_N1 * sizeof(_T));

	for (std::size_t y = 0; y != _N; y ++)
		for (std::size_t x = 0; x != _N1; x ++)
			to_return[y][x] = __data[y][x];
	return to_return;
}
}
}
}

# endif /*__make__dynamic__hpp*/
