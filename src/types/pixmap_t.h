# ifndef __pixmap__t__h
# define __pixmap__t__h
# include "byte_t.h"
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef byte_t __pixmap_t;
typedef byte_t* pixmap_t;
typedef byte_t* _1d_pm_t;
typedef byte_t** _2d_pm_t;
typedef byte_t*** _3d_pm_t;
}
}
}
# else
typedef ffly_byte_t __ffly_pixmap_t;
typedef ffly_byte_t* ffly_pixmap_t;
typedef ffly_byte_t* ffly_1d_pm_t;
typedef ffly_byte_t** ffly_2d_pm_t;
typedef ffly_byte_t*** ffly_3d_pm_t;
# endif
# endif /*__pixmap__t__h*/
