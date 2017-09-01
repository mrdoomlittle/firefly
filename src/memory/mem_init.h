# ifndef __mem__init__hpp
# define __mem__init__hpp
# include <mdlint.h>
# include <string.h>
namespace mdl {
namespace firefly {
namespace memory {
__inline__ void* mem_init(void * __mptr, uint_t __size) {
	bzero((u8_t *)__mptr, __size * sizeof(u8_t));
	return __mptr;
}
}
}
}

# endif /*__mem__init__hpp*/
