# ifndef __ffly__png__loader__h
# define __ffly__png__loader__h
# include <mdlint.h>
# include "../types/err_t.h"
# include <stddef.h>
# include <stdio.h>
# include <jpeglib.h>
# include "image.h"
// might rename to ffly_ld_jpeg_img
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ld_jpeg_file(char*, char*, ffly_imagep);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
static types::err_t(*ld_jpeg_file)(char*, char*, imagep) = &ffly_ld_jpeg_file;
}
}
}
# endif
# endif /*__ffly__png__loader__h*/
