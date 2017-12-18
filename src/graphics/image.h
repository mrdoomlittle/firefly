# ifndef __ffly__image__h
# define __ffly__image__h
# include <mdlint.h>
# include "../types/byte_t.h"
struct ffly_image {
	ffly_byte_t *pixels;
	mdl_u32_t width, height;
	ffly_byte_t bit_depth, chn_c;
};

typedef struct ffly_image* ffly_imagep;
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct ffly_image image;
typedef ffly_imagep imagep;
}
}
}
# endif
# endif /*__ffly__image__h*/
