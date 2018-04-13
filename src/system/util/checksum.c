# include "checksum.h"
ff_u32_t ffly_bsd_cksum32(void *__p, ff_uint_t __bc) {
    ff_u32_t ret = 0;
    ff_u8_t *p = (ff_u8_t*)__p;
    while(p != (ff_u8_t*)__p+__bc)
        ret = (((ret>>1)+((ret&0x1)<<15))+*(p++))&0xFFFF;
    return ret;
}

ff_u64_t ffly_bsd_cksum64(void *__p, ff_uint_t __bc) {
    ff_u64_t ret = 0;
    ff_u8_t *p = (ff_u8_t*)__p;
    while(p != (ff_u8_t*)__p+__bc)
        ret = (((ret>>1)+((ret&0x1)<<31))+*(p++))&0xFFFFFFFF;
    return ret;
}

ff_u64_t ffly_ff_cksum64(void *__p, ff_uint_t __bc) {
    ff_u64_t ret = 0;
    ff_u8_t *p = (ff_u8_t*)__p;
    while(p != (ff_u8_t*)__p+__bc) {
        ret = (((ret<<2)|(ret>>30&0x2))+(((*p)>>1)*(p-(ff_u8_t*)__p)))&0xFFFFFF;
        p++;
    }

    return ret;
}

ff_u32_t ffly_cksum32(void *__p, ff_uint_t __bc) {
    return ffly_bsd_cksum32(__p, __bc);
}

ff_u64_t ffly_cksum64(void *__p, ff_uint_t __bc) {
    return ffly_bsd_cksum64(__p, __bc);
}
