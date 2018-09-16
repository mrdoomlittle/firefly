# include "../../ffint.h"
# include "../../sr/raise.h"
# include "../../prim.h"
# include "../../sr/types.h"
# include "../../sr/context.h"
static ff_u8_t cb[2048];
static ff_u8_t *cb_p = cb;

void static
sr_done(void) {
	sr_raise(cb, cb_p-cb);
	cb_p = cb;
}

void static
_sr_ctx_new(ff_u16_t __p) {
	*cb_p = sr_op_ctx_new;
	*(ff_u16_t*)(cb_p+1) = __p;
	cb_p+=3;
}

void static
_sr_ctx_destroy(ff_u16_t __p) {
	*cb_p = sr_op_ctx_destroy;
	*(ff_u16_t*)(cb_p+1) = __p;
	cb_p+=3;
}

void static
_sr_raster_tri2(ff_u16_t __tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	*cb_p = sr_op_raster_tri2;
	*(ff_u16_t*)(cb_p+1) = __tri;
	*(ff_u16_t*)(cb_p+3) = __tex;
	*(ff_u32_t*)(cb_p+5) = __x;
	*(ff_u32_t*)(cb_p+9) = __y;
	cb_p+=13;
}

# include "../../dep/mem_cpy.h"
# define sr_stack(__at) \
	(sr_raise_stack+(__at))

void static
_sr_sput(void *__buf, ff_u32_t __size, ff_u16_t __adr) {
	*cb_p = sr_op_sput;
	*(void**)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(ff_u16_t*)(cb_p+13) = __adr;
	cb_p+=15;
}

void static
_sr_sget(void *__buf, ff_u32_t __size, ff_u16_t __adr) {
	*cb_p = sr_op_sget;
	*(void**)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(ff_u16_t*)(cb_p+13) = __adr;
	cb_p+=15;
}

void static
sr_tri2(struct ffly_tri2 *__tri, ff_u16_t __dst) {
	struct sr_tri2 *tri;

	tri = (struct sr_tri2*)sr_stack(__dst);
	tri->v0.x = __tri->v0.x;
	tri->v0.y = __tri->v0.y;

	tri->v1.x = __tri->v1.x;
	tri->v1.y = __tri->v1.y;

	tri->v2.x = __tri->v2.x;
	tri->v2.y = __tri->v2.y;
}

void static
sr_tex(struct ffly_tex *__tex, ff_u16_t __dst) {
	struct sr_tex *tx;
	tx = (struct sr_tex*)sr_stack(__dst);
	*(ff_u32_t*)tx->inn = *(ff_u32_t*)__tex->inn;
}

void static
_sr_putframe(ff_u8_t *__dst, ff_u32_t __x, ff_u32_t __y, ff_u32_t __width, ff_u32_t __height) {
	*cb_p = sr_op_putframe;
	*(ff_u8_t**)(cb_p+1) = __dst;
	cb_p+=sizeof(ff_u8_t*)+1;

	*(ff_u32_t*)cb_p = __x;
	*(ff_u32_t*)(cb_p+4) = __y;
	*(ff_u32_t*)(cb_p+8) = __width;
	*(ff_u32_t*)(cb_p+12) = __height;
	cb_p+=16;
}

void static
_sr_setctx(ff_u16_t __ctx) {
	*cb_p = sr_op_setctx;
	*(ff_u16_t*)(cb_p+1) = __ctx;
	cb_p+=3;
}

void static
_sr_start(void) {
	*(cb_p++) = sr_op_start;
}

void static
_sr_finish(void) {
	*(cb_p++) = sr_op_finish;
}

void static
_sr_pixdraw(ff_u32_t __x, ff_u32_t __y, ff_u8_t *__pixels, ff_u32_t __width, ff_u32_t __height) {
	*cb_p = sr_op_pixdraw;
	*(ff_u32_t*)(cb_p+1) = __x; 
	*(ff_u32_t*)(cb_p+5) = __y;

	*(ff_u8_t**)(cb_p+9) = __pixels;
	*(ff_u32_t*)(cb_p+17) = __width;
	*(ff_u32_t*)(cb_p+21) = __height;
	cb_p+=24;
}

void static
_sr_pixfill(ff_u32_t __npix, ff_u8_t *__colour) {
	*cb_p = sr_op_pixfill;
	*(ff_u32_t*)(cb_p+1) = __npix;
	*(ff_u8_t**)(cb_p+5) = __colour;
	cb_p+=13;
}

void static
_sr_fb_set(ff_u16_t __fb) {
	*cb_p = sr_op_fb_set;
	*(ff_u16_t*)(cb_p+1) = __fb;
	cb_p+=3;
}

void static
_sr_fb_new(ff_u32_t __width, ff_u32_t __height, ff_u16_t __fb) {
	*cb_p = sr_op_fb_new;
	*(ff_u32_t*)(cb_p+1) = __width;
	*(ff_u32_t*)(cb_p+5) = __height;
	*(ff_u16_t*)(cb_p+9) = __fb;
	cb_p+=11;
}

void static
_sr_fb_destroy(ff_u16_t __fb) {
	*cb_p = sr_op_fb_destroy;
	*(ff_u16_t*)(cb_p+1) = __fb;
	cb_p+=3;
}

void static
_sr_ptile_new(ff_u16_t __pt, void(*__get)(ff_u8_t, long long, void*), void *__tile) {
	*cb_p = sr_op_ptile_new;
	*(ff_u16_t*)(cb_p+1) = __pt;
	*(void**)(cb_p+3) = (void*)__get;
	*(void**)(cb_p+11) = __tile;
	cb_p+=19;
}

void static
_sr_ptile_destroy(ff_u16_t __pt) {
	*cb_p = sr_op_ptile_destroy;
	*(ff_u16_t*)(cb_p+1) = __pt;
	cb_p+=3;
}

void static
_sr_tdraw(ff_u16_t __tile, ff_u32_t __x, ff_u32_t __y) {
	*cb_p = sr_op_tdraw;
	*(ff_u16_t*)(cb_p+1) = __tile;
	*(ff_u32_t*)(cb_p+3) = __x;
	*(ff_u32_t*)(cb_p+7) = __y;
	cb_p+=11;
}

# include "../../driver.h"
void ffly_sr(struct ff_driver *__driver) {
	__driver->tdraw = _sr_tdraw;
	__driver->ptile_new = _sr_ptile_new;
	__driver->ptile_destroy = _sr_ptile_destroy;
	__driver->fb_set = _sr_fb_set;
	__driver->fb_new = _sr_fb_new;
	__driver->fb_destroy = _sr_fb_destroy;
	__driver->pixcopy = (void*)0;
	__driver->pixdraw = _sr_pixdraw;
	__driver->pixfill = _sr_pixfill;
	__driver->sput = _sr_sput;
	__driver->sget = _sr_sget;
	__driver->ctx_new = _sr_ctx_new;
	__driver->ctx_destroy = _sr_ctx_destroy;
	__driver->raster_tri2 = _sr_raster_tri2;
	__driver->tri2 = sr_tri2;
	__driver->tex = sr_tex;
	__driver->done = sr_done;
	__driver->ctx_struc_sz = sizeof(struct sr_context);
	__driver->tri2_struc_sz = sizeof(struct sr_tri2);
	__driver->tex_struc_sz = sizeof(struct sr_tex);
	__driver->frame = _sr_putframe;
	__driver->setctx = _sr_setctx;
	__driver->start = _sr_start;
	__driver->finish = _sr_finish;
}
