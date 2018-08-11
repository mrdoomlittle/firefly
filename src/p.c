# include "p.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
void ff_p_info_read(ff_pstrucp __s, ff_pinfop __info) {
	__info->width = __s->width;
	__info->height = __s->height;
}

void ff_p_row_read(ff_pstrucp __s, ff_u8_t *__dst) {
	__s->read(__dst, __s->off+(((__s->row_num++)*__s->width)*4), __s->width*4, __s->arg);
}

void ff_p_row_write(ff_pstrucp __s, ff_u8_t *__src) {
	__s->write(__src, __s->off+(((__s->row_num++)*__s->width)*4), __s->width*4, __s->arg);
}

void ff_p_set_prop(ff_pstrucp __s, ff_uint_t __width, ff_uint_t __height) {
	__s->width = __width;
	__s->height = __height;
}

void ff_p_set_hdr(ff_pstrucp __s) {
	struct ff_pimage im;
	im.width = __s->width;
	im.height = __s->height;
	__s->write(&im, 0, sizeof(struct ff_pimage), __s->arg);
}

void ff_p_io_init(ff_pstrucp __s,
	void(*__read)(void*, ff_u32_t, ff_uint_t, void*),
	void(*__write)(void*, ff_u32_t, ff_uint_t, void*), void *__arg)
{
	__s->read = __read;
	__s->write = __write;
	__s->arg = __arg;
}

void ff_p_reader_init(ff_pstrucp __s) {
	struct ff_pimage im;
	__s->read(&im, 0, sizeof(struct ff_pimage), __s->arg);

	__s->width = im.width;
	__s->height = im.height;
	__s->off = sizeof(struct ff_pimage);
}

void ff_p_writer_init(ff_pstrucp __s) {
	__s->off = sizeof(struct ff_pimage);
}

ff_pstrucp ff_p_struc_creat(void) {
	ff_pstrucp s;
	s = (ff_pstrucp)__ffly_mem_alloc(sizeof(struct ff_pstruc));
	s->off = 0;
	return s;
}

void ff_p_struc_destroy(ff_pstrucp __s) {
	__ffly_mem_free(__s);
}
