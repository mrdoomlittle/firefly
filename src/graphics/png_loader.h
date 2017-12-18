# ifndef __ffly__png__loader__h
# define __ffly__png__loader__h
# include <mdlint.h>
# include "../types/err_t.h"
# include <png.h>
# include "image.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ld_png_file(char*, char*, ffly_imagep);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
static types::err_t(*ld_png_file)(char*, char*, imagep) = &ffly_ld_png_file;
}
}
}
# endif
# endif /*__ffly__png__loader__h*/
