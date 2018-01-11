# ifndef __ffly__graphics__draw__h
# define __ffly__graphics__draw__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/byte_t.h"
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
ffly_err_t ffly_pixeldraw(mdl_uint_t, mdl_uint_t, ffly_byte_t*, mdl_uint_t, ffly_byte_t*, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_pixdraw(mdl_uint_t, mdl_uint_t, ffly_byte_t*, mdl_uint_t, ffly_byte_t*, mdl_uint_t, mdl_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__graphics__draw__h*/
