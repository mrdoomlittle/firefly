# include "hash.h"
// going to be using this for now
mdl_u64_t static mdl_hash(mdl_u8_t const *__key, mdl_uint_t __bc) {
	mdl_u8_t const *p = __key;
	mdl_u8_t const *end = p+__bc;
	mdl_u64_t ret_val = 2<<(__bc>>2);
	while(p != end) {
		ret_val = (((ret_val>>1)+1)*(ret_val<<2))+(*p*(((p-__key)+1)<<1));
		p++;
	}
	return ret_val;
}

mdl_u64_t static mdl_dvhash(mdl_u8_t const *__key, mdl_uint_t __bc) {
	mdl_u8_t const *p = __key;
	mdl_u8_t const *end = p+__bc;
	mdl_u64_t ret_val = 1;
	while(p != end) {
		ret_val = ((ret_val&0xffffffffffffffff<<8)|(((ret_val&0xff)**p))>>2)<<4;
		ret_val<<=2;
		ret_val |= ret_val>>60;
		p++;
	}
	return ret_val;
}

mdl_u64_t(*ffly_hash)(mdl_u8_t const*, mdl_uint_t) = &mdl_dvhash;
