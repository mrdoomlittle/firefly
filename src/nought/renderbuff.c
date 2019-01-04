# include "renderbuff.h"
# include "raise.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "context.h"
# include "plate.h"
# include "../system/io.h"
struct nt_renderbuff* nt_renderbuff_creat(ff_uint_t __width, ff_uint_t __height) {
	struct nt_renderbuff *rb;
	rb = (struct nt_renderbuff*)__ffly_mem_alloc(sizeof(struct nt_renderbuff));

	ffly_printf("new render buffer: %u:%u\n", __width, __height);

	ff_uint_t wt, ht;

	wt = (__width+((1<<RB_TILESZ)-1))>>RB_TILESZ;
	ht = (__height+((1<<RB_TILESZ)-1))>>RB_TILESZ;
	rb->tw = wt;
	rb->th = ht;

	rb->width = __width;
	rb->height = __height;
	ff_uint_t nt;
	nt = wt*ht;
	rb->n = nt;

	rb->tiles = (struct nt_tile**)__ffly_mem_alloc(nt*sizeof(struct nt_tile*));
	rb->dst = NULL;
	
	struct nt_tile **t, **e;
	t = rb->tiles;
	e = t+nt;
	while(t != e)
		*(t++) = NULL;

	rb->bk = &nt_ctx->rb_top;
	if (nt_ctx->rb_top != NULL)
		nt_ctx->rb_top->bk = &rb->next;
	rb->next = nt_ctx->rb_top;
	nt_ctx->rb_top = rb;
	return rb;
}

void nt_renderbuff_del(struct nt_renderbuff *__rb) {
	struct nt_tile **t, **e;
	t = __rb->tiles;
	e = t+__rb->n;

	struct nt_tile *p;

	while(t != e) {
		if ((p = *(t++)) != NULL) {
			nt_tile_unmap(p);
			nt_tile_destroy(p);
		}
	}
	__ffly_mem_free(__rb->tiles);
	*__rb->bk = __rb->next;
	if (__rb->next != NULL)
		__rb->next->bk = __rb->bk;

	__ffly_mem_free(__rb);
}

void nt_rb_bind(void) {
	struct nt_renderbuff *rb;
	struct nt_framebuff *fb;

	rb = (struct nt_renderbuff*)nt_plate_get(*(ff_u32_t*)nt_raise_p);
	fb = (struct nt_framebuff*)nt_plate_get(*(ff_u32_t*)(nt_raise_p+4));

	rb->dst = fb;
	nt_ctx->rb = rb;
	ffly_printf("render buff bind: %u:%u\n", rb->width, rb->height);
}

void nt_rb_new(void) {
	ff_u32_t plate;
	ff_u32_t width, height;

	width = *(ff_u32_t*)nt_raise_p;
	height = *(ff_u32_t*)(nt_raise_p+4);
	plate = *(ff_u32_t*)(nt_raise_p+8);
	nt_plate_put(plate, nt_renderbuff_creat(width, height));
}

void nt_rb_destroy(void) {
	nt_renderbuff_del((struct nt_renderbuff*)nt_plate_get(*(ff_u32_t*)nt_raise_p));
}

