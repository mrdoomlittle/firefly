# ifndef __ffly__copy__h
# define __ffly__copy__h
# include <mdlint.h>
# include "../types/byte_t.h"
# include "../types/err_t.h"
# include "../types/pixelmap_t.h"
# include "../types/off_t.h"
# ifdef __cplusplus
# ifndef __NVCC__
#	include "pipe.h"
# endif
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t gpu_pixcopy(types::byte_t *__dst, types::byte_t *__src, mdl_uint_t __nopix);
# ifndef __NVCC__
types::err_t static pixelcopy(types::pixelmap_t __dst, types::pixelmap_t __src, mdl_uint_t __nopix) {
	return pipe_inject(&__ffly_grp__, mk_job(_grj_pixcopy, __dst, __src, __nopix));
}
# endif
}
}
}
extern "C" {
# endif
ffly_err_t ffly_pixcopy(ffly_byte_t*, ffly_byte_t*, mdl_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__copy__h*/
