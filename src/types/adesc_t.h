# ifndef __adesc__t__h
# define __adesc__t__h
# include "aformat_t.h"

# ifdef __cplusplus
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	aformat_t format;
	u32_t rate;
	u16_t chn_c;
} adesc_t;
}
}
}
# endif
# endif /*__adesc__t__h*/
