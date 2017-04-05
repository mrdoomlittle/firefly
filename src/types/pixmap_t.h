# ifndef __pixmap__t__h
# define __pixmap__t__h
# ifdef __cplusplus
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef boost::uint8_t __pixmap_t;
typedef boost::uint8_t* pixmap_t;
}
}
}
# else
# include <stdint.h>
typedef uint8_t __ffly_pixmap_t;
typedef uint8_t* ffly_pixmap_t;
# endif
# endif /*__pixmap__t__h*/
