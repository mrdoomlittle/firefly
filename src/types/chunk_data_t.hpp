# ifndef __chunk__data__t__hpp
# define __chunk__data__t__hpp
# include "pixmap_t.h"
# include "uni_par_t.hpp"

namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uni_par_t *particles = nullptr;
	pixmap_t pixmap = nullptr;
} chunk_data_t;
}
}
}

# endif /*__chunk__data__t__hpp*/
