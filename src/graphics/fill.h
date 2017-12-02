# ifndef __ffly__fill__h
# define __ffly__fill__h
# include <mdlint.h>
# include "../types/byte_t.h"
# include "../types/colour_t.h"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t fill_frame(types::byte_t*, mdl_uint_t, mdl_uint_t, types::colour_t);
}
}
}
# endif /*__ffly__fill__h*/
