# ifndef __ffly__fill__h
# define __ffly__fill__h
# include "../ffint.h"
# include "colour.h"
# include "../pallet.h"
# ifdef __cplusplus
# ifndef __NVCC__
#	include "pipe.h"
# endif
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t gpu_pixfill(types::byte_t *__dst, mdl_uint_t __nopix, types::colour_t __colour);
# ifndef __NVCC__
types::err_t static pixelfill(types::pixelmap_t __dst, mdl_uint_t __nopix, types::colour_t __colour) {
	return pipe_inject(&__ffly_grp__, mk_job(_grj_pixfill, __dst, __nopix, __colour));
}
# endif
}
}
}
extern "C" {
# endif
ff_err_t ffly_pixfill(ff_u32_t, ffly_colour_t, ff_u32_t);
ff_err_t __ffly_pixfill(ff_u32_t, ffly_colour_t, ff_u32_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__fill__h*/
