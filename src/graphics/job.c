# include "job.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
# include "../system//errno.h"
# include "../system/err.h"
char const static *kstr(ff_u8_t __kind) {
	switch(__kind) {
		case _grj_pixfill:
			return "pixfill";
		case _grj_pixcopy:
			return "pixcopy";
		case _grj_pixdraw:
			return "pixdraw";
	}
	return "unknown";
}

static struct ffly_grj *top = NULL;
struct ffly_grj* ffly_grj_mk(ff_u8_t __kind, void *__p) {
	struct ffly_grj *job = (struct ffly_grj*)__ffly_mem_alloc(sizeof(struct ffly_grj));
	if (job != NULL) {
		if (top != NULL)
			top->prev = job;
		*job = (struct ffly_grj) {
			.kind = __kind,
			.p = __p,
			.prev = NULL,
			.next = top
		};
		top = job;
	}
	return job;
}



#define parsz3(__t0, __t1, __t2) \
	(sizeof(__t0)+sizeof(__t1)+sizeof(__t2))
#define parsz4(__t0, __t1, __t2, __t3) \
	(sizeof(__t0)+sizeof(__t1)+sizeof(__t2)+sizeof(__t3))
#define parsz5(__t0, __t1, __t2, __t3, __t4) \
	(sizeof(__t0)+sizeof(__t1)+sizeof(__t2)+sizeof(__t3)+sizeof(__t4))

#define put(__dst, __src, __ty) \
	*((__ty*)(__dst)) = __src
/*
	might be worth having its own allocator for parameters memory
*/


/*
	TODO:
		par should be data+(offset) ????  and not par[num]???? better?¿??
*/
struct ffly_grj*
ffly_grj_pixfill(ff_u32_t __npix, ffly_colour_t __colour, ff_u32_t __off) {
	ffly_fprintf(ffly_log, "pixfill job.\n");

	ff_u8_t *p;
	ff_uint_t sz;

	sz = parsz3(ff_u32_t, ffly_colour_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_pixfill, p);
	void **par = j->par;
    
	ff_u8_t *npix, *colour, *off;
	npix = p;
	colour = p+4;
	off = p+4+sizeof(ffly_colour_t);

	put(npix, __npix, ff_u32_t);
	put(colour, __colour, ffly_colour_t);
	put(off, __off, ff_u32_t);

	*par = npix;
	par[1] = colour;
	par[2] = off;
	return j;
}

struct ffly_grj*
ffly_grj_pixdraw(ff_u32_t __x, ff_u32_t __y, void *__src,
	ff_u32_t __width, ff_u32_t __height)
{
	ffly_fprintf(ffly_log, "pixdraw job.\n");
	ff_u8_t *p;
	ff_uint_t sz;

	sz = parsz5(ff_u32_t, ff_u32_t, void*, ff_u32_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_pixdraw, p);
	void **par = j->par;

	ff_u8_t *x, *y, *src, *width, *height;
	x = p;
	y = p+4;
	src = p+8;
	width = p+16;
	height = p+20;

	put(x, __x, ff_u32_t);
	put(y, __y, ff_u32_t);
	put(src, __src, void*);
	put(width, __width, ff_u32_t);
	put(height, __height, ff_u32_t);

	*par = x;
	par[1] = y;
	par[2] = src;
	par[3] = width;
	par[4] = height;
	return j;
}

struct ffly_grj*
ffly_grj_pixcopy(ff_u32_t __x, ff_u32_t __y, ff_u32_t __width, ff_u32_t __height) {
	ffly_fprintf(ffly_log, "pixcopy job.\n");
	ff_u8_t *p;
	ff_uint_t sz;

	sz = parsz4(ff_u32_t, ff_u32_t, ff_u32_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_pixcopy, p);
	void **par = j->par;

	ff_u8_t *x, *y, *width, *height;

	x = p;
	y = p+4;
	width = p+8;
	height = p+12;

	put(x, __x, ff_u32_t);
	put(y, __y, ff_u32_t);
	put(width, __width, ff_u32_t);
	put(height, __height, ff_u32_t);
	
	*par = x;
	par[1] = y;
	par[2] = width;
	par[3] = height;
	return j;
}

struct ffly_grj*
ffly_grj_tdraw(ffly_tilep __tile, ff_u32_t __x, ff_u32_t __y)
{
	ffly_fprintf(ffly_log, "tdraw job.\n");
	ff_u8_t *p;
	ff_uint_t sz;

	sz = parsz3(ffly_tilep, ff_u32_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_tdraw, p);
	void **par = j->par;

	ff_u8_t *tile, *x, *y;
	tile = p;
	x = p+8;
	y = p+12;

	put(tile, __tile,  ffly_tilep);
	put(x, __x, ff_u32_t);
	put(y, __y, ff_u32_t);

	*par = tile;
	par[1] = x;
	par[2] = y;
	return j;
}

struct ffly_grj*
ffly_grj_tri2(struct ffly_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	ffly_fprintf(ffly_log, "tri2.\n");
	ff_u8_t *p;
	ff_uint_t sz;
	sz = parsz4(struct ffly_tri2, ff_u16_t, ff_u32_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_tri2, p);

	void **par = j->par;

	ff_u8_t *tri;
	ff_u8_t *tex;
	ff_u8_t *x, *y;

	tri = p;
	p+=sizeof(struct ffly_tri2);
	tex = p;
	x = p+2;
	y = p+6;

	put(tri, *__tri, struct ffly_tri2);
	put(tex, __tex, ff_u16_t);
	put(x, __x, ff_u32_t);
	put(y, __y, ff_u32_t);

	*par = tri;
	par[1] = tex;
	par[2] = x;
	par[3] = y;

	return j;
}

struct ffly_grj*
ffly_grj_tri3(struct ffly_tri3 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	ffly_fprintf(ffly_log, "tri3.\n");
	ff_u8_t *p;
	ff_uint_t sz;
	sz = parsz5(struct ffly_tri3, ff_u16_t, ff_u32_t, ff_u32_t, ff_u32_t);
	p = (ff_u8_t*)__ffly_mem_alloc(sz);

	struct ffly_grj *j = ffly_grj_mk(_grj_tri3, p);

	void **par = j->par;

	ff_u8_t *tri;
	ff_u8_t *tex;
	ff_u8_t *x, *y, *z;

	tri = p;
	p+=sizeof(struct ffly_tri3);
	tex = p;
	x = p+2;
	y = p+6;
	z = p+10;

	put(tri, *__tri, struct ffly_tri3);
	put(tex, __tex, ff_u16_t);
	put(x, __x, ff_u32_t);
	put(y, __y, ff_u32_t);
	put(z, __z, ff_u32_t);

	*par = tri;
	par[1] = tex;
	par[2] = x;
	par[3] = y;
	par[4] = z;
	return j;
}
void static
job_free(struct ffly_grj *__job) {
	if (__job == top) {
		if ((top = __job->next) != NULL)
			top->prev = NULL;
	} else {
		if (__job->prev != NULL)
			__job->prev->next = __job->next;
		if (__job->next != NULL)
			__job->next->prev = __job->prev;
	}
	__ffly_mem_free(__job->p);
	__ffly_mem_free(__job);
}

void ffly_grj_cleanup() {
	struct ffly_grj *cur = top, *bk;
	while(cur != NULL) {
		ffly_printf("job cleaning, %s\n", kstr(cur->kind));
		bk = cur;
		cur = cur->next;
		job_free(bk);
	}
}

# include "fill.h"
# include "copy.h"
# include "draw.h"

void static
_pixfill(struct ffly_grj *__job) {
	void **par = __job->par;
	ff_u32_t npix;
	ffly_colour_t colour;
	ff_u32_t off;

	npix = *(ff_u32_t*)*par;
	colour = *(ffly_colour_t*)par[1];
	off = *(ff_u32_t*)par[2];
	ffly_printf("pixfill: %u, %u.%u.%u.%u, %u\n", npix, colour.r, colour.g, colour.b, colour.a, off);
	__ffly_pixfill(npix, colour, off);
}

void static
_pixcopy(struct ffly_grj *__job) {
	void **par = __job->par;

	ff_u32_t x, y, width, height;
	x = *(ff_u32_t*)*par;
	y = *(ff_u32_t*)par[1];
	width = *(ff_u32_t*)par[2];
	height = *(ff_u32_t*)par[3];
	__ffly_pixcopy(x, y, width, height);
}


void static
_pixdraw(struct ffly_grj *__job) {
	void **par = __job->par;

	ff_u32_t x, y;
	void *src;
	ff_u32_t width, height;

	x = *(ff_u32_t*)*par;
	y = *(ff_u32_t*)par[1];
	src = *(void**)par[2];
	width = *(ff_u32_t*)par[3];
	height = *(ff_u32_t*)par[4];
	__ffly_pixdraw(x, y, src, width, height);
}

void static
_tdraw(struct ffly_grj *__job) {
	void **par = __job->par;

	ffly_tilep t;
	ff_u32_t x, y;

	t = *(ffly_tilep*)*par;
	x = *(ff_u32_t*)par[1];
	y = *(ff_u32_t*)par[2];

	__ffly_tdraw(t, x, y);
}

void static
_tri2(struct ffly_grj *__job) {
	void **par = __job->par;

	struct ffly_tri2 *tri;
	ff_u16_t tex;
	ff_u32_t x, y; 

	tri = (struct ffly_tri2*)*par;
	tex = *(ff_u16_t*)par[1];
	x = *(ff_u32_t*)par[2];
	y = *(ff_u32_t*)par[3];

	__ffly_tri2(tri, tex, x, y);
}

void static
_tri3(struct ffly_grj *__job) {
	void **par = __job->par;

	struct ffly_tri3 *tri;
	ff_u16_t tex;
	ff_u32_t x, y, z; 

	tri = (struct ffly_tri3*)*par;
	tex = *(ff_u16_t*)par[1];
	x = *(ff_u32_t*)par[2];
	y = *(ff_u32_t*)par[3];
	z = *(ff_u32_t*)par[4];

	__ffly_tri3(tri, tex, x, y, z);
}

static void(*funcs[])(struct ffly_grj*) = {
	_pixfill,
	_pixcopy,
	_pixdraw,
	_tdraw,
	_tri2,
	_tri3
};

/*
	TBR:
		each job is to contain micro code then shall be run by the graphics command executor?
*/

ff_err_t ffly_grj_prosess(struct ffly_grj *__job) {
	ff_err_t err;
	ffly_fprintf(ffly_log, "about to prosess graphics job.\n");
	funcs[__job->kind](__job);

	job_free(__job);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "job failure.\n");
		return err;
	}

	return FFLY_SUCCESS;
}
