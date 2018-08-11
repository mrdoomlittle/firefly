# ifndef __ffly__image__h
# define __ffly__image__h
# include "../ffint.h"
# include "../types.h"

# define FF_IM struct ffly_image
typedef struct ffly_image {
	ff_byte_t *pixels;
	ff_u32_t width, height;
	ff_byte_t bit_depth, chn_c;
} *ffly_imagep;

typedef struct ffly_image** ffly_imagepp;

enum {
	_ffly_im_png,
	_ffly_im_jpeg,
	_ffly_im_p
};

void ffly_im_save(ffly_imagep, char const*, ff_uint_t);
void ffly_im_write(ffly_imagep, void*, ff_u32_t, ff_uint_t);
void ffly_im_read(ffly_imagep, void*, ff_u32_t, ff_uint_t);
ff_err_t ffly_im_ld(ffly_imagep, char const*, ff_uint_t);
ffly_imagep ffly_im_creat(ff_uint_t, ff_uint_t);
void ffly_im_destroy(ffly_imagep);
# endif /*__ffly__image__h*/
