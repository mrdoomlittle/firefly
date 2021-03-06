# ifndef __flip__dir__hpp
# define __flip__dir__hpp
# include <mdlint.h>
# include <atomic>
namespace mdl {
namespace firefly {
__inline__ void flip_dir(std::atomic<i8_t>& __dir) {
	__dir = ~__dir ^ 1;
}
}
}

# endif /*__flip__dir__hpp*/
