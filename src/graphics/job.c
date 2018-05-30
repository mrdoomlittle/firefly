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

# define parsize3(__t0, __t1, __t2) \
	(sizeof(__t0)+sizeof(__t1)+sizeof(__t2))
# define parsize7(__t0, __t1, __t2, __t3, __t4, __t5, __t6) \
	(sizeof(__t0)+sizeof(__t1)+sizeof(__t2)+sizeof(__t3)+sizeof(__t4)+sizeof(__t5)+sizeof(__t6))

# define put(__dst, __src, __ty) \
	__dst = (__ty)__src
# define inc(__p, __ty) \
	__p+=sizeof(__ty)

/*
	might be worth having its own allocator for parameters memory
*/

struct ffly_grj*
ffly_grj_pixfill(ff_byte_t *__dst, ff_uint_t __npix, ffly_colour_t __colour) {
	ffly_fprintf(ffly_log, "pixfill job.\n");
	ff_u8_t *p = (ff_u8_t*)__ffly_mem_alloc(parsize3(ff_byte_t*, ff_uint_t, ffly_colour_t));
	struct ffly_grj *ret = ffly_grj_mk(_grj_pixfill, p);
	void **par = ret->par;

	ff_byte_t **dst;
	ff_uint_t *npix;
	ffly_colour_t *colour;

	put(dst, p, ff_byte_t**);
	inc(p, ff_byte_t*);
	*(par++) = dst;

	put(npix, p, ff_uint_t*);
	inc(p, ff_uint_t);
	*(par++) = npix;

	put(colour, p, ffly_colour_t*);
	*par = colour;

	*dst = __dst;
	*npix = __npix;
	*colour = __colour;
	return ret;
}

struct ffly_grj*
ffly_grj_pixdraw(ff_uint_t __x, ff_uint_t __y, ff_byte_t *__dst, ff_uint_t __dstw,
	ff_byte_t *__pixels, ff_uint_t __width, ff_uint_t __height)
{
	ffly_fprintf(ffly_log, "pixdraw job.\n");
	ff_u8_t *p = (ff_u8_t*)__ffly_mem_alloc(parsize7(ff_uint_t, ff_uint_t, ff_byte_t*, ff_uint_t, ff_byte_t*, ff_uint_t, ff_uint_t));
	struct ffly_grj *ret = ffly_grj_mk(_grj_pixdraw, p);
	void **par = ret->par;

	ff_uint_t *x;
	ff_uint_t *y;
	ff_byte_t **dst;
	ff_uint_t *dstw;
	ff_byte_t **pixels;
	ff_uint_t *width;
	ff_uint_t *height;

	put(x, p, ff_uint_t*);
	inc(p, ff_uint_t);
	*(par++) = x;

	put(y, p, ff_uint_t*);
	inc(p, ff_uint_t);
	*(par++) = y;

	put(dst, p, ff_byte_t**);
	inc(p, ff_byte_t*);
	*(par++) = dst;

	put(dstw, p, ff_uint_t*);
	inc(p, ff_uint_t);
	*(par++) = dstw;

	put(pixels, p, ff_byte_t**);
	inc(p, ff_byte_t*);
	*(par++) = pixels;

	put(width, p, ff_uint_t*);
	inc(p, ff_uint_t);
	*(par++) = width;

	put(height, p, ff_uint_t*);
	*par = height;

	*x = __x;
	*y = __y;
	*dst = __dst;
	*dstw = __dstw;
	*pixels = __pixels;
	*width = __width;
	*height = __height;
	return ret;
}

struct ffly_grj*
ffly_grj_pixcopy(ff_byte_t *__dst, ff_byte_t *__src, ff_uint_t __npix) {
	ffly_fprintf(ffly_log, "pixcopy job.\n");
	ff_u8_t *p = (ff_u8_t*)__ffly_mem_alloc(parsize3(ff_byte_t*, ff_byte_t*, ff_uint_t));
	struct ffly_grj *ret = ffly_grj_mk(_grj_pixcopy, p);
	void **par = ret->par;

	ff_byte_t **dst;
	ff_byte_t **src;
	ff_uint_t *npix;

	put(dst, p, ff_byte_t**);
	inc(p, ff_byte_t*);
	*(par++) = dst;

	put(src, p, ff_byte_t**);
	inc(p, ff_byte_t*);
	*(par++) = src;

	put(npix, p, ff_uint_t*);
	*par = npix;

	*dst = __dst;
	*src = __src;
	*npix = __npix;
	return ret;
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
ff_err_t ffly_grj_prosess(struct ffly_grj *__job) {
	ff_err_t err;
	ffly_fprintf(ffly_log, "about to prosess graphics job.\n");
	switch(__job->kind) {
		case _grj_pixfill: {
			void **par = __job->par;
			ff_byte_t *dst;
			ff_uint_t npix;
			ffly_colour_t colour;

			dst = *(ff_byte_t**)*(par++);
			npix = *(ff_uint_t*)*(par++);
			colour = *(ffly_colour_t*)*par;
	
			if (_err(err = __ffly_pixfill(dst, npix, colour))) {
				ffly_fprintf(ffly_err, "failed to fill with pixels.\n");
			}
			ffly_fprintf(ffly_log, "graphics job-%s, %p %u, %u-%u-%u-%u\n", kstr(__job->kind), dst, npix, colour.r, colour.g, colour.b, colour.a);
			break;
		}
		case _grj_pixcopy: {
			void **par = __job->par;
			ff_byte_t *dst, *src;
			ff_uint_t npix;

			dst = *(ff_byte_t**)*(par++);
			src = *(ff_byte_t**)*(par++);
			npix = *(ff_uint_t*)*par;

			if (_err(err = __ffly_pixcopy(dst, src, npix))) {
				ffly_fprintf(ffly_err, "failed to copy pixels.\n");
			}
			ffly_fprintf(ffly_log, "graphics job-%s, %p, %p, %u\n", kstr(__job->kind), dst, src, npix);
			break;
		}
		case _grj_pixdraw: {
			void **par = __job->par;
			ff_uint_t x, y;
			ff_byte_t *dst;
			ff_uint_t dstw;
			ff_byte_t *pixels;
			ff_uint_t width, height;

			x = *(ff_uint_t*)*(par++);
			y = *(ff_uint_t*)*(par++);
	
			dst = *(ff_byte_t**)*(par++);
			dstw = *(ff_uint_t*)*(par++);
			pixels = *(ff_byte_t**)*(par++);
			width = *(ff_uint_t*)*(par++);
			height = *(ff_uint_t*)*par;

			if (_err(err = __ffly_pixdraw(x, y, dst, dstw, pixels, width, height))) {
				ffly_fprintf(ffly_err, "failed to draw pixels.\n");
			}
			ffly_fprintf(ffly_log, "graphics job-%s, %u, %u, %p, %u, %p, %u, %u\n", kstr(__job->kind), x, y, dst, dstw, pixels, width, height);
			break;
		}
		default:
			ffly_fprintf(ffly_err, "graphics, job kind not recognised.\n");
			err = FFLY_FAILURE;
	}

	job_free(__job);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "job failure.\n");
		return err;
	}

	return FFLY_SUCCESS;
}
