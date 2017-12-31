# ifndef __ffly__scale__pixelmap__h
# define __ffly__scale__pixelmap__h
# include "../types/pixelmap_t.h"
# include "../types/err_t.h"
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_scale_nearest_neighbor(ffly_pixelmap_t, ffly_pixelmap_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
ffly_err_t ffly_scale_bilinear(ffly_pixelmap_t, ffly_pixelmap_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__scale__pixelmap__h*/
