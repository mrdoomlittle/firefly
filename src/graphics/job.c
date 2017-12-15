# include "job.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
# include "../system//errno.h"
char const static *kstr(mdl_u8_t __kind) {
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

struct ffly_grj* ffly_grj_mk(mdl_u8_t __kind, void *__par) {
	struct ffly_grj *job = (struct ffly_grj*)__ffly_mem_alloc(sizeof(struct ffly_grj));
	if (job != NULL) {
		*job = (struct ffly_grj) {
			.kind = __kind,
			.par = __par
		};
	}
	return job;
}

# include "../types/colour_t.h"
# include "fill.h"
# include "copy.h"
# include "draw.h"
# include "../types/byte_t.h"
# include "../types/off_t.h"
ffly_err_t ffly_grj_prosess(struct ffly_grj *__job) {
	ffly_err_t err;
	ffly_fprintf(ffly_log, "about to prosess graphics job.\n");
	switch(__job->kind) {
		case _grj_pixfill: {
			mdl_u8_t *p = (mdl_u8_t*)__job->par;
			ffly_byte_t *buf = *(ffly_byte_t**)p;
			p+= sizeof(ffly_byte_t**);

			mdl_uint_t nopix = *(mdl_uint_t*)p;
			p+= sizeof(mdl_uint_t);

			ffly_colour_t colour = *(ffly_colour_t*)p;

			err = ffly_pixfill(buf, nopix, colour);
			ffly_fprintf(ffly_log, "graphics job-%s, %p %u, %u-%u-%u-%u\n", kstr(__job->kind), buf, nopix, colour.r, colour.g, colour.b, colour.a);
			break;
		}
		case _grj_pixcopy: {
			mdl_u8_t *p = (mdl_u8_t*)__job->par;
			ffly_byte_t *dst, *src;

			dst = *(ffly_byte_t**)p;
			p+= sizeof(ffly_byte_t*);

			src = *(ffly_byte_t**)p;
			p+= sizeof(ffly_byte_t*);

			mdl_uint_t nopix = *(mdl_uint_t*)p;

			err = ffly_pixcopy(dst, src, nopix);
			ffly_fprintf(ffly_log, "graphics job-%s, %p, %p, %u\n", kstr(__job->kind), dst, src, nopix);
			break;
		}
		case _grj_pixdraw: {
			mdl_u8_t *p = (mdl_u8_t*)__job->par;
			ffly_off_t x, y;

			x = *(ffly_off_t*)p;
			p+= sizeof(ffly_off_t);

			y = *(ffly_off_t*)p;
			p+= sizeof(ffly_off_t);

			ffly_byte_t *buff = *(ffly_byte_t**)p;
			p+= sizeof(ffly_byte_t*);

			mdl_uint_t bufw = *(mdl_uint_t*)p;
			p+= sizeof(mdl_uint_t);

			ffly_byte_t *pixels = *(ffly_byte_t**)p;
			p+= sizeof(ffly_byte_t*);

			mdl_uint_t width = *(mdl_uint_t*)p;
			p+= sizeof(mdl_uint_t);

			mdl_uint_t height = *(mdl_uint_t*)p;

			err = ffly_pixdraw(x, y, buff, bufw, pixels, width, height);
			ffly_fprintf(ffly_log, "graphics job-%s, %u, %u, %p, %u, %p, %u, %u\n", kstr(__job->kind), x, y, buff, bufw, pixels, width, height);
			break;
		}
		default:
			ffly_fprintf(ffly_err, "graphics, job kind not recognised.\n");
			__ffly_mem_free(__job);
			return FFLY_FAILURE;
	}
	__ffly_mem_free(__job);
	return FFLY_SUCCESS;
}
