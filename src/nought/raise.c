# include "raise.h"
# include "raster.h"
# include "context.h"
# include "objbuf.h"
# include "framebuff.h"
# include "../system/io.h"
# include "shit.h"
# include "../dep/mem_cpy.h"
# include "tex.h"
ff_u8_t *nt_raise_p;
ff_u8_t nt_raise_stack[STACK_SIZE];
ff_u16_t nt_raise_sp;
#define MAX 35
void static
nt_sput(void) {
	void *buf;
	ff_u32_t size;
	ff_u16_t dst;

	buf = *(void**)nt_raise_p;
	size = *(ff_u32_t*)(nt_raise_p+8);
	dst = *(ff_u16_t*)(nt_raise_p+12);

	ffly_mem_cpy(nt_raise_stack+dst, buf, size);
}

void static
nt_sget(void) {
	void *buf;
	ff_u32_t size;
	ff_u16_t ntc;

	buf = *(void**)nt_raise_p;
	size = *(ff_u32_t*)(nt_raise_p+8);
	ntc = *(ff_u16_t*)(nt_raise_p+12);

	ffly_mem_cpy(buf, nt_raise_stack+ntc, size);
}

static void(*op[])(void) = {
	nt_raster_tri2,
	nt_ctx_new,
	nt_ctx_destroy,
	nt_putframe,
	nt_setctx,
	nt_start,
	nt_finish,
	nt_pixcopy,
	nt_pixdraw,
	nt_pixfill,
	nt_fb_set,
	nt_fb_new,
	nt_fb_destroy,
	nt_ptile_new,
	nt_ptile_destroy,
	nt_tdraw,
	nt_sput,
	nt_sget,
	nt_sb,
	nt_cb,
	nt_objbuf_new,
	nt_objbuf_destroy,
	nt_objbuf_map,
	nt_objbuf_unmap,
	nt_objbuf_write,
	nt_objbuf_read,
	nt_draw,
	nt_texbuf_new,
	nt_texbuf_destroy,
	nt_texbuf_map,
	nt_texbuf_unmap,
	nt_texbuf_write,
	nt_texbuf_read,
	nt_tex_new,
	nt_tex_destroy

};

ff_uint_t static os[] = {
	(sizeof(ff_u16_t)*2)+(sizeof(ff_u32_t)*2),			//nt_raster_tri2
	sizeof(ff_u16_t),									//nt_ctx_new
	sizeof(ff_u16_t),									//nt_ctx_destroy
	sizeof(ff_u8_t*)+(sizeof(ff_u32_t)*4),				//nt_putframe
	sizeof(ff_u16_t),									//nt_setctx
	0,													//nt_start
	0,													//nt_finish
	0,													//nt_pixcopy
	(sizeof(ff_u32_t)*4)+sizeof(ff_u8_t*),				//nt_pixdraw
	sizeof(ff_u8_t*)+sizeof(ff_u32_t),					//nt_pixfill
	sizeof(ff_u16_t),									//nt_fb_set
	sizeof(ff_u16_t)+(sizeof(ff_u32_t)*2),				//nt_fb_new
	sizeof(ff_u16_t),									//nt_fb_destroy
	sizeof(ff_u16_t)+(sizeof(void*)*2),					//nt_ptile_new
	sizeof(ff_u16_t),									//nt_ptile_destroy
	sizeof(ff_u16_t)+(sizeof(ff_u32_t)*2),				//nt_tdraw
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t),	//nt_sput
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t),	//nt_sget
	1,													//nt_sb
	1,													//nt_cb
	6,													//nt_objbuf_new
	2,													//nt_objbuf_destroy
	2,													//nt_objbuf_map
	2,													//nt_objbuf_unmap
	18,													//nt_objbuf_write
	18,													//nt_objbuf_read
	6,													//nt_draw
	6,													//nt_texbuf_new
	2,													//nt_texbuf_destroy
	2,													//nt_texbuf_map
	2,													//nt_texbuf_unmap
	18,													//nt_texbuf_write
	18,													//nt_texbuf_read
	6,													//nt_tex_new
	2													//nt_tex_destroy
};

static char const *ostr[] = {
	"raster_tri2",
	"ctx_new",
	"ctx_destroy",
	"putframe",
	"setctx",
	"start",
	"pixcopy",
	"pixdraw",
	"pixfill",
	"fb_set",
	"fb_new",
	"fb_destroy",
	"ptile_new",
	"ptile_destroy",
	"tdraw",
	"sput",
	"sget",
	"sb",
	"cb",
	"objbuf_new",
	"objbuf_destroy",
	"objbuf_map",
	"objbuf_unmap",
	"objbuf_write",
	"objbuf_read",
	"draw",
	"texbuf_new",
	"texbuf_destroy",
	"texbuf_map",
	"texbuf_unmap",
	"texbuf_write",
	"texbuf_read",
	"tex_new",
	"tex_destroy"
};
void nt_raise(ff_u8_t *__bin, ff_uint_t __size) {
	ffly_printf("bin size: %u\n", __size);

	nt_raise_sp = STACK_SIZE;
	nt_raise_p = __bin;
	ff_u8_t *end;

	end = nt_raise_p+__size;
	ff_u8_t on;
_again:
	on = *(nt_raise_p++);
	if (on >= MAX) {
		ffly_printf("invalid operation.\n");
		return;
	}
	ffly_printf("nought_op{%s}\n", ostr[on]);
	op[on]();

	nt_raise_p+=os[on];
	if (nt_raise_p != end) {
		goto _again;
	}
}
