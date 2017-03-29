# ifndef __btn__info__t__hpp
# define __btn__info__t__hpp
# include <eint_t.hpp>
# include "coords_t.hpp"
# include "pixmap_t.h"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	pixmap_t pixmap;
	coords_t<> coords;
	uint_t xaxis_len, yaxis_len;
} btn_info_t;
}
}
}

# endif /*__btn__info__t__hpp*/
