# ifndef __velocity__t__hpp
# define __velocity__t__hpp
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include "direction_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	direction_t direction;
	uint_t xaxis, yaxis, zaxis;
} velocity_t;

}
}
}

# endif /*__velocity__t__hpp*/
