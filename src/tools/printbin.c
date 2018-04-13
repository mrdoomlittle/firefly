# include "../ffint.h"
# ifdef NONFF
# include <stdio.h>
# else
# include "../stdio.h"
# endif
void ffly_printbin(ff_u64_t __val, ff_u8_t __n) {
	ff_u8_t off = 0;
	while(off != __n)
		printf("%u", __val>>((__n-1)-(off++))&0x1);
	printf("\n");
}
