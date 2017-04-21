# ifndef __flip__dir__hpp
# define __flip__dir__hpp
# include <eint_t.hpp>
# include <atomic>
namespace mdl {
namespace firefly {
__inline__ void flip_dir(std::atomic<i8_t>& __dir) {
	switch(__dir) {case -1: __dir += 2; break; case 1: __dir -= 2; break;}
}
}
}

# endif /*__flip__dir__hpp*/
