# ifndef __direction__t__hpp
# define __direction__t__hpp
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	boost::int8_t xaxis, yaxis, zaxis;
} direction_t;

}
}
}

# endif /*__direction__t__hpp*/
