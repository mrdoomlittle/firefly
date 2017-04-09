# ifndef __byte__t__h
# define __byte__t__h

# ifdef __cplusplus
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef boost::uint8_t byte_t;
}
}
}
# else
# include <stdint.h>
typedef uint8_t ffly_byte_t;
# endif

# endif /*__byte__t__h*/
