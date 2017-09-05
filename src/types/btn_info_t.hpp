# ifndef __btn__info__t__hpp
# define __btn__info__t__hpp
# include <mdlint.h>
# include "coords_t.h"
# include "pixmap_t.h"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	pixmap_t pixmap;
	_2d_coords_t coords;
	uint_t xaxis_len, yaxis_len;
} btn_info_t;
}
}
}

# endif /*__btn__info__t__hpp*/
