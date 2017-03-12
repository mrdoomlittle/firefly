# ifndef __shape__info__t__hpp
# define __shape__info__t__hpp
# include <eint_t.hpp>
# include "coords_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t edges, faces;
	coords_t **edge_coords, **face_coords;
} shape_info_t;

}
}
}

# endif /*__shape__info__t__hpp*/
