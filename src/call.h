# ifndef __ffly__call__h
# define __ffly__call__h
# include "ffint.h"
enum {
    _ffcal_printf,
    _ffcal_fprintf,
	_ffcal_malloc,
	_ffcal_free,
	_ffcal_realloc,
	_ffcal_mod_dcp,
	_ffcal_mod_scp
};

enum {
	_ffpa_seg,
	_ffpa_u
};

typedef struct ffpa {
	struct ffpa *next;
	ff_u8_t type;
	void *p, *end;
} *ffpap;

void ffcall(ff_u8_t, void*, void*);
# endif /*__ffly__call__h*/
