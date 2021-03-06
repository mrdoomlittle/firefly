# include "tex.h"
# include "raise.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
# include "../stdio.h"
# include "plate.h"
#define is_flag(__flags, __flag) \
	(((__flags)&(__flag))>0)

nt_texp static
tex_new(nt_texbufp __buf, ff_u32_t __width, ff_u32_t __height) {
	printf("new texture with buff size of %u\n", __buf->size);
	nt_texp tx;
	tx = (nt_texp)__ffly_mem_alloc(sizeof(struct nt_tex));
	tx->b = __buf;
	tx->width = __width;
	tx->height = __height;
	return tx;
}

void static
tex_destroy(nt_texp __tex) {
	__ffly_mem_free(__tex);
}

nt_texbufp static
texbuf_new(ff_u32_t __size) {
	printf("new texbuf %u\n", __size);
	nt_texbufp buf;
	buf = (nt_texbufp)__ffly_mem_alloc(sizeof(struct nt_texbuf));
	buf->size = __size;
	buf->flags = 0x00;
	return buf;
}

void static
texbuf_destroy(nt_texbufp __buf) {
	if (is_flag(__buf->flags, NT_TB_MAPPED)) {
		
	}
	__ffly_mem_free(__buf);
}

void static
texbuf_map(nt_texbufp __buf) {
	__buf->data = __ffly_mem_alloc(__buf->size);
	__buf->flags |= NT_TB_MAPPED;
}

void static
texbuf_unmap(nt_texbufp __buf) {
	__ffly_mem_free(__buf->data);
#ifndef be_vigilant
	__buf->flags ^= NT_TB_MAPPED;
#else
	__buf->flags &= ~NT_TB_MAPPED;
#endif
}

void static
texbuf_write(nt_texbufp __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	ffly_mem_cpy(((ff_u8_t*)__buf->data)+__offset, __src, __size);
}

void static
texbuf_read(nt_texbufp __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	ffly_mem_cpy(__dst, ((ff_u8_t*)__buf->data)+__offset, __size);
}

void nt_tex_new(void) {
	nt_texp tx;

	ff_u32_t width, height;
	width = *(ff_u32_t*)(nt_raise_p+8);
	height = *(ff_u32_t*)(nt_raise_p+12);

	tx = tex_new((nt_texbufp)nt_plate_get(*(ff_u32_t*)(nt_raise_p+4)), width, height);
	nt_plate_put(*(ff_u32_t*)nt_raise_p, tx);	
}

void nt_tex_destroy(void) {
	nt_texp tx;
	tx = (nt_texp)nt_plate_get(*(ff_u32_t*)nt_raise_p);

	tex_destroy(tx);
}

void nt_texbuf_new(void) {
	nt_texbufp buf;
	buf = texbuf_new(*(ff_u32_t*)(nt_raise_p+4));

	nt_plate_put(*(ff_u32_t*)nt_raise_p, buf);
}

void nt_texbuf_destroy(void) {
	nt_texbufp buf;
	buf = (nt_texbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);
	texbuf_destroy(buf);
}

void nt_texbuf_map(void) {
	nt_texbufp buf;
	buf = (nt_texbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);

	texbuf_map(buf);
}

void nt_texbuf_unmap(void) {
	nt_texbufp buf;
	buf = (nt_texbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);

	texbuf_unmap(buf);
}

void nt_texbuf_write(void) {
	nt_texbufp buf;
	buf = (nt_texbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);
	ff_u32_t offset, size;
	void *src;

	offset = *(ff_u32_t*)(nt_raise_p+4);
	size = *(ff_u32_t*)(nt_raise_p+8);
	src = *(void**)(nt_raise_p+12);
	texbuf_write(buf, offset, size, src);
}

void nt_texbuf_read(void) {
	nt_texbufp buf;
	buf = (nt_texbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);
	ff_u32_t offset, size;
	void *dst;

	offset = *(ff_u32_t*)(nt_raise_p+4);
	size = *(ff_u32_t*)(nt_raise_p+8);
	dst = *(void**)(nt_raise_p+12);
	texbuf_read(buf, offset, size, dst);
}
