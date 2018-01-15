# include "checksum.h"
mdl_u32_t ffly_bsd_cksum32(void *__p, mdl_uint_t __bc) {
    mdl_u32_t ret = 0;
    mdl_u8_t *p = (mdl_u8_t*)__p;
    while(p != (mdl_u8_t*)__p+__bc)
        ret = (((ret>>1)+((ret&0x1)<<15))+*(p++))&0xFFFF;
    return ret;
}

mdl_u64_t ffly_bsd_cksum64(void *__p, mdl_uint_t __bc) {
    mdl_u64_t ret = 0;
    mdl_u8_t *p = (mdl_u8_t*)__p;
    while(p != (mdl_u8_t*)__p+__bc)
        ret = (((ret>>1)+((ret&0x1)<<31))+*(p++))&0xFFFFFFFF;
    return ret;
}
