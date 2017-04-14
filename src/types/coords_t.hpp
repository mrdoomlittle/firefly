# ifndef __coords__t__hpp
# define __coords__t__hpp
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
template <typename _T = uint_t>
struct coords_t {
# ifndef __NVCC__
	operator coords_t<uint_t>() const {
		coords_t<uint_t> _coords = {
			.xaxis = (uint_t)this-> xaxis,
			.yaxis = (uint_t)this-> yaxis,
			.zaxis = (uint_t)this-> zaxis
		};
		return _coords;
	}

	operator coords_t<boost::uint8_t>() const {
		coords_t<boost::uint8_t> _coords = {
			.xaxis = (boost::uint8_t)this-> xaxis,
			.yaxis = (boost::uint8_t)this-> yaxis,
			.zaxis = (boost::uint8_t)this-> zaxis
		};
		return _coords;
	}
# endif
	_T xaxis, yaxis, zaxis;
};

template <typename _T = uint_t>
struct _2d_coords_t {
	_T xaxis, yaxis;
};

# ifndef __NVCC__
template <typename _T = uint_t>
static _2d_coords_t<_T> __coords__(_T __xaxis, _T __yaxis) {
	_2d_coords_t<_T> _coords = {
		.xaxis = __xaxis,
		.yaxis = __yaxis
	};
	return _coords;
}

# endif

template <typename _T = uint_t>
struct _3d_coords_t {
    _T xaxis, yaxis, zaxis;
};

# ifndef __NVCC__
template <typename _T = uint_t>
static _3d_coords_t<_T> __coords__(_T __xaxis, _T __yaxis, _T __zaxis) {
	_3d_coords_t<_T> _coords = {
		.xaxis = __xaxis,
		.yaxis = __yaxis,
		.zaxis = __zaxis
	};
	return _coords;
}
# endif


// ncvv seems to not like this
# ifndef __NVCC__
template <typename _T = uint_t>
static coords_t<_T> coords(_T __xaxis = 0, _T __yaxis = 0, _T __zaxis = 0) {
	coords_t<_T> _coords = {
		.xaxis = __xaxis,
		.yaxis = __yaxis,
		.zaxis = __zaxis
	};
	return _coords;
}
# endif
}
}
}

# endif /*__coords__t__hpp*/
