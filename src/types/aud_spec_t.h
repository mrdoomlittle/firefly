# ifndef __ffly_aud_spec__t__h
# define __ffly_aud_spec__t__h
# include "aud_format_t.h"
# include <mdlint.h>
typedef struct {
	ffly_aud_format_t format;
	mdl_u32_t rate;
	mdl_u16_t chn_c;
} ffly_aud_spec_t;
# endif /*__ffly_aud_spec__t__h*/
