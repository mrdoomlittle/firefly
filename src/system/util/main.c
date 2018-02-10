# include <stdio.h>
# include "checksum.h"
# include <string.h>
# include <mdlint.h>
# include "hash.h"
int main() {
	

//    char const *s = "ijuhs8fusdhfsdnkjfnsifuini38hfdjfhsyhfsdjf9uh4euir34h8yhiusanidhfidshi4e8yr784:q";
  //  mdl_uint_t l = strlen(s);
    //printf("32: %u, 64: %lu, mdl: %lu\n", ffly_bsd_cksum32((void*)s, l), ffly_bsd_cksum64((void*)s, l), ffly_mdl_cksum64((void*)s, l));

	mdl_u8_t data[2];
	data[2] = 212;
	mdl_u8_t *i = data;
	while(*i != 0xff) {
		printf("%u\n", ffly_hash(data, 2));
		(*i)++;
	}
	char const *text = "abcddhhasjskdjhfihshighgsugsfsuytgfsuisuhyfuysdhfijgsuyfhuidhgufshifsdguyd";
	printf("%u\n", ffly_hash(text, strlen(text)));
}
