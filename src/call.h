# ifndef __ffly__call__h
# define __ffly__call__h
# include <mdlint.h>
enum {
    _ffcal_printf,
    _ffcal_fprintf,
	_ffcal_malloc,
	_ffcal_free,
	_ffcal_realloc
};

enum {
	_ffpa_seg
};

typedef struct ffpa {
	struct ffpa *next;
	mdl_u8_t type;
	void *p, *end;
} *ffpap;

void ffcall(mdl_u8_t, void*, void*);
# endif /*__ffly__call__h*/
