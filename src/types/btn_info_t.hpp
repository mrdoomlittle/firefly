# ifndef __btn__info__t__hpp
# define __btn__info__t__hpp
# include <eint_t.hpp>
# include "pixmap_t.h"
# include "coords_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	coords_t coords;
	uint_t xaxis_len, yaxis_len;
	bool enabled, hover_enabled, mouse_hovering;
	void (* hover_fptr) (uint_t) = nullptr;
	pixmap_t pixmap;
} btn_info_t;
}
}
}

# endif /*__btn__info__t__hpp*/
