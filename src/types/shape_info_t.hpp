# ifndef __shape__info__t__hpp
# define __shape__info__t__hpp
# include <mdlint.h>
# include "coords_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t edges, faces, vertices;
	coords_t<> (*edge_coords)[2], (*face_coords)[2], (*vertice_coords)[2];
} shape_info_t;

}
}
}

# endif /*__shape__info__t__hpp*/
