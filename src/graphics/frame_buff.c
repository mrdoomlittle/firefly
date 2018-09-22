# include "frame_buff.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# define TILESZ _ffly_tile_32
ffly_frame_buffp
ffly_frame_buff_creat(ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c, ff_err_t *__err) {
	ffly_frame_buffp fb;
	
	fb = (ffly_frame_buffp)__ffly_mem_alloc(sizeof(struct ffly_frame_buff));
	ffly_frame_buff_init(fb, __width, __height, __chn_c);
	return fb;
}

ff_err_t
ffly_frame_buff_del(ffly_frame_buffp __fb) {
	ffly_frame_buff_de_init(__fb);
	__ffly_mem_free(__fb);
}

# include "../context.h"
ff_u8_t static *tmp;
void ffly_fb_copy(ffly_frame_buffp __fb) {
	tmp = __ffly_mem_alloc(__fb->width*__fb->height*4);
	BRON_CONTEXT->driver.frame(tmp, 0, 0, __fb->width, __fb->height);

}

void ffly_fb_yank(ffly_frame_buffp __fb) {
	ffly_fb_write(__fb, tmp, __fb->width, __fb->height, 0, 0);
	__ffly_mem_free(tmp);
}

ff_err_t // should use mmap to allocate the frame?
ffly_frame_buff_init(ffly_frame_buffp __fb, ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c) {
	ff_uint_t wt, ht, sz;

	wt = (__width+((1<<TILESZ)-1))>>TILESZ;
	ht = (__height+((1<<TILESZ)-1))>>TILESZ;
	__fb->tiles = (ffly_tilep*)__ffly_mem_alloc((sz = (wt*ht))*sizeof(ffly_tilep));

	__fb->n = sz;
	ffly_tilep *t, *e;
	t = __fb->tiles;
	e = t+sz;
	while(t != e)
		*(t++) = ffly_tile_creat(TILESZ);
	__fb->wt = wt;
	__fb->ht = ht;
	__fb->width = __width;
	__fb->height = __height;
	__fb->chn_c = __chn_c;
	__fb->frame = __ffly_mem_alloc(__width*__height*4);
}

ff_err_t
ffly_frame_buff_de_init(ffly_frame_buffp __fb) {
	ffly_tilep *t, *e;
	t = __fb->tiles;
	e = t+__fb->n;
	while(t != e)
		ffly_tile_del(*(t++));
	__ffly_mem_free(__fb->tiles);
	if (__fb->frame != NULL)
		__ffly_mem_free(__fb->frame);
}

/*
	generate the frame
*/
void ffly_fb_gen(ffly_frame_buffp __fb) {
	ffly_fb_read(__fb, __fb->frame, __fb->width, __fb->height, 0, 0);
}

# define at(__fb, __x, __y) \
	((__fb)->tiles+(__x)+((__y)*(__fb)->wt))

/*
	only for now simple but inefficient
*/
void ffly_fb_read(ffly_frame_buffp __fb, void *__dst,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ff_u8_t *d;
	ffly_tilep *t, s;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_uint_t x, y;
	y = 0;
	while(y != __height) {
		x = 0;
		while(x != __width) {
			ff_uint_t rx, ry;
			rx = __x+x;
			ry = __y+y;
			tx = rx>>TILESZ;
			ty = ry>>TILESZ;
			txo = rx-(tx*(1<<TILESZ));
			tyo = ry-(ty*(1<<TILESZ));
			s = *(t = at(__fb, tx, ty));
			d = ((ff_u8_t*)__dst)+((x+(y*__width))*4);
			ffly_mo_read(s->m, d, 4, (txo+(tyo*(1<<TILESZ)))*4);
			x++;
		}
		y++;
	}
}

void ffly_fb_write(ffly_frame_buffp __fb, void *__src,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __x, ff_uint_t __y)
{
	ff_u8_t *s;
	ffly_tilep *t, d;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_uint_t x, y;
	y = 0;
	while(y != __height) {
		x = 0;
		while(x != __width) {
			ff_uint_t rx, ry;
			rx = __x+x;
			ry = __y+y;
			tx = rx>>TILESZ;
			ty = ry>>TILESZ;
			txo = rx-(tx*(1<<TILESZ));
			tyo = ry-(ty*(1<<TILESZ));
			d = *(t = at(__fb, tx, ty));
			s = ((ff_u8_t*)__src)+((x+(y*__width))*4);
			ffly_mo_write(d->m, s, 4, (txo+(tyo*(1<<TILESZ)))*4);
			x++;
		}
		y++;
	}
}

ffly_frame_buffp __frame_buff__;
