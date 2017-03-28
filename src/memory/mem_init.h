# ifndef __mem__init__hpp
# define __mem__init__hpp
# include <eint_t.hpp>
# include <string.h>
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace memory {
__inline__ void* mem_init(void * __mem, uint_t __size) {
	bzero((boost::uint8_t *)__mem, __size * sizeof(boost::uint8_t));
	return __mem;
}
}
}
}

# endif /*__mem__init__hpp*/
