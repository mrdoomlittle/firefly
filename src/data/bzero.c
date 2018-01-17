# include "bzero.h"
void ffly_bzero(void *__p, mdl_uint_t __n) {
    mdl_u8_t *p = (mdl_u8_t*)__p;
    mdl_u8_t *end = p+__n;
    while(p != end) *(p++) = 0x0;
}
