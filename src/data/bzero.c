# include "bzero.h"
void ffly_bzero(void *__p, mdl_uint_t __n) {
    mdl_u8_t *itr = (mdl_u8_t*)__p;
    while(itr != (mdl_u8_t*)__p+__n) *(itr++) = 0x0;
}
