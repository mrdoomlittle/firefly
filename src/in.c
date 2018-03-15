# include "in.h"
# include "dep/byteswap.h"
uint16_t htons(uint16_t __x) {
	return bswp_16(__x);
}
