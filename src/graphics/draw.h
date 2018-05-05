# ifndef __ffly__graphics__draw__h
# define __ffly__graphics__draw__h
# include "../ffint.h"
# include "../types.h"
# ifdef __cplusplus
# ifndef __NVCC__
#	include "pipe.h"
# endif
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t gpu_pixdraw(uint_t __x, uint_t __y, types::byte_t *__dst, uint_t __dstw, types::byte_t *__pixels, uint_t __width, uint_t __height);
# ifndef __NVCC__
types::err_t static pixeldraw(uint_t __x, uint_t __y, types::byte_t *__dst, uint_t __dstw, types::byte_t *__pixels, uint_t __width, uint_t __height) {
	return pipe_inject(&__ffly_grp__, mk_job(_grj_pixdraw, __x, __y, __dst, __dstw, __pixels, __width, __height));
}
# endif
}
}
}
extern "C" {
# endif
//ff_err_t ffly_pixeldraw(ff_uint_t, ff_uint_t, ff_byte_t*, ff_uint_t, ff_byte_t*, ff_uint_t, ff_uint_t);
ff_err_t ffly_pixdraw(ff_uint_t, ff_uint_t, ff_byte_t*, ff_uint_t, ff_byte_t*, ff_uint_t, ff_uint_t);
ff_err_t __ffly_pixdraw(ff_uint_t, ff_uint_t, ff_byte_t*, ff_uint_t, ff_byte_t*, ff_uint_t, ff_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__graphics__draw__h*/
