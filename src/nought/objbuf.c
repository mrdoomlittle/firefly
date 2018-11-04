# include "objbuf.h"
# include "raise.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
#define is_flag(__flags, __flag) \
	(((__flags)&(__flag))>0)
nt_objbufp static
objbuf_new(ff_u32_t __size) {
	nt_objbufp buf;
	buf = (nt_objbufp)__ffly_mem_alloc(sizeof(struct nt_objbuf));
	buf->size = __size;
	buf->flags = 0x00;
	return buf;
}

void static
objbuf_destroy(nt_objbufp __buf) {
	if (is_flag(__buf->flags, NT_OB_MAPPED)) {
		
	}
	__ffly_mem_free(__buf);
}

void static
objbuf_map(nt_objbufp __buf) {
	__buf->data = __ffly_mem_alloc(__buf->size);
	__buf->flags |= NT_OB_MAPPED;
}

void static
objbuf_unmap(nt_objbufp __buf) {
	__ffly_mem_free(__buf->data);
#ifndef be_vigilant
	__buf->flags ^= NT_OB_MAPPED;
#else
	__buf->flags &= NT_OB_MAPPED;
#endif
}

void static
objbuf_write(nt_objbufp __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	ffly_mem_cpy(((ff_u8_t*)__buf->data)+__offset, __src, __size);
}

void static
objbuf_read(nt_objbufp __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	ffly_mem_cpy(__dst, ((ff_u8_t*)__buf->data)+__offset, __size);
}

void nt_objbuf_new(void) {
	nt_objbufp *buf;
	buf = (nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);
	ff_u32_t sz;
	sz = *(ff_u32_t*)(nt_raise_p+2);
	*buf = objbuf_new(sz);
}

void nt_objbuf_destroy(void) {
	nt_objbufp buf;
	buf = *(nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);
	objbuf_destroy(buf);
}

void nt_objbuf_map(void) {
	nt_objbufp buf;
	buf = *(nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);

	objbuf_map(buf);
}

void nt_objbuf_unmap(void) {
	nt_objbufp buf;
	buf = *(nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);

	objbuf_unmap(buf);
}

void nt_objbuf_write(void) {
	nt_objbufp buf;
	buf = *(nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);
	
	ff_u32_t offset, size;
	void *src;

	offset = *(ff_u32_t*)(nt_raise_p+2);
	size = *(ff_u32_t*)(nt_raise_p+6);
	src = *(void**)(nt_raise_p+10);
	objbuf_write(buf, offset, size, src);
}

void nt_objbuf_read(void) {
	nt_objbufp buf;
	buf = *(nt_objbufp*)stack_at(*(ff_u16_t*)nt_raise_p);

	ff_u32_t offset, size;
	void *dst;

	offset = *(ff_u32_t*)(nt_raise_p+2);
	size = *(ff_u32_t*)(nt_raise_p+6);
	dst = *(void**)(nt_raise_p+10);
	objbuf_read(buf, offset, size, dst);
}
