# ifndef __ffly__duct__h
# define __ffly__duct__h
# include "ffint.h"
# include "event.h"
# include "system/event.h"
typedef struct ff_dt {
	void *text;
	ff_uint_t len;
} *ff_dtp;


typedef struct ff_dc {
	ff_u8_t pipe;
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

ff_dcp ff_duct_open(ff_u8_t);
void ff_duct_close(ff_dcp);
void ff_duct_connect(ff_dcp);
void ff_duct_listen(ff_dcp);
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
