# ifndef __ffly__image__h
# define __ffly__image__h
# include "../ffint.h"
# include "../types.h"
struct ffly_image {
	ff_byte_t *pixels;
	ff_u32_t width, height;
	ff_byte_t bit_depth, chn_c;
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
ff_err_t ffly_ld_img(ffly_imagep, char*, char*, ff_uint_t);
ff_err_t ffly_free_img(ffly_imagep);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct ffly_image image;
typedef ffly_imagep imagep;
typedef ffly_imagepp imagepp;
static ff_err_t(*ld_img)(imagep, char*, char*, uint_t) = &ffly_ld_img;
}
}
}
# endif
# endif /*__ffly__image__h*/
