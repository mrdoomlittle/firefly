# ifndef __adisc__t__h
# define __adisc__t__h
# include <eint_t.h>
# include "aformat_t.h"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	aformat_t format;
	u32_t rate;
	u8_t chn_c;
} adisc_t;
}
}
}
# endif /*__adisc__t__h*/
