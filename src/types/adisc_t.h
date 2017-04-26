# ifndef __adisc__t__h
# define __adisc__t__h
# include "aformat_t.h"

# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	aformat_t format;
	u32_t rate;
	u16_t chn_c;
} adisc_t;
}
}
}
# endif
# endif /*__adisc__t__h*/
