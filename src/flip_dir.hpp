# ifndef __flip__dir__hpp
# define __flip__dir__hpp
# include <boost/cstdint.hpp>
# include <atomic>
namespace mdl {
namespace firefly {
void flip_dir(std::atomic<boost::int8_t>& __dir);
}
}

# endif /*__flip__dir__hpp*/
