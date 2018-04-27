# ifndef __ffly__pixelmap__t__h
# define __ffly__pixelmap__t__h
# include "../types.h"
typedef ff_byte_t __ffly_pixelmap_t;
typedef ff_byte_t* ffly_pixelmap_t;
typedef ff_byte_t* ffly_1d_pm_t;
typedef ff_byte_t** ffly_2d_pm_t;
typedef ff_byte_t*** ffly_3d_pm_t;
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef __ffly_pixelmap_t __pixelmap_t;
typedef ffly_pixelmap_t pixelmap_t;
typedef ffly_1d_pm_t _1d_pm_t;
typedef ffly_2d_pm_t _2d_pm_t;
typedef ffly_3d_pm_t _3d_pm_t;
}
}
}
# endif
# endif /*__ffly__pixelmap__t__h*/
