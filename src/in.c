# include "in.h"
# include "dep/byteswap.h"
ff_u16_t htons(ff_u16_t __x) {
	return bswp_16(__x);
}
