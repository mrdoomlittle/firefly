# ifndef __pixmap__t__h
# define __pixmap__t__h
# include "byte_t.h"
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef byte_t __pixmap_t;
typedef byte_t* pixmap_t;
}
}
}
# else
typedef ffly_byte_t __ffly_pixmap_t;
typedef ffly_byte_t* ffly_pixmap_t;
# endif
# endif /*__pixmap__t__h*/
