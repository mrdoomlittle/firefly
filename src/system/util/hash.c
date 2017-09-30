# include "hash.h"
// going to be using this for now
mdl_u64_t static mdl_hash(mdl_u8_t const *__key, ffly_size_t __bc) {
	mdl_u8_t const *itr = __key;
	mdl_u64_t ret_val = 2<<(__bc>>2);
	while(itr != __key+__bc) {
		ret_val = (((ret_val>>1)+1)*(ret_val<<2))+(*itr*(((itr-__key)+1)<<1));
		itr++;
	}

	return ret_val;
}

mdl_u64_t ffly_hash(mdl_u8_t const *__key, ffly_size_t __bc) {
	return mdl_hash(__key, __bc);
}
