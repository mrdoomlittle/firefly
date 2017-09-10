# ifndef __ffly__aud__fad__t__h
# define __ffly__aud__fad__t__h
# include "byte_t.h"
# include "size_t.h"
# include "aud_fformat_t.h"
typedef struct {
	ffly_byte_t *fp;
	ffly_size_t fsize;
	ffly_aud_fformat_t fformat;
} ffly_aud_fad_t;

# endif /*__ffly__aud__fad__t__h*/
