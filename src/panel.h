# ifndef __ffly__panel__h
# define __ffly__panel__h
# include "ffint.h"
# include "types.h"
# include "m.h"
enum {
	_ffly_po_disconnect,
	_ffly_po_halt,
	_ffly_po_resume,
	_ffly_po_meminfo
};

void ff_p_connect(ff_uint_t);
void ff_p_disconnect(void);
void ff_p_meminfo(struct ffly_meminfo*);

void ffly_panel_start(void);
# endif /*__ffly__panel__h*/
