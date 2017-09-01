# ifndef __chunk__data__t__hpp
# define __chunk__data__t__hpp
# include "pixmap_t.h"
# include "uni_par_t.hpp"

namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uni_par_t *particles = nullptr;
	_1d_pm_t _1d_pm = nullptr;
	_3d_pm_t _3d_pm = nullptr;
} chunk_data_t;
}
}
}

# endif /*__chunk__data__t__hpp*/
