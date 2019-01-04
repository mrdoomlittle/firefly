# ifndef __ffly__bron__driver__h
# define __ffly__bron__driver__h
# include "../ffint.h"
# include "../types.h"
# include "prim.h"
# include "view.h"
#define NOUGHT_BLEND 0x01
# define bron_driver \
	ff_bron_driver
#define BI_CTX_ST_SZ	0x00
#define BI_TRI2_ST_SZ	0x01
#define BI_TEX_ST_SZ	0x02
#define BI_VTX2_ST_SZ	0x03
struct ff_bron_driver {
	void(*sb)(ff_u8_t);
	void(*cb)(ff_u8_t);
	void(*tdraw)(ff_u32_t, ff_u32_t, ff_u32_t);
	void(*ptile_new)(ff_u32_t, void(*)(ff_u8_t, long long, void*), void*);
	void(*ptile_destroy)(ff_u32_t);
	void(*fb_set)(ff_u32_t);
	void(*fb_new)(ff_u32_t, ff_u32_t, ff_u32_t);
	void(*fb_destroy)(ff_u32_t);
	void(*rb_bind)(ff_u32_t, ff_u32_t);
	void(*rb_new)(ff_u32_t, ff_u32_t, ff_u32_t);
	void(*rb_destroy)(ff_u32_t);

	void(*pixcopy)(ff_u8_t*, ff_u32_t);
	void(*pixdraw)(ff_u32_t, ff_u32_t, ff_u8_t*, ff_u32_t, ff_u32_t);
	void(*pixfill)(ff_u32_t, ff_u8_t*);
	void(*sput)(void*, ff_uint_t, ff_u16_t);
	void(*sget)(void*, ff_uint_t, ff_u16_t);
	void(*ctx_new)(ff_u16_t);
	void(*ctx_destroy)(ff_u16_t);
	void(*raster_tri2)(ff_u16_t, ff_u32_t, ff_u32_t, ff_u32_t);
	void(*tri2)(struct bron_tri2*, ff_u16_t);
	void(*done)(void);

	void(*frame)(ff_u8_t*, ff_u32_t, ff_u32_t, ff_u32_t, ff_u32_t);
	void(*setctx)(ff_u16_t);
	void(*start)(void);
	void(*finish)(void);

	void(*objbuf_new)(ff_u32_t, ff_u32_t);
	void(*objbuf_destroy)(ff_u32_t);
	void(*objbuf_map)(ff_u32_t);
	void(*objbuf_unmap)(ff_u32_t);
	void(*objbuf_write)(ff_u32_t, ff_u32_t, ff_u32_t, void*);
	void(*objbuf_read)(ff_u32_t, ff_u32_t, ff_u32_t, void*);

	void(*texbuf_new)(ff_u32_t, ff_u32_t);
	void(*texbuf_destroy)(ff_u32_t);
	void(*texbuf_map)(ff_u32_t);
	void(*texbuf_unmap)(ff_u32_t);
	void(*texbuf_write)(ff_u32_t, ff_u32_t, ff_u32_t, void*);
	void(*texbuf_read)(ff_u32_t, ff_u32_t, ff_u32_t, void*);

	void(*tex_new)(ff_u32_t, ff_u32_t, ff_u32_t, ff_u32_t);
	void(*tex_destroy)(ff_u32_t);

	void(*draw)(ff_u32_t, ff_u32_t);
	void(*info)(ff_u8_t, long long);
	void(*viewport)(struct bron_viewport*);
	ff_uint_t ctx_struc_sz;
	ff_uint_t tri2_struc_sz;
	ff_uint_t tex_struc_sz;
};

enum {
	_bron_dd_nought
};

# define bron_dd \
	ffly_bron_driver

void ffly_bron_driver(ff_u8_t, struct bron_driver*);
# endif /*__ffly__bron__driver__h*/
