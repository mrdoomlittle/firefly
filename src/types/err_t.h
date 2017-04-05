# ifndef __err__t__hpp
# define __err__t__hpp

# ifdef __cplusplus
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef boost::int8_t err_t;
}
}
}
# else
# include <stdint.h>
typedef int8_t ffly_err_t;
# endif

# endif /*__err__t__hpp*/
