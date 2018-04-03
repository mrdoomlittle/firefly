# include <mdlint.h>
# ifdef NONFF
# include <stdio.h>
# else
# include "../stdio.h"
# endif
void ffly_printbin(mdl_u64_t __val, mdl_u8_t __n) {
	mdl_u8_t off = 0;
	while(off != __n)
		printf("%u", __val>>((__n-1)-(off++))&0x1);
	printf("\n");
}
