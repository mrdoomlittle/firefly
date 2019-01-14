# include "framebuff.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "context.h"
# include "raise.h"
# include "../system/io.h"
# include "pixel.h"
# include "../dep/mem_set.h"
/*
	add clear function
*/
struct nt_framebuff*
nt_framebuff_creat(ff_uint_t __width, ff_uint_t __height) {
	struct nt_framebuff *fb;

	fb = (struct nt_framebuff*)__ffly_mem_alloc(sizeof(struct nt_framebuff));
	ff_uint_t wt, ht;
	wt = (__width+((1<<TILESZ)-1))>>TILESZ;
	ht = (__height+((1<<TILESZ)-1))>>TILESZ;

	ff_uint_t nt;
	nt = wt*ht;
	fb->tw = wt;
	fb->th = ht;
	fb->width = __width;
	fb->height = __height;
	fb->tiles = (struct nt_tile**)__ffly_mem_alloc(nt*sizeof(struct nt_tile*));
	fb->n = nt;

	struct nt_tile **t, **e;
	t = fb->tiles;
	e = t+nt;
	while(t != e)
		*(t++) = NULL;
	fb->dpb = (ff_u16_t*)__ffly_mem_alloc(__width*__height*sizeof(ff_u16_t));
	ffly_mem_set(fb->dpb, 255, __width*__height*sizeof(ff_u16_t));
	return fb;
}

void nt_framebuff_del(struct nt_framebuff *__fb) {
	struct nt_tile **t, **e;
	t = __fb->tiles;
	e = t+__fb->n;

	struct nt_tile *p;

	while(t != e) {
		if ((p = *(t++)) != NULL) {
			nt_tile_unmap(p);
			nt_tile_destroy(p);
		}
	}
	__ffly_mem_free(__fb->tiles);
	__ffly_mem_free(__fb->dpb);
	__ffly_mem_free(__fb);
}

# include "plate.h"
void nt_fb_set(void) {
	struct nt_framebuff *fb;

	fb = (struct nt_framebuff*)nt_plate_get(*(ff_u32_t*)nt_raise_p);
	nt_ctx->fb = fb;
}

void nt_fb_new(void) {
	ff_u32_t plate;
	ff_u32_t width, height;

	width = *(ff_u32_t*)nt_raise_p;
	height = *(ff_u32_t*)(nt_raise_p+4);
	plate = *(ff_u32_t*)(nt_raise_p+8);

	nt_plate_put(plate, nt_framebuff_creat(width, height));
}

void nt_fb_destroy(void) {
	nt_framebuff_del((struct nt_framebuff*)nt_plate_get(*(ff_u32_t*)nt_raise_p));
}

void static
fb_write(struct nt_framebuff *__fb, void *__buf, ff_uint_t __x, ff_uint_t __y, ff_uint_t __width, ff_uint_t __height) {
	ff_uint_t x, y;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;

	struct nt_tile *t, **tp;
	ff_u8_t *d, *s;
	y = __y;
	while(y != __y+__width && y < __fb->height) {
		x = __x;
		while(x != __x+__height && x <__fb->width) {
			tx = x>>TILESZ;
			ty = y>>TILESZ;
			txo = x-(tx<<TILESZ);
			tyo = y-(ty<<TILESZ);

			if (!(t = *(tp = tile_at(tx, ty, __fb)))) {
				t = (*tp = nt_tile_new(TILESZ));
				nt_tile_map(t);
			}
	

			d = tilepx(t, txo, tyo);
			s = ((ff_u8_t*)__buf)+(((x-__x)+((y-__y)*__width))*4);
			*(ff_u32_t*)d = *(ff_u32_t*)s;
			//nt_setpix(s, d);
			x++;
		}
		y++;
	}
}

# include "renderbuff.h"
// murge everything into one frame
void static
converge(void) {
	struct nt_renderbuff *rb;
	rb = nt_ctx->rb_top;
	while(rb != NULL) {
		struct nt_tile **t;
		ff_uint_t x, y;
		ff_uint_t xx, yy;
		y = 0;
		while(y != rb->th) {
			x = 0;
			while(x != rb->tw) {
				t = rb->tiles+x+(y*rb->tw); 
				if (*t != NULL) {
					xx = x<<RB_TILESZ;
					yy = y<<RB_TILESZ;
					fb_write(rb->dst, (*t)->map, xx, yy, 1<<RB_TILESZ, 1<<RB_TILESZ);	
				}
				x++;
			}
			y++;
		}

		rb = rb->next;
	}
}

void nt_putframe(void) {
	converge();
	ff_u8_t *dst;
	ff_uint_t sc_x, sc_y;
	ff_uint_t width, height;
	
	ff_u8_t *p;
	p = nt_raise_p;
	dst = *(ff_u8_t**)p;

	p+=sizeof(ff_u8_t*);

	sc_x = *(ff_u32_t*)p;
	sc_y = *(ff_u32_t*)(p+4);
	width = *(ff_u32_t*)(p+8);
	height = *(ff_u32_t*)(p+12);

	ffly_printf("%p : %u, %u, %u, %u, %p\n", dst, sc_x, sc_y, width, height, nt_ctx->fb);

	struct nt_context *ctx;
	ctx = nt_ctx;
	ffly_printf("fb, width: %ut, height: %ut\n", ctx->fb->width, ctx->fb->height);

	struct nt_tile *t;
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
			txo = (x+sc_x)-(tx<<TILESZ);
			tyo = (y+sc_y)-(ty<<TILESZ);

			s = tilepx(t, txo, tyo);
			*(ff_u32_t*)d = *(ff_u32_t*)s;
		_sk:
			x++;
		}
		y++;
	}
}
