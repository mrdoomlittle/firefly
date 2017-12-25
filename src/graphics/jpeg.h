# ifndef __ffly__png__h
# define __ffly__png__h
# include <mdlint.h>
# include "../types/err_t.h"
# include <stddef.h>
# include <stdio.h>
# include <jpeglib.h>
# include "image.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ld_jpeg_img(char*, char*, ffly_imagep);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
static types::err_t(*ld_jpeg_img)(char*, char*, imagep) = &ffly_ld_jpeg_img;
}
}
}
# endif
# endif /*__ffly__png__h*/
