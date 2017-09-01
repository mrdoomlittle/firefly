# ifndef __chunk__info__t__hpp
# define __chunk__info__t__hpp
# include <mdlint.h>
# include "coords_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	coords_t<> coords;
	uint_t *id;
} chunk_info_t;
}
}
}

# endif /*__chunk__info__t__hpp*/
