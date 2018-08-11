# include "p.h"
# include "../p.h"
# include "../system/file.h"
# include "../system/error.h"
# include "../data/str_cmb.h"
void static
_read(void *__buf, ff_u32_t __offset, ff_uint_t __n, void *__arg) {
	ffly_fpread((FF_FILE*)__arg, __buf, __n, __offset);
}
void static
_write(void *__buf, ff_u32_t __offset, ff_uint_t __n, void *__arg) {
	ffly_fpwrite((FF_FILE*)__arg, __buf, __n, __offset);
}

void ffly_im_p_se(char const *__path, ffly_imagep __im) {
	FF_FILE *f;
	ff_pstrucp p;

	ff_err_t err;
	f = ffly_fopen(__path, FF_O_WRONLY|FF_O_CREAT|FF_O_TRUNC, S_IRUSR|S_IWUSR, &err);
	p = ff_p_struc_creat();

	ff_p_io_init(p, _read, _write, f);

	ff_p_set_prop(p, __im->width, __im->height);
	ff_p_writer_init(p);

	ff_p_set_hdr(p);
	ff_uint_t row;

	row = 0;
	for(;row != __im->height;row++)
		ff_p_row_write(p, __im->pixels+(row*(__im->width*4)));

	ff_p_struc_destroy(p);
	ffly_fclose(f);
}

void ffly_im_p_ld(char const *__path, ffly_imagep __im) {
	FF_FILE *f;
	ff_pstrucp p;

	ff_err_t err;
	f = ffly_fopen(__path, FF_O_RDONLY, 0, &err);
	p = ff_p_struc_creat();
	struct ff_pinfo info;
	ff_p_info_read(p, &info);

	__im->width = info.width;
	__im->height = info.height;

	ff_p_io_init(p, _read, _write, f);

	ff_p_reader_init(p);

	ff_uint_t row;

	row = 0;
	for(;row != info.height;row++)
		ff_p_row_read(p, __im->pixels+(row*(__im->width*4)));

	ff_p_struc_destroy(p);
	ffly_fclose(f);
}
