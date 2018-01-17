# include <stdio.h>
# include "checksum.h"
# include <string.h>
# include <mdlint.h>
int main() {
    char const *s = "ijuhs8fusdhfsdnkjfnsifuini38hfdjfhsyhfsdjf9uh4euir34h8yhiusanidhfidshi4e8yr784:q";
    mdl_uint_t l = strlen(s);
    printf("32: %u, 64: %lu, mdl: %lu\n", ffly_bsd_cksum32((void*)s, l), ffly_bsd_cksum64((void*)s, l), ffly_mdl_cksum64((void*)s, l));
}
