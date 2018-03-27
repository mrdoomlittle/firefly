# include "in.h"
# include "dep/byteswap.h"
mdl_u16_t htons(mdl_u16_t __x) {
	return bswp_16(__x);
}
