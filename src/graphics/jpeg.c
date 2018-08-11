# include "jpeg.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
# include "../system/file.h"
# include "../system/errno.h"
# include <jpeglib.h>
ff_err_t ffly_im_jpeg_ld(char const*__path, ffly_imagep __image) {
	ff_err_t err;
	FF_FILE *f = ffly_fopen(__path, FF_O_RDONLY, 0, &err);
	struct ffly_stat st;
	ffly_fstat(__path, &st);

	ff_byte_t *p = (ff_byte_t*)__ffly_mem_alloc(st.size);
	ffly_fread(f, p, st.size);

	struct jpeg_decompress_struct dec;
	struct jpeg_error_mgr jerr;

	dec.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dec);

	jpeg_mem_src(&dec, p, st.size);
	if (jpeg_read_header(&dec, TRUE) != JPEG_HEADER_OK) {
		ffly_fprintf(ffly_log, "failed to read jpeg header.\n");
		return FFLY_FAILURE;
	}

	jpeg_start_decompress(&dec);

	__image->width = dec.output_width;
	__image->height = dec.output_height;
	__image->chn_c = dec.output_components;

	__image->pixels = (ff_byte_t*)__ffly_mem_alloc(__image->width*__image->height*__image->chn_c);
	while (dec.output_scanline < dec.output_height) {
		ff_byte_t *row = __image->pixels+(dec.output_scanline*(__image->width*__image->chn_c));
		jpeg_read_scanlines(&dec, &row, 1);
	}

	jpeg_finish_decompress(&dec);
	jpeg_destroy_decompress(&dec);

	ffly_fclose(f);
	__ffly_mem_free(p);
	ffly_fprintf(ffly_log, "loaded jpeg image, width: %u, height: %u, chn_c: %u\n", __image->width, __image->height, __image->chn_c);
	return FFLY_SUCCESS;
}
