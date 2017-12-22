# ifndef __ffly__image__h
# define __ffly__image__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/byte_t.h"
struct ffly_image {
	ffly_byte_t *pixels;
	mdl_u32_t width, height;
	ffly_byte_t bit_depth, chn_c;
};
typedef struct ffly_image* ffly_imagep;
typedef struct ffly_image** ffly_imagepp;
enum {
	_ffly_img_png,
	_ffly_img_jpeg
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ld_img(ffly_imagep, char*, char*, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct ffly_image image;
typedef ffly_imagep imagep;
typedef ffly_imagepp imagepp;
static ffly_err_t(*ld_img)(imagep, char*, char*, uint_t) = &ffly_ld_img;
}
}
}
# endif
# endif /*__ffly__image__h*/
