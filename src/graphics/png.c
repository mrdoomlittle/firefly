# include "png.h"
# include "../system/file.h"
# include "../system/errno.h"
# include "../system/err.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/str_cmb.h"
# include "../types/byte_t.h"
# include <unistd.h>
# include <png.h>
ffly_err_t ffly_ld_png_img(char *__dir, char *__name, ffly_imagep __image) {
	char *path = ffly_str_cmb(__dir, ffly_str_cmb("/", ffly_str_cmb(__name, ".png", 0), _ffly_stc_free_rhs), _ffly_stc_free_rhs);
	ffly_fprintf(ffly_log, "going to load png file %s\n", path);
	ffly_err_t err;
	FF_FILE *file = ffly_fopen(path, FF_O_RDONLY, 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to open file.\n");
		return err;
	}

	png_structp png_p;
	if (!(png_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL))) {
		ffly_fprintf(ffly_err, "failed to create png struct.\n");
		return FFLY_FAILURE;
	}

	png_infop info_p;
	if (!(info_p = png_create_info_struct(png_p))) {
		ffly_fprintf(ffly_err, "failed to create png info struct.\n");
		return FFLY_FAILURE;
	}

	if (setjmp(png_jmpbuf(png_p)))
		return FFLY_FAILURE;

	int fd = dup(file->fd);
	FILE *f = fdopen(fd, "rb");

	png_init_io(png_p, f);
	png_read_info(png_p, info_p);

	__image->width = png_get_image_width(png_p, info_p);
	__image->height = png_get_image_height(png_p, info_p);
	__image->chn_c = png_get_channels(png_p, info_p);
	__image->bit_depth = png_get_bit_depth(png_p, info_p);
	__image->pixels = (ffly_byte_t*)__ffly_mem_alloc(__image->width*__image->height*__image->chn_c);

	mdl_u32_t row = 0;
	for(;row != __image->height;row++)
		png_read_row(png_p, __image->pixels+(row*(__image->width*__image->chn_c)), NULL);

	fclose(f);
	ffly_fclose(file);
	__ffly_mem_free(path);
	png_destroy_info_struct(png_p, &info_p);
	png_destroy_read_struct(&png_p, (png_infopp)NULL, (png_infopp)NULL);
	ffly_fprintf(ffly_log, "loaded png image, width: %u, height: %u, chn_c: %u\n", __image->width, __image->height, __image->chn_c);
	return FFLY_SUCCESS;
}
