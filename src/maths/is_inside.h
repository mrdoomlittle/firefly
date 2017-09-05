# ifndef __ffly__is__inside__h
# define __ffly__is__inside__h
# include <mdlint.h>
# include "../types/coords_t.h"
# include "../types/bool_t.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_bool_t ffly_is_inside(ffly_3d_coords_t*, ffly_3d_coords_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace maths {
static types::bool_t(*is_inside)(types::_3d_coords_t*, types::_3d_coords_t*, uint_t, uint_t, uint_t, mdl_u8_t) = &ffly_is_inside;
}
}
}
# endif
# endif /*__ffly__is__inside__h*/
