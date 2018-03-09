# ifndef __ffly__call__h
# define __ffly__call__h
# include <mdlint.h>
enum {
    _ffcal_printf,
    _ffcal_fprintf
};

void ffcall(mdl_u8_t, void*, void*);
# endif /*__ffly__call__h*/
