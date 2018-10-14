# include "raise.h"
# include "raster.h"
# include "context.h"
# include "framebuff.h"
# include "../system/io.h"
# include "shit.h"
# include "../dep/mem_cpy.h"
ff_u8_t *nt_raise_p;
ff_u8_t nt_raise_stack[STACK_SIZE];
ff_u16_t nt_raise_sp;
# define MAX 19
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
	nt_cb
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
	1													//nt_cb
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
	op[on]();

	nt_raise_p+=os[on];
	if (nt_raise_p != end) {
		goto _again;
	}
}
