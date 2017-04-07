# ifndef __ffly__errno__h
# define __ffly__errno__h
// basic errno
# ifndef FFLY_SUCCESS
#	define FFLY_SUCCESS 0
# endif

# ifndef FFLY_FAILURE
#	define FFLY_FAILURE -1
# endif

# ifndef FFLY_NOP
#	define FFLY_NOP 1
# endif

# ifdef __cplusplus
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace system {
//static boost::int8_t errno;
}
}
}
# else
# include <stdint.h>
//static int8_t ffly_errno;
# endif
# endif /*__ffly__errno__h*/
