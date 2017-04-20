# ifndef __flip__dir__hpp
# define __flip__dir__hpp
# include <boost/cstdint.hpp>
# include <atomic>
namespace mdl {
namespace firefly {
__inline__ flip_dir(std::atomic<boost::int8_t>& __dir) {
	switch(__dir) {case -1: __dir += 2 break; case 1: __dir -= 2; break;}
}
}
}

# endif /*__flip__dir__hpp*/
