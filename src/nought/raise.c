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
	nt_raster_tri2,		//0
	nt_ctx_new,			//1
	nt_ctx_destroy,		//2
	nt_putframe,		//3
	nt_setctx,			//4
	nt_start,			//5
	nt_finish,			//6
	nt_pixcopy,			//7
	nt_pixdraw,			//8
	nt_pixfill,			//9
	nt_fb_set,			//10
	nt_fb_new,			//11
	nt_fb_destroy,		//12
	nt_ptile_new,		//13
	nt_ptile_destroy,	//14
	nt_tdraw,			//15
	nt_sput,			//16
	nt_sget,			//17
	nt_sb,				//18
	nt_cb,				//19
	nt_objbuf_new,		//20
	nt_objbuf_destroy,	//21
	nt_objbuf_map,		//22
	nt_objbuf_unmap,	//23
	nt_objbuf_write,	//24
	nt_objbuf_read,		//25
	nt_draw,			//26
	nt_texbuf_new,		//27
	nt_texbuf_destroy,	//28
	nt_texbuf_map,		//29
	nt_texbuf_unmap,	//30
	nt_texbuf_write,	//31
	nt_texbuf_read,		//32
	nt_tex_new,			//33
	nt_tex_destroy		//34
};

ff_uint_t static os[] = {
	sizeof(ff_u16_t)+(sizeof(ff_u32_t)*3),				//nt_raster_tri2		-0
	sizeof(ff_u16_t),									//nt_ctx_new			-1
	sizeof(ff_u16_t),									//nt_ctx_destroy		-2
	sizeof(ff_u8_t*)+(sizeof(ff_u32_t)*4),				//nt_putframe			-3
	sizeof(ff_u16_t),									//nt_setctx				-4
	0,													//nt_start				-5
	0,													//nt_finish				-6
	0,													//nt_pixcopy			-7
	(sizeof(ff_u32_t)*4)+sizeof(ff_u8_t*),				//nt_pixdraw			-8
	sizeof(ff_u8_t*)+sizeof(ff_u32_t),					//nt_pixfill			-9
	sizeof(ff_u32_t),									//nt_fb_set				-10
	sizeof(ff_u32_t)+(sizeof(ff_u32_t)*2),				//nt_fb_new				-11
	sizeof(ff_u32_t),									//nt_fb_destroy			-12
	sizeof(ff_u32_t)+(sizeof(void*)*2),					//nt_ptile_new			-13
	sizeof(ff_u32_t),									//nt_ptile_destroy		-14
	sizeof(ff_u32_t)+(sizeof(ff_u32_t)*2),				//nt_tdraw				-15
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t),	//nt_sput				-16
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t),	//nt_sget				-17
	1,													//nt_sb					-18
	1,													//nt_cb					-19
	8,													//nt_objbuf_new			-20
	4,													//nt_objbuf_destroy		-21
	4,													//nt_objbuf_map			-22
	4,													//nt_objbuf_unmap		-23
	20,													//nt_objbuf_write		-24
	20,													//nt_objbuf_read		-25
	8,													//nt_draw				-26
	8,													//nt_texbuf_new			-27
	4,													//nt_texbuf_destroy		-28
	4,													//nt_texbuf_map			-29
	4,													//nt_texbuf_unmap		-30
	20,													//nt_texbuf_write		-31
	20,													//nt_texbuf_read		-32
	16,													//nt_tex_new			-33
	4													//nt_tex_destroy		-34
};

static char const *ostr[] = {
	"raster_tri2",		//0
	"ctx_new",			//1
	"ctx_destroy",		//2
	"putframe",			//3
	"setctx",			//4
	"start",			//5
	"finish",			//6
	"pixcopy",			//7
	"pixdraw",			//8
	"pixfill",			//9
	"fb_set",			//10
	"fb_new",			//11
	"fb_destroy",		//12
	"ptile_new",		//13
	"ptile_destroy",	//14
	"tdraw",			//15
	"sput",				//16
	"sget",				//17
	"sb",				//18
	"cb",				//19
	"objbuf_new",		//20
	"objbuf_destroy",	//21
	"objbuf_map",		//22
	"objbuf_unmap",		//23
	"objbuf_write",		//24
	"objbuf_read",		//25
	"draw",				//26
	"texbuf_new",		//27
	"texbuf_destroy",	//28
	"texbuf_map",		//29
	"texbuf_unmap",		//30
	"texbuf_write",		//31
	"texbuf_read",		//32
	"tex_new",			//33
	"tex_destroy"		//34
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
//#ifdef DEBUG
	ffly_printf("nought_op{%s}\n", ostr[on]);
	ffly_fdrain(ffly_out);
//#endif
	op[on]();

	nt_raise_p+=os[on];
	if (nt_raise_p != end) {
		goto _again;
	}
}
