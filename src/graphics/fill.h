# ifndef __ffly__fill__h
# define __ffly__fill__h
# include <mdlint.h>
# include "../types/pixelmap_t.h"
# include "../types/byte_t.h"
# include "../types/colour_t.h"
# include "../types/err_t.h"
# ifdef __cplusplus
# ifndef __NVCC__
#	include "pipe.h"
# endif
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t gpu_pixfill(types::byte_t *__buff, mdl_uint_t __nopix, types::colour_t __colour);
# ifndef __NVCC__
types::err_t static pixelfill(types::pixelmap_t __buff, mdl_uint_t __nopix, types::colour_t __colour) {
	return pipe_inject(&__ffly_grp__, mk_job(_grj_pixfill, __buff, __nopix, __colour));
}
# endif
}
}
}
extern "C" {
# endif
ffly_err_t ffly_pixfill(ffly_byte_t*, mdl_uint_t, ffly_colour_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__fill__h*/
