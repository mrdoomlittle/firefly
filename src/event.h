# ifndef __ffly__event__h
# define __ffly__event__h
# include "ffint.h"
# include "types.h"

# define ff_event_build(__kind, __field, __data, __size, __p) \
    *(__p) = (ff_event_t){.kind=(__kind), .field=(__field), .data=(__data), .size=(__size)}

enum ffly_event_kind {
	_ffly_wd_ek_key_press,
	_ffly_wd_ek_key_release,
	_ffly_wd_ek_btn_press,
	_ffly_wd_ek_btn_release,
	_ffly_wd_ek_closed,
	_ffly_ek_unknown
};

enum ffly_event_field {
	_ffly_ef_wd,
};

typedef struct ff_event {
	ff_u8_t kind;
/*
	TODO:
		rename field to sector or .... somthing that specifies a location from where
		within the engine its come from, window, ...
*/
	ff_u8_t field;
	void *data;
	ff_uint_t size;
} ff_event_t, *ff_eventp, ffly_event_t;

ff_eventp ff_event_creat(ff_u8_t, ff_u8_t, void*, ff_uint_t, ff_err_t*);
ff_err_t ff_event_del(ff_eventp);

ff_eventp ff_event_alloc(ff_err_t*);
ff_err_t ff_event_free(ff_eventp);
ff_i8_t ff_event_poll(ff_eventp*);
void ff_event_cleanup(void);
# endif /*__ffly__event__h*/
