# include <stdio.h>
//# include "byteswap.h"
//# include <netinet/in.h>
# include "../ffint.h"
# include "bzero.h"
int main() {
//	mdl_u16_t a = 1;
//	a = bswp_16(a);
//	printf("%u, %u\n", a, htons(1));
	ff_u64_t x = 0xffffff;
	ffly_bzero(&x, 3);
	printf("%u, %u, %u\n", x&0xff, x>>8&0xff, x>>16&0xff);

}
