# ifndef __colour__ptr__t__hpp
# define __colour__ptr__t__hpp
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {

typedef struct {
	boost::uint8_t *r, *g, *b, *a;
} colour_ptrs_t;

}

}
}

# endif /*__colour__ptr__t__hpp*/
