# include "bcopy.h"
void ffly_bcopy(void *__dst, void *__src, mdl_uint_t __n) {
    mdl_u8_t *p = (mdl_u8_t*)__src;
    mdl_u8_t *end = p+__n;
    while(p != end)
        *((mdl_u8_t*)__dst+(p-(mdl_u8_t*)__src)) = *(p++);
}
