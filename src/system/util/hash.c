# include "hash.h"
// going to be using this for now
ff_u64_t static
ff_hash(ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u8_t const *p = __key;
	ff_u8_t const *end = p+__bc;
	ff_u64_t ret_val = 2<<(__bc>>2);
	while(p != end) {
		ret_val = (((ret_val>>1)+1)*(ret_val<<2))+(*p*(((p-__key)+1)<<1));
		p++;
	}
	return ret_val;
}

ff_u64_t static
ff_dvhash(ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u8_t const *p = __key;
	ff_u8_t const *end = p+__bc;
	ff_u64_t ret_val = 1;
	while(p != end) {
		ret_val = ((ret_val&0xffffffffffffffff<<8)|(((ret_val&0xff)**p))>>2)<<4;
		ret_val<<=2;
		ret_val |= ret_val>>60;
		p++;
	}
	return ret_val;
}

ff_u64_t static
ff_ahash(ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u8_t const *p = __key;
	ff_u8_t const *end = p+__bc;
	ff_u64_t ret_val = 1;
	while(p != end) {
		ret_val = ret_val<<16|(ret_val>>48);
		ret_val = ret_val^(*p*0x67452301);
		ret_val &= 0xffffffffffffff;
		p++;
	}
	return ret_val;
}

ff_u64_t static
ff_rest(ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u8_t const *p = __key;
	ff_u8_t const *end = p+__bc;
	ff_u64_t ret_val = 0xffffffffffffffff;
	while(p != end) {
		ret_val = ret_val>>4|((ret_val>>60)^((ret_val&0xf)<<60));
		ret_val	= ret_val^(*__key^(ret_val&0xff));
		p++;
	}
	return ret_val;
}

ff_u64_t(*ffly_hash)(ff_u8_t const*, ff_uint_t) = &ff_rest;
