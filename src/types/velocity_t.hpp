# ifndef __velocity__t__hpp
# define __velocity__t__hpp
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include "direction_t.hpp"
# include <atomic>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	direction_t direction;
	std::atomic<uint_t> xaxis, yaxis, zaxis;
} velocity_t;

}
}
}

# endif /*__velocity__t__hpp*/
