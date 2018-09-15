# include "pallet.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/draw.h"
# include "system/io.h"
# include "system/log.h"
# define tile_at(__pallet, __x, __y) \
	(*((__pallet)->tiles+(__x)+((__y)*(__pallet)->wt)))
# define at(__pallet, __x, __y) \
	((__pallet)->tiles+(__x)+((__y)*(__pallet)->wt))
ffly_tilep ffly_tile_at(ffly_palletp __pallet, ff_uint_t __x, ff_uint_t __y) {
	return tile_at(__pallet, __x, __y);
}

ffly_palletp ffly_pallet_new(ff_uint_t __width, ff_uint_t __height, ff_u8_t __ts) {
	ffly_palletp pt;

	pt = (ffly_palletp)__ffly_mem_alloc(sizeof(struct ffly_pallet));
	ffly_pallet_init(pt, __width, __height, __ts);
	return pt;
}

void ffly_pallet_distroy(ffly_palletp __pt) {
	ffly_pallet_de_init(__pt);
	__ffly_mem_free(__pt);
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
	while(y != __src->ht) {
		x = 0;
		while(x != __src->wt) {
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

void ffly_pallet_draw(ffly_palletp __src, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y) {
	ff_uint_t x, y = __y;
	ffly_tilep src, cur, *p;
	ff_u16_t depth;
	while(y != __y+__src->ht) {
		x = __x;
		while(x != __x+__src->wt) {
# ifdef __ffly_debug
			log("tile, %u:%u\n", x, y);
# endif
			if (!*(p = at(__src, x, y)))
				goto _sk;
			src = *p;

			cur = src;
			depth = 0;
			while(cur != NULL) {
				ffly_tile_draw(cur, __width, __height, x*(1<<__src->tilesize), y*(1<<__src->tilesize));
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
	ff_uint_t wt, ht, size;

	wt = ((__width+(0xffffffffffffffff>>(64-__tilesize)))>>__tilesize);
	ht = ((__height+(0xffffffffffffffff>>(64-__tilesize)))>>__tilesize);
	size = (__pallet->wt = wt)*(__pallet->ht = ht);

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
/*
	for now simple but inefficient and less error while working on things
*/
void ffly_pallet_write(ffly_palletp __pallet, void *__src, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y) {
	ffly_tilep *dst;

	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_u8_t tsz;
	tsz = __pallet->tilesize;
	ff_uint_t x, y;
	ff_u8_t *s;
	y = 0;
	while(y != __height) {
		x = 0;
		while(x != __width) {
			tx = (x+__x)>>tsz;
			ty = (y+__y)>>tsz;

			txo = (x+__x)-(tx*(1<<tsz));
			tyo = (y+__y)-(ty*(1<<tsz));

			if (!*(dst = at(__pallet, tx, ty))) {
				*dst = ffly_tile_creat(tsz);
			}

			ffly_tilep d;
			d = *dst;
			d->bits &= ~TILE_BLANK;
			s = ((ff_u8_t*)__src)+((x+(y*__width))*4);
			ffly_mo_write(d->m, s, 4, (txo+(tyo*(1<<tsz)))*4);
			x++;
		}
		y++;
	}
}


void ffly_pallet_read(ffly_palletp __pallet, void *__dst,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ff_uint_t x, y;

	ff_uint_t tx, ty;

	ff_u8_t tsz;

	tsz = __pallet->tilesize;

	ff_uint_t txo, tyo;
	struct ffly_tile *t;

	ff_u8_t *d;
	y = 0;
	while(y != __height) {
		x = 0;
		while(x != __width) {
			tx = (x+__x)>>tsz;
			ty = (y+__y)>>tsz;

			txo = (x+__x)-(tx*(1<<tsz));
			tyo = (y+__y)-(ty*(1<<tsz));

			d = ((ff_u8_t*)__dst)+((x+(y*__width))*4);
			if (!(t = *at(__pallet, tx, ty))) {
				goto _sk;
			}

			if ((t->bits&TILE_BLANK)>0)
				goto _sk;
			ffly_mo_read(t->m, d, 4, (txo+(tyo*(1<<tsz)))*4);		
		_sk:
			x++;
		}
		y++;
	}

}

void ffly_pallet_de_init(ffly_palletp __pallet) {
	ffly_tilep *cur = __pallet->tiles;
	ffly_tilep *end = cur+(__pallet->wt*__pallet->ht);
	while(cur != end) {
		if (*cur != NULL)
			ffly_tile_del(*cur);
		cur++;
	}
	__ffly_mem_free(__pallet->tiles);
}
