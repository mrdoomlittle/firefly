# include "tri.h"
# include "tex.h"
# include "context.h"
ff_u16_t static
_bron_tri2(struct bron_tri2 *__tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y) {
	ff_u16_t tri;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;

	ctx->driver.tri2(__tri, tri = ctx->stack);
	ctx->driver.raster_tri2(tri, __tex, __x, __y);

	ctx->stack+=ctx->driver.tri2_struc_sz;
	return tri;
}

void static
_bron_tri3(struct bron_tri3 *__tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	ff_u16_t tri;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	ctx->driver.tri3(__tri, tri = ctx->stack);
	ctx->driver.raster_tri3(tri, __tex, __x, __y, __z);
	ctx->stack+=ctx->driver.tri3_struc_sz;
}

ff_u16_t ffly_bron_tri2(struct bron_tri2 *__tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y) {
	return _bron_tri2(__tri, __tex, __x, __y);
}

# include "depth.h"
# include "../system/io.h"
# include "../maths/floor.h"
void bron_tri3(struct bron_tri3 *__tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	float x0, x1, x2;
	float y0, y1, y2;

	struct bron_context *ctx;
	ctx = BRON_CONTEXT;

	float zdis;
	zdis = __z-ctx->ez;

	// going to cause error for 1./0
	// should just time by x or something
	float m, j;
	m = 1./zdis;

	j = 1./ctx->fov;

	ffly_printf("--------->: %u\n", (ff_u32_t)(zdis));
	float z, a;
	z = __z;


	x0 = ((float)__tri->x0)*j;
	x1 = ((float)__tri->x1)*j;
	x2 = ((float)__tri->x2)*j;

	y0 = ((float)__tri->y0)*j;
	y1 = ((float)__tri->y1)*j;
	y2 = ((float)__tri->y2)*j;

	x0 = x0*ctx->a;
	y0 = y0*ctx->a;

	x1 = x1*ctx->b;
	y1 = y1*ctx->b;

	x2 = x2*ctx->c;
	y2 = y2*ctx->c;
	

	float ex, ey;
	ex = ctx->ex;
	ey = ctx->ey;

	ffly_printf("----------> 0 %d, %d\n", (ff_s32_t)ex, (ff_s32_t)ey);
	__x = ex+((((float)__x)-ex)*j);
	__y = ey+((((float)__y)-ey)*j);

	struct bron_tri3 tri = {
		(ff_i32_t)x0, (ff_i32_t)y0, __tri->z0,
		(ff_i32_t)x1, (ff_i32_t)y1, __tri->z1,
		(ff_i32_t)x2, (ff_i32_t)y2, __tri->z2
	};


	ffly_printf("tri: %d, %d, %d, %d, %d, %d, %d\n", tri.x0, tri.y0, tri.x1, tri.y1, tri.x2, tri.y2, (ff_uint_t)zdis);
	_bron_tri3(&tri, __tex, __x, __y, __z);
}
