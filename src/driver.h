# ifndef __ffly__driver__h
# define __ffly__driver__h
# include "ffint.h"
# include "types.h"
# include "prim.h"
struct ff_driver {
	void(*tdraw)(ff_u16_t, ff_u32_t, ff_u32_t);
	void(*ptile_new)(ff_u16_t, void(*)(ff_u8_t, long long, void*), void*);
	void(*ptile_destroy)(ff_u16_t);
	void(*fb_set)(ff_u16_t);
	void(*fb_new)(ff_u32_t, ff_u32_t, ff_u16_t);
	void(*fb_destroy)(ff_u16_t);
	void(*pixcopy)(ff_u8_t*, ff_u32_t);
	void(*pixdraw)(ff_u32_t, ff_u32_t, ff_u8_t*, ff_u32_t, ff_u32_t);
	void(*pixfill)(ff_u32_t, ff_u8_t*);
	void(*sput)(void*, ff_uint_t, ff_u16_t);
	void(*sget)(void*, ff_uint_t, ff_u16_t);
	void(*ctx_new)(ff_u16_t);
	void(*ctx_destroy)(ff_u16_t);
	void(*raster_tri2)(ff_u16_t, ff_u16_t, ff_u32_t, ff_u32_t);
	void(*tri2)(struct ffly_tri2*, ff_u16_t);
	void(*tex)(struct ffly_tex*, ff_u16_t);
	void(*done)(void);

	ff_uint_t(*new_bufobj)(void);
	void(*frame)(ff_u8_t*, ff_u32_t, ff_u32_t, ff_u32_t, ff_u32_t);
	void(*setctx)(ff_u16_t);
	void(*start)(void);
	void(*finish)(void);

	ff_uint_t ctx_struc_sz;
	ff_uint_t tri2_struc_sz;
	ff_uint_t tex_struc_sz;
};

enum {
	_driver_sr
};

void ffly_driver(ff_u8_t, struct ff_driver*);
# endif /*__ffly__driver__h*/
