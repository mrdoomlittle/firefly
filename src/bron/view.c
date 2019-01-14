# include "view.h"
# include "context.h"
void bron_viewport(struct bron_viewport *__vp) {
	BRON_CONTEXT->depth = __vp->width;
	BRON_CONTEXT->driver.viewport(__vp);
}

# include "../system/io.h"
# include "../maths/floor.h"
void bron_lookat(ff_u32_t __x, ff_u32_t __y, ff_u32_t __z, float __a, float __b, float __c) {
	struct bron_context *ct;
	ct = BRON_CONTEXT;
	ct->ex = __x;
	ct->ey = __y;
	ct->ez = __z;
/*
	float g0, g1, g2, g3;
	g0 = a+(-b);
	g2 = c;

	g1 = -c;
	g3 = (-a)+b;
*/
	float dis;
	dis = __a/(ct->depth*0.5);


	ct->a = ffly_floor(dis);
	ffly_printf("lookat: %u, depth: %u\n", (ff_s32_t)ct->a, (ff_s32_t)ct->depth);
}
