# include <stdio.h>
# include "byteswap.h"
# include <netinet/in.h>
int main() {
	mdl_u16_t a = 1;
	a = bswp_16(a);
	printf("%u, %u\n", a, htons(1));
}
