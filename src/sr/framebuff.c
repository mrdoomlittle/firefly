# include "framebuff.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "context.h"
# include "raise.h"
# include "../system/io.h"
struct sr_framebuff*
sr_framebuff_creat(ff_uint_t __width, ff_uint_t __height) {
	struct sr_framebuff *fb;

	fb = (struct sr_framebuff*)__ffly_mem_alloc(sizeof(struct sr_framebuff));
	ff_uint_t width, height;
	width = (__width+((1<<TILESZ)-1))>>TILESZ;
	height = (__height+((1<<TILESZ)-1))>>TILESZ;

	fb->rw = __width;
	fb->rh = __height;
	fb->width = width;
	fb->height = height;
	fb->tiles = (struct sr_tile**)__ffly_mem_alloc((width*height)*sizeof(struct sr_tile*));
	ff_uint_t n;
	n = width*height;
	fb->n = n;

	struct sr_tile **t, **e;
	t = fb->tiles;
	e = t+n;
	while(t != e)
		*(t++) = NULL;
	return fb;
}

void sr_framebuff_del(struct sr_framebuff *__fb) {
	struct sr_tile **t, **e;
	t = __fb->tiles;
	e = t+__fb->n;

	struct sr_tile *p;

	while(t != e) {
		if ((p = *(t++)) != NULL) {
			sr_tile_unmap(p);
			sr_tile_destroy(p);
		}
	}
	__ffly_mem_free(__fb->tiles);
	__ffly_mem_free(__fb);
}


void sr_fb_set(void) {
	struct sr_framebuff *fb;

	fb = *(struct sr_framebuff**)stack_at(*(ff_u16_t*)sr_raise_p);
	sr_ctx->fb = fb;
}

void sr_fb_new(void) {
	struct sr_framebuff **fb;

	ff_u32_t width, height;

	width = *(ff_u32_t*)sr_raise_p;
	height = *(ff_u32_t*)(sr_raise_p+4);
	fb = ((struct sr_framebuff**)stack_at(*(ff_u16_t*)(sr_raise_p+8)));

	*fb = sr_framebuff_creat(width, height);
}

void sr_fb_destroy(void) {
	struct sr_framebuff *fb;

	fb = *(struct sr_framebuff**)stack_at(*(ff_u16_t*)sr_raise_p);
	sr_framebuff_del(fb);
}

void sr_putframe(void) {
	ff_u8_t *dst;
	ff_uint_t sc_x, sc_y;
	ff_uint_t width, height;
	
	ff_u8_t *p;
	p = sr_raise_p;
	dst = *(ff_u8_t**)p;

	p+=sizeof(ff_u8_t*);

	sc_x = *(ff_u32_t*)p;
	sc_y = *(ff_u32_t*)(p+4);
	width = *(ff_u32_t*)(p+8);
	height = *(ff_u32_t*)(p+12);

	ffly_printf("%p : %u, %u, %u, %u, %p\n", dst, sc_x, sc_y, width, height, sr_ctx->fb);

	struct sr_context *ctx;
	ctx = sr_ctx;
	ffly_printf("fb, width: %ut, height: %ut\n", ctx->fb->width, ctx->fb->height);

	struct sr_tile *t;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;

	ff_u8_t *d, *s;
	ff_uint_t x, y;
	y = 0;
	while(y != height) {
		x = 0;
		while(x != width) {
			d = dst+((x+(y*width))*4);
			tx = (x+sc_x)>>TILESZ;
			ty = (y+sc_y)>>TILESZ;
			if (!(t = *tile_at(tx, ty, ctx->fb))) {
				*(ff_u32_t*)d = 21; // debug
//				ffly_printf("skipping non existent tile.\n");
				// skip tile
				goto _sk;				
			}
			txo = (x+sc_x)-(tx*(1<<TILESZ));
			tyo = (y+sc_y)-(ty*(1<<TILESZ));

			s = tilepx(t, txo, tyo);
			*(ff_u32_t*)d = *(ff_u32_t*)s;
		_sk:
			x++;
		}
		y++;
	}
}
