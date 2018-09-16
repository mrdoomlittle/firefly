# include "raise.h"
# include "raster.h"
# include "context.h"
# include "framebuff.h"
# include "../system/io.h"
# include "shit.h"
# include "../dep/mem_cpy.h"
ff_u8_t *sr_raise_p;
ff_u8_t sr_raise_stack[STACK_SIZE];
ff_u16_t sr_raise_sp;
# define MAX 18

void static
sr_sput(void) {
	void *buf;
	ff_u32_t size;
	ff_u16_t dst;

	buf = *(void**)sr_raise_p;
	size = *(ff_u32_t*)(sr_raise_p+8);
	dst = *(ff_u16_t*)(sr_raise_p+12);

	ffly_mem_cpy(sr_raise_stack+dst, buf, size);
}

void static
sr_sget(void) {
	void *buf;
	ff_u32_t size;
	ff_u16_t src;

	buf = *(void**)sr_raise_p;
	size = *(ff_u32_t*)(sr_raise_p+8);
	src = *(ff_u16_t*)(sr_raise_p+12);

	ffly_mem_cpy(buf, sr_raise_stack+src, size);
}

static void(*op[])(void) = {
	sr_raster_tri2,
	sr_ctx_new,
	sr_ctx_destroy,
	sr_putframe,
	sr_setctx,
	sr_start,
	sr_finish,
	sr_pixcopy,
	sr_pixdraw,
	sr_pixfill,
	sr_fb_set,
	sr_fb_new,
	sr_fb_destroy,
	sr_ptile_new,
	sr_ptile_destroy,
	sr_tdraw,
	sr_sput,
	sr_sget
};

ff_uint_t static os[] = {
	(sizeof(ff_u16_t)*2)+(sizeof(ff_u32_t)*2),
	sizeof(ff_u16_t),
	sizeof(ff_u16_t),
	sizeof(ff_u8_t*)+(sizeof(ff_u32_t)*4),
	sizeof(ff_u16_t),
	0,
	0,
	0,
	(sizeof(ff_u32_t)*4)+sizeof(ff_u8_t*),
	sizeof(ff_u8_t*)+sizeof(ff_u32_t),
	sizeof(ff_u16_t),
	sizeof(ff_u16_t)+(sizeof(ff_u32_t)*2),
	sizeof(ff_u16_t),
	sizeof(ff_u16_t)+(sizeof(void*)*2),
	sizeof(ff_u16_t),
	sizeof(ff_u16_t)+(sizeof(ff_u32_t)*2),
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t),
	sizeof(void*)+sizeof(ff_u32_t)+sizeof(ff_u16_t)
};

void sr_raise(ff_u8_t *__bin, ff_uint_t __size) {
	ffly_printf("bin size: %u\n", __size);

	sr_raise_sp = STACK_SIZE;
	sr_raise_p = __bin;
	ff_u8_t *end;

	end = sr_raise_p+__size;
	ff_u8_t on;
_again:
	on = *(sr_raise_p++);
	if (on >= MAX) {
		ffly_printf("invalid operation.\n");
		return;
	}
	op[on]();

	sr_raise_p+=os[on];
	if (sr_raise_p != end) {
		goto _again;
	}
}
