# include "bcopy.h"
void ffly_bcopy(void *__dst, void *__src, mdl_uint_t __n) {
    mdl_u8_t *itr = (mdl_u8_t*)__src;
    while(itr != (mdl_u8_t*)__src+__n)
        *((mdl_u8_t*)__dst+(itr-(mdl_u8_t*)__src)) = *(itr++);
}
