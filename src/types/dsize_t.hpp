# ifndef __dsize__t__hpp
# define __dsize__t__hpp
# include <eint_t.hpp>

namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t xaxis_len, yaxis_len, zaxis_len;
} dsize_t;

template<typename _T = uint_t>
struct _3d_dsize_t {
	_T xaxis_len, yaxis_len, zaxis_len;
};

template<typename _T = uint_t>
struct _2d_dsize_t {
	_T xaxis_len, yaxis_len;
};

static dsize_t __dsize__(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len) {
	dsize_t dsize = {
		.xaxis_len = __xaxis_len,
		.yaxis_len = __yaxis_len,
		.zaxis_len = __zaxis_len
	};
	return dsize;
}

}
}
}

# endif /*__dsize__t__hpp*/
