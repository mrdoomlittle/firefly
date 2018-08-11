# include "image.h"
# include "../system/error.h"
# include "../system/log.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../linux/limits.h"
#ifdef __ffly_has_libpng
# include "png.h"
#endif

#ifdef __ffly_has_libjpeg
# include "jpeg.h"
#endif

#ifdef __ffly_has_p
# include "p.h"
#endif
/*
# include "../data/str_put.h"
static char const*
mkpath(char const *__dir, char const *__name, char const *__ext) {
	char buf[PATH_MAX], *p;
	p = buf;
	p+=ffly_str_put(__dir, p);
	*(p++) = '/';
	p+=ffly_str_put(__name, p);
	*(p++) = '.';
	ffly_str_put(__dir, __ext);

	ff_uint_t l;
	char *r;
	r = (char*)__ffly_mem_alloc(l = (p-buf));
	ffly_mem_cpy(r, buf, l);
	*(r+l) = '\0';
	return (char const*)r;
}
*/

void ffly_im_save(ffly_imagep __im, char const *__path, ff_uint_t __format) {
	switch(__format) {
		case _ffly_im_png:
		break;
		case _ffly_im_jpeg:
		break;
		case _ffly_im_p:
#ifdef __ffly_has_p
			ffly_im_p_se(__path, __im);
#else
#endif
		break;
	}
}

ffly_imagep
ffly_im_creat(ff_uint_t __width, ff_uint_t __height) {
	ffly_imagep im;
	im = (ffly_imagep)__ffly_mem_alloc(sizeof(struct ffly_image));

	im->pixels = (ff_byte_t*)__ffly_mem_alloc(__width*__height*4);
	im->width = __width;
	im->height = __height;
	return im;
}

void ffly_im_write(ffly_imagep __im, void *__buf, ff_u32_t __offset, ff_uint_t __sz) {
	ffly_mem_cpy(__im->pixels+__offset, __buf, __sz);
}

void ffly_im_read(ffly_imagep __im, void *__buf, ff_u32_t __offset, ff_uint_t __sz) {
	ffly_mem_cpy(__im->pixels+__offset, __buf, __sz);
}

void ffly_im_destroy(ffly_imagep __im) {
	__ffly_mem_free(__im->pixels);
	__ffly_mem_free(__im);
}

ff_err_t
ffly_im_ld(ffly_imagep __im, char const *__path, ff_uint_t __format) {
	ff_err_t err;
	switch(__format) {
		case _ffly_im_png:
#ifdef __ffly_has_libpng
			ffly_im_png_ld(__path, __im);	
#else
	log("not compiled with libpng.\n");
#endif
		break;
		case _ffly_im_jpeg:
#ifdef __ffly_has_libjpeg
			ffly_im_jpeg_ld(__path, __im);
#else
	log("not compiled with libjpeg.\n");
#endif
		break;
		case _ffly_im_p:
#ifdef __ffly_has_p
			ffly_im_p_ld(__path, __im);
#else
	log("not compiled with p.\n");
#endif
		break;
		default:
			err = FFLY_FAILURE;
	}
	retok;
}
