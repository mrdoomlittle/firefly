# ifndef __ffly__bron__depth__h
# define __ffly__bron__depth__h
# include "../ffint.h"

#define bron_dpb_at(__x, __y)\
	(BRON_CONTEXT->dpbuf+(__x)+((__y)*BRON_CONTEXT->width))
void bron_dpb_alloc(void);
void bron_dpb_free(void);
void bron_dpb_save(void);
# endif /*__ffly__bron__depth__h*/
