# include "../../ffint.h"
# include "../../nought/raise.h"
# include "../../bron/prim.h"
# include "../../nought/types.h"
# include "../../nought/context.h"
# include "../../hexdump.h"
# include "../../system/io.h"
# include "../../bron/view.h"
static ff_u8_t cb[4048];
static ff_u8_t *cb_p = cb;

void static
nt_done(void) {
	nt_raise(cb, cb_p-cb);
#ifdef DEBUG
	ffly_hexdump(cb, cb_p-cb);
#endif
	cb_p = cb;
}

void static
_nt_ctx_new(ff_u16_t __p) {
	*cb_p = _nt_op_ctx_new;
	*(ff_u16_t*)(cb_p+1) = __p;
	cb_p+=3;
}

void static
_nt_ctx_destroy(ff_u16_t __p) {
	*cb_p = _nt_op_ctx_destroy;
	*(ff_u16_t*)(cb_p+1) = __p;
	cb_p+=3;
}

void static
_nt_raster_tri2(ff_u16_t __tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y) {
	*cb_p = _nt_op_raster_tri2;
	*(ff_u16_t*)(cb_p+1) = __tri;
	*(ff_u32_t*)(cb_p+3) = __tex;
	*(ff_u32_t*)(cb_p+7) = __x;
	*(ff_u32_t*)(cb_p+11) = __y;
	cb_p+=15;
}

void static
_nt_raster_tri3(ff_u16_t __tri, ff_u32_t __tex, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	*cb_p = _nt_op_raster_tri3;
	*(ff_u16_t*)(cb_p+1) = __tri;
	*(ff_u32_t*)(cb_p+3) = __tex;
	*(ff_u32_t*)(cb_p+7) = __x;
	*(ff_u32_t*)(cb_p+11) = __y;
	*(ff_u32_t*)(cb_p+15) = __z;
	cb_p+=19;
}

# include "../../dep/mem_cpy.h"
#define nt_stack(__at) \
	(nt_raise_stack+(__at))

void static
_nt_sput(void *__buf, ff_u32_t __size, ff_u16_t __adr) {
	*cb_p = _nt_op_sput;
	*(void**)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(ff_u16_t*)(cb_p+13) = __adr;
	cb_p+=15;
}

void static
_nt_sget(void *__buf, ff_u32_t __size, ff_u16_t __adr) {
	*cb_p = _nt_op_sget;
	*(void**)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(ff_u16_t*)(cb_p+13) = __adr;
	cb_p+=15;
}

void static
nt_tri2(struct bron_tri2 *__tri, ff_u16_t __dst) {
	struct nt_tri2 *tri;

	tri = (struct nt_tri2*)nt_stack(__dst);
	tri->v0.x = __tri->x0;
	tri->v0.y = __tri->y0;

	tri->v1.x = __tri->x1;
	tri->v1.y = __tri->y1;

	tri->v2.x = __tri->x2;
	tri->v2.y = __tri->y2;
}

void static
nt_tri3(struct bron_tri3 *__tri, ff_u16_t __dst) {
	*(struct nt_tri3*)nt_stack(__dst) = *(struct nt_tri3*)__tri;
}

void static
_nt_putframe(ff_u8_t *__dst, ff_u32_t __x, ff_u32_t __y, ff_u32_t __width, ff_u32_t __height) {
	*cb_p = _nt_op_putframe;
	*(ff_u8_t**)(cb_p+1) = __dst;
	cb_p+=sizeof(ff_u8_t*)+1;

	*(ff_u32_t*)cb_p = __x;
	*(ff_u32_t*)(cb_p+4) = __y;
	*(ff_u32_t*)(cb_p+8) = __width;
	*(ff_u32_t*)(cb_p+12) = __height;
	cb_p+=16;
}

void static
_nt_setctx(ff_u16_t __ctx) {
	*cb_p = _nt_op_setctx;
	*(ff_u16_t*)(cb_p+1) = __ctx;
	cb_p+=3;
}

void static
_nt_start(void) {
	*(cb_p++) = _nt_op_start;
}

void static
_nt_finish(void) {
	*(cb_p++) = _nt_op_finish;
}

void static
_nt_pixdraw(ff_u32_t __x, ff_u32_t __y, ff_u8_t *__pixels, ff_u32_t __width, ff_u32_t __height) {
	*cb_p = _nt_op_pixdraw;
	*(ff_u32_t*)(cb_p+1) = __x; 
	*(ff_u32_t*)(cb_p+5) = __y;

	*(ff_u8_t**)(cb_p+9) = __pixels;
	*(ff_u32_t*)(cb_p+17) = __width;
	*(ff_u32_t*)(cb_p+21) = __height;
	cb_p+=25;
}

void static
_nt_pixfill(ff_u32_t __npix, ff_u8_t *__colour) {
	*cb_p = _nt_op_pixfill;
	*(ff_u32_t*)(cb_p+1) = __npix;
	*(ff_u8_t**)(cb_p+5) = __colour;
	cb_p+=13;
}

void static
_nt_fb_set(ff_u32_t __fb) {
	*cb_p = _nt_op_fb_set;
	*(ff_u32_t*)(cb_p+1) = __fb;
	cb_p+=5;
}

void static
_nt_fb_new(ff_u32_t __width, ff_u32_t __height, ff_u32_t __fb) {
	*cb_p = _nt_op_fb_new;
	*(ff_u32_t*)(cb_p+1) = __width;
	*(ff_u32_t*)(cb_p+5) = __height;
	*(ff_u32_t*)(cb_p+9) = __fb;
	cb_p+=13;
}

void static
_nt_fb_destroy(ff_u32_t __fb) {
	*cb_p = _nt_op_fb_destroy;
	*(ff_u32_t*)(cb_p+1) = __fb;
	cb_p+=5;
}

void static
_nt_rb_bind(ff_u32_t __rb, ff_u32_t __fb) {
	*cb_p = _nt_op_rb_bind;
	*(ff_u32_t*)(cb_p+1) = __rb;
	*(ff_u32_t*)(cb_p+5) = __fb;
	cb_p+=9;
}

void static
_nt_rb_new(ff_u32_t __width, ff_u32_t __height, ff_u32_t __rb) {
	*cb_p = _nt_op_rb_new;
	*(ff_u32_t*)(cb_p+1) = __width;
	*(ff_u32_t*)(cb_p+5) = __height;
	*(ff_u32_t*)(cb_p+9) = __rb;
	cb_p+=13;
}

void static
_nt_rb_destroy(ff_u32_t __rb) {
	*cb_p = _nt_op_rb_destroy;
	*(ff_u32_t*)(cb_p+1) = __rb;
	cb_p+=5;
}

void static
_nt_ptile_new(ff_u32_t __pt, void(*__get)(ff_u8_t, long long, void*), void *__tile) {
	*cb_p = _nt_op_ptile_new;
	*(ff_u32_t*)(cb_p+1) = __pt;
	*(void**)(cb_p+5) = (void*)__get;
	*(void**)(cb_p+13) = __tile;
	cb_p+=21;
}

void static
_nt_ptile_destroy(ff_u32_t __pt) {
	*cb_p = _nt_op_ptile_destroy;
	*(ff_u32_t*)(cb_p+1) = __pt;
	cb_p+=5;
}

void static
_nt_tdraw(ff_u32_t __tile, ff_u32_t __x, ff_u32_t __y) {
	*cb_p = _nt_op_tdraw;
	*(ff_u32_t*)(cb_p+1) = __tile;
	*(ff_u32_t*)(cb_p+5) = __x;
	*(ff_u32_t*)(cb_p+9) = __y;
	cb_p+=13;
}

void static
_nt_sb(ff_u8_t __bits) {
	*cb_p = _nt_op_sb;
	*(cb_p+1) = __bits;
	cb_p+=2;
}

void static
_nt_cb(ff_u8_t __bits) {
	*cb_p = _nt_op_cb;
	*(cb_p+1) = __bits;
	cb_p+=2;
}

void static
_nt_objbuf_new(ff_u32_t __buf, ff_u32_t __size) {
	*cb_p = _nt_op_obn;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __size;
	cb_p+=9;
}

void static
_nt_objbuf_destroy(ff_u32_t __buf) {
	*cb_p = _nt_op_obd;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_objbuf_map(ff_u32_t __buf) {
	*cb_p = _nt_op_obm;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_objbuf_unmap(ff_u32_t __buf) {
	*cb_p = _nt_op_obum;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_objbuf_write(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	*cb_p = _nt_op_obw;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __offset;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(void**)(cb_p+13) = __src;
	cb_p+=21;
}

void static
_nt_objbuf_read(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	*cb_p = _nt_op_obr;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __offset;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(void**)(cb_p+13) = __dst;
	cb_p+=21;
}

void static
_nt_draw(ff_u32_t __buf, ff_u32_t __n) {
	*cb_p = _nt_op_draw;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __n;
	cb_p+=9;
}

void static
_nt_texbuf_new(ff_u32_t __buf, ff_u32_t __size) {
	ffly_printf("texbuf new: %u\n", _nt_op_txbn);
	*cb_p = _nt_op_txbn;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __size;
	cb_p+=9;
}

void static
_nt_texbuf_destroy(ff_u32_t __buf) {
	*cb_p = _nt_op_txbd;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_texbuf_map(ff_u32_t __buf) {
	*cb_p = _nt_op_txbm;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_texbuf_unmap(ff_u32_t __buf) {
	*cb_p = _nt_op_txbum;
	*(ff_u32_t*)(cb_p+1) = __buf;
	cb_p+=5;
}

void static
_nt_texbuf_write(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	*cb_p = _nt_op_txbw;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __offset;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(void**)(cb_p+13) = __src;
	cb_p+=21;
}

void static
_nt_texbuf_read(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	*cb_p = _nt_op_txbr;
	*(ff_u32_t*)(cb_p+1) = __buf;
	*(ff_u32_t*)(cb_p+5) = __offset;
	*(ff_u32_t*)(cb_p+9) = __size;
	*(void**)(cb_p+13) = __dst;
	cb_p+=21;
}

void static
_nt_tex_new(ff_u32_t __tx, ff_u32_t __txb, ff_u32_t __width, ff_u32_t __height) {
	*cb_p = _nt_op_txn;
	*(ff_u32_t*)(cb_p+1) = __tx;
	*(ff_u32_t*)(cb_p+5) = __txb;
	*(ff_u32_t*)(cb_p+9) = __width;
	*(ff_u32_t*)(cb_p+13) = __height;
	cb_p+=17;
}

void static
_nt_tex_destroy(ff_u32_t __tx) {
	*cb_p = _nt_op_txd;
	*(ff_u32_t*)(cb_p+1) = __tx;
	cb_p+=5;
}

void _nt_dpbuf_write(ff_u32_t __size, ff_u32_t __offset, void *__src) {
	*cb_p = _nt_op_dpbw;
	*(ff_u32_t*)(cb_p+1) = __size;
	*(ff_u32_t*)(cb_p+5) = __offset;
	*(void**)(cb_p+9) = __src;
	cb_p+=17;
}

void static
_nt_viewport(struct bron_viewport *__vp) {
	*cb_p = _nt_op_vp;
	struct nt_viewport *vp;
	vp = (struct nt_viewport*)(cb_p+1);
	vp->width = __vp->width;
	vp->height = __vp->height;
	vp->x = __vp->x;
	vp->y = __vp->y;

	cb_p+=1+sizeof(struct nt_viewport);
}

void static
_nt_slg_prog(ff_u8_t *__p, ff_u32_t __size) {
	*cb_p = _nt_op_slagprog;
	*(ff_u8_t**)(cb_p+1) = __p;
	*(ff_u32_t*)(cb_p+9) = __size;
	cb_p+=13;
}

# include "../../nought/tex.h"
# include "../../bron/driver.h"
void static
_nt_info(ff_u8_t __what, long long __arg) {
	switch(__what) {
		case BI_CTX_ST_SZ:
			*(ff_u32_t*)__arg = sizeof(struct nt_context);
		break;
		case BI_TRI2_ST_SZ:
			*(ff_u32_t*)__arg = sizeof(struct nt_tri2);
		break;
		case BI_TEX_ST_SZ:
			*(ff_u32_t*)__arg = sizeof(struct nt_tex);
		break;
		case BI_VTX2_ST_SZ:
			*(ff_u32_t*)__arg = sizeof(struct nt_vertex2);
		break;
	}
}

/*
	TODO:
		copy struct to struct
*/
void ffly_nought(struct bron_driver *__driver) {
	__driver->sb = _nt_sb;
	__driver->cb = _nt_cb;
	__driver->tdraw = _nt_tdraw;
	__driver->ptile_new = _nt_ptile_new;
	__driver->ptile_destroy = _nt_ptile_destroy;
	__driver->fb_set = _nt_fb_set;
	__driver->fb_new = _nt_fb_new;
	__driver->fb_destroy = _nt_fb_destroy;
	__driver->rb_bind = _nt_rb_bind;
	__driver->rb_new = _nt_rb_new;
	__driver->rb_destroy = _nt_rb_destroy;
	__driver->pixcopy = (void*)0;
	__driver->pixdraw = _nt_pixdraw;
	__driver->pixfill = _nt_pixfill;
	__driver->sput = _nt_sput;
	__driver->sget = _nt_sget;
	__driver->ctx_new = _nt_ctx_new;
	__driver->ctx_destroy = _nt_ctx_destroy;
	__driver->raster_tri2 = _nt_raster_tri2;
	__driver->raster_tri3 = _nt_raster_tri3;
	__driver->tri2 = nt_tri2;
	__driver->tri3 = nt_tri3;
	__driver->done = nt_done;
	__driver->ctx_struc_sz = sizeof(struct nt_context);
	__driver->tri2_struc_sz = sizeof(struct nt_tri2);
	__driver->tri3_struc_sz = sizeof(struct nt_tri3);
	__driver->tex_struc_sz = sizeof(struct nt_tex);
	__driver->frame = _nt_putframe;
	__driver->setctx = _nt_setctx;
	__driver->start = _nt_start;
	__driver->finish = _nt_finish;
	__driver->objbuf_new = _nt_objbuf_new;
	__driver->objbuf_destroy = _nt_objbuf_destroy;
	__driver->objbuf_map = _nt_objbuf_map;
	__driver->objbuf_unmap = _nt_objbuf_unmap;
	__driver->objbuf_write = _nt_objbuf_write;
	__driver->objbuf_read = _nt_objbuf_read;
	__driver->draw = _nt_draw;
	__driver->texbuf_new = _nt_texbuf_new;
	__driver->texbuf_destroy = _nt_texbuf_destroy;
	__driver->texbuf_map = _nt_texbuf_map;
	__driver->texbuf_unmap = _nt_texbuf_unmap;
	__driver->texbuf_write = _nt_texbuf_write;
	__driver->texbuf_read = _nt_texbuf_read;
	__driver->tex_new = _nt_tex_new;
	__driver->tex_destroy = _nt_tex_destroy;
	__driver->viewport = _nt_viewport;
	__driver->dpbuf_write = _nt_dpbuf_write;
	__driver->slgprog = _nt_slg_prog;
}
