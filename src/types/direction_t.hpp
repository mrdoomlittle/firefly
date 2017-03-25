# ifndef __direction__t__hpp
# define __direction__t__hpp
# include <boost/cstdint.hpp>
# include <atomic>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	std::atomic<boost::int8_t> xaxis, yaxis, zaxis;
} direction_t;

}
}
}

# endif /*__direction__t__hpp*/
