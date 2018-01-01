# ifndef __ffly__freetype__h
# define __ffly__freetype__h
# include <ft2build.h>
# include <mdlint.h>
# include "types/err_t.h"
struct ffly_ft {
    FT_Face face;
    FT_Library library;
};

ffly_err_t ffly_ft_init(struct ffly_ft*);
ffly_err_t ffly_ft_ld(struct ffly_ft*, char*, char*);
ffly_err_t ffly_ft_set_pixel_size(struct ffly_ft*, mdl_uint_t, mdl_uint_t);
# endif /*__ffly__freetype__h*/
