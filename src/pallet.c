# include "pallet.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/draw.h"
# include "system/io.h"
# include "system/log.h"
# define tile_at(__pallet, __x, __y) \
	(*((__pallet)->tiles+(__x)+((__y)*(__pallet)->xl)))
# define at(__pallet, __x, __y) \
	((__pallet)->tiles+(__x)+((__y)*(__pallet)->xl))
ffly_tilep ffly_tile_at(ffly_palletp __pallet, ff_uint_t __x, ff_uint_t __y) {
	return tile_at(__pallet, __x, __y);
}
# include "dep/mem_cpy.h"
void ffly_pallet_copy(ffly_palletp __src, ffly_palletp __dst, ff_uint_t __x, ff_uint_t __y) {
	ff_uint_t x, y = 0;
	ffly_tilep src, *dst;
	ff_uint_t src_tz, dst_tz, xdis, ydis;

	src_tz = 1<<__src->tilesize;
	dst_tz = 1<<__dst->tilesize;

	// displace it by ...x amount from tile x so tile X + displace
	xdis = __x-((__x>>__dst->tilesize)*dst_tz);
	ydis = __y-((__y>>__dst->tilesize)*dst_tz);
	while(y != __src->yl) {
		x = 0;
		while(x != __src->xl) {
			log("tile, %u:%u\n", x, y);
			if (!(src = tile_at(__src, x, y)))
				goto _sk;
			if ((src->bits&TILE_BLANK)>0)
				goto _sk;
			dst = at(__dst, x+(__x>>__dst->tilesize), y+(__y>>__dst->tilesize));

			src->child = *dst;
			*dst = src;
			src->xdis = xdis;
			src->ydis = ydis;
		_sk:
			x++;
		}
		y++;
	}
}

void ffly_pallet_draw(ffly_palletp __src, ff_u8_t *__dst, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y) {
	ff_uint_t x, y = __y;
	ffly_tilep src, cur, *p;
	ff_u16_t depth;
	while(y != __y+__src->yl) {
		x = __x;
		while(x != __x+__src->xl) {
# ifdef __ffly_debug
			log("tile, %u:%u\n", x, y);
# endif
			if (!*(p = at(__src, x, y)))
				goto _sk;
			src = *p;

			cur = src;
			depth = 0;
			while(cur != NULL) {
				ffly_tile_draw(cur, __dst, __width, __height, x*(1<<__src->tilesize), y*(1<<__src->tilesize));
				cur = cur->child;
				depth++;
			}
			log("tile depth %u.\n", depth);
			*p = NULL;
		_sk:
			x++;
		}
		y++;
	}
}

void ffly_pallet_init(ffly_palletp __pallet, ff_uint_t __width, ff_uint_t __height, ff_u8_t __tilesize) {
	ff_uint_t xl, yl, size;

	xl = ((__width+(0xffffffffffffffff>>(64-__tilesize)))>>__tilesize);
	yl = ((__height+(0xffffffffffffffff>>(64-__tilesize)))>>__tilesize);
	size = (__pallet->xl = xl)*(__pallet->yl = yl);

	__pallet->tiles = (ffly_tilep*)__ffly_mem_alloc(size*sizeof(ffly_tilep));
	ffly_tilep *cur = __pallet->tiles;
	ffly_tilep *end = cur+size;
	while(cur != end) {
		*(cur++) = NULL;
	}
	__pallet->tilesize = __tilesize;
}

# include "pixel.h"
/*
	will be replaced
*/
# include "dep/mem_cpy.h"
# include "dep/mem_set.h"
void ffly_pallet_update(ffly_palletp __pallet, void *__p, ff_uint_t __width, ff_uint_t __height) {
	ff_uint_t off = 0;
	ff_u8_t tilesize = 1<<__pallet->tilesize;
	ffly_tilep *dst;
	ff_uint_t x, y = 0;

	ff_uint_t xoh = (__pallet->xl*tilesize)-__width;
	ff_u8_t *p;
	ff_uint_t yy;
	while(y != __pallet->yl) {
		x = 0;
		while(x != __pallet->xl) {		
			/* get tile at?
			*/
			if (!*(dst = at(__pallet, x, y)))
				/*
					if no tile is located create one
				*/
				*dst = ffly_tile_creat(__pallet->tilesize);
			// note: *4 is r,g,b,a

			ffly_tilep d;
			d = *dst;
			d->bits &= ~TILE_BLANK;

			yy = 0;
			ff_uint_t xleft = __width-(x*tilesize);
			while(yy != tilesize && (y*tilesize)+yy < __height) {
				p = (ff_u8_t*)__p+(((x*tilesize)+(y*((__pallet->xl*tilesize)*tilesize)))*4);
				p = p+(yy*(__pallet->xl*tilesize)*4);
				p = p-(((y*tilesize)+yy)*(xoh*4));

				ff_uint_t size = (xleft<tilesize?xleft:tilesize)*4;
				if (p+size>((ff_u8_t*)__p)+(__width*__height*4)) {
					ffly_printf("error.\n");
					return;
				}

				ffly_mem_cpy(d->p+((yy*tilesize)*4), p, size);
				yy++;
			}
			x++;
		}
		y++;
	}
}

void ffly_pallet_de_init(ffly_palletp __pallet) {
	ffly_tilep *cur = __pallet->tiles;
	ffly_tilep *end = cur+(__pallet->xl*__pallet->yl);
	while(cur != end) {
		if (*cur != NULL)
			ffly_tile_del(*cur);
		cur++;
	}
	__ffly_mem_free(__pallet->tiles);
}
