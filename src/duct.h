# ifndef __ffly__duct__h
# define __ffly__duct__h
# include "ffint.h"
# include "event.h"
# include "system/event.h"
typedef struct ff_dt {
	void *text;
	ff_uint_t len;
} *ff_dtp;


#define __duct_open(__c, ...)\
	(__c)->open((__c)->arg)
#define __duct_close(__c, ...)\
	(__c)->close((__c)->arg)
#define __duct_read(__c, ...)\
	(__c)->read((__c)->arg, __VA_ARGS__)
#define __duct_write(__c, ...)\
	(__c)->write((__c)->arg, __VA_ARGS__)
#define __duct_connect(__c, ...)\
	(__c)->connect((__c)->arg)
#define __duct_listen(__c, ...)\
	(__c)->listen((__c)->arg)
#define duct(__c, __func, ...)\
	__duct_ ## __func (__c, __VA_ARGS__)

typedef struct ff_dc {
	ff_u8_t pipe;
	void(*open)(long long);
	void(*close)(long long);
	void(*read)(long long, void*, ff_uint_t);
	void(*write)(long long, void*, ff_uint_t);
	void(*connect)(long long);
	void(*listen)(long long);
	long long arg;
} *ff_dcp;

/*
	duct event struct
*/
typedef struct ff_des {
	ff_u8_t kind;
	ff_u8_t field;
	ff_uint_t size;
	ff_u16_t data;
} *ff_desp;

ff_dcp ff_duct(ff_u8_t, ff_u8_t);
void ff_duct_destroy(ff_dcp);
ff_i8_t ff_duct_serve(ff_dcp);

void ff_duct_get_frame(ff_dcp, void*, ff_uint_t, ff_uint_t, ff_u8_t);
void ff_duct_exit(ff_dcp);
void ff_duct_read(ff_dcp, void*, ff_u16_t, ff_u32_t, ff_u32_t);
void ff_duct_write(ff_dcp, void*, ff_u16_t, ff_u32_t, ff_u32_t);
ff_u16_t ff_duct_alloc(ff_dcp, ff_u32_t);
void ff_duct_free(ff_dcp, ff_u16_t);
void ff_duct_event(ff_dcp, ff_eventp);
void ff_duct_done(ff_dcp);
# endif /*__ffly__duct__h*/
