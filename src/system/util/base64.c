# include "base64.h"
# include <stdio.h>
char const static *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
ff_uint_t ffly_b64_encode(char *__dst, void *__src, ff_uint_t __len) {
    char *p = (ff_u8_t*)__dst;
    ff_u8_t *src = (ff_u8_t*)__src;
    ff_u8_t *end = src+__len;
    ff_u64_t buf = 0;
    ff_u8_t n = 0;
    while (src != end) {
        buf = buf<<8|*src;
        n+=8;
       _bk:
        if (n>=6) {
            *(p++) = table[(buf>>(n-6)&0x3f)];
            n-=6;         
            goto _bk;
        }
        src++;
    }
    if (n>0)
        *(p++) = table[(buf<<(6-n)&0x3f)];
    return p-__dst;
}

ff_uint_t ffly_b64_enc_len(ff_uint_t __len) {
    ff_uint_t ret = (((__len*8)>>1)/3);
    ret = ret+((__len-(ret*6))>0);
    return ret;
}

void ffly_b64_decode(void *__dst, void *__src, ff_uint_t __len) {

}
/*
# include <string.h>
int main() {
    char buf[200];
    memset(buf, '\0', 200);
    char const *s = "Heiu8u28u8du8uhj";
    ff_uint_t sl = strlen(s);
    ff_uint_t len = ffly_b64_encode(buf, (void*)s, sl);
    printf("%u, %u\n", len, ffly_b64_enc_len(sl));
 
}*/
