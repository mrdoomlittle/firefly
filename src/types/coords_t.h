# ifndef __coords__t__h
# define __coords__t__h
# include <mdlint.h>
typedef struct {
	mdl_uint_t xa, ya, za;
} ffly_3d_uint_coords_t;

typedef struct {
	mdl_uint_t xa, ya;
} ffly_2d_uint_coords_t;

typedef struct {
	mdl_u32_t xa, ya;
} ffly_2d_u32_coords_t;

typedef struct {
	mdl_int_t xa, ya, za;
} ffly_3d_int_coords_t;

typedef struct {
	mdl_int_t xa, ya;
} ffly_2d_int_coords_t;

typedef struct {
	mdl_i32_t xa, ya;
} ffly_2d_i32_coords_t;

typedef ffly_3d_uint_coords_t ffly_3d_coords_t;
typedef ffly_2d_uint_coords_t ffly_2d_coords_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_3d_coords_t _3d_coords_t;
typedef ffly_2d_coords_t _2d_coords_t;
template<typename _T = uint_t>
struct coords_t {

# ifndef __NVCC__
operator coords_t<uint_t>() const {
	return (coords_t<uint_t>) {
		xaxis:(uint_t)this->xaxis,
		yaxis:(uint_t)this->yaxis,
		zaxis:(uint_t)this->zaxis
	};
}

operator coords_t<u8_t>() const {
	return coords_t<u8_t>{
		xaxis:(u8_t)this->xaxis,
		yaxis:(u8_t)this->yaxis,
		zaxis:(u8_t)this->zaxis
	};
}


# endif
	_T xaxis, yaxis, zaxis;
};

// ncvv seems to not like this
# ifndef __NVCC__
template<typename _T = uint_t>
static coords_t<_T> make_coords(_T __xa, _T __ya, _T __za) {
	return (coords_t<_T>) {
		xaxis:__xa,
		yaxis:__ya,
		zaxis:__za
	};
}

template<typename _T = uint_t>
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
# endif
# endif /*__coords__t__h*/
