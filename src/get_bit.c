# include "get_bit.h"
ff_u8_t ffly_get_bit(ffly_getbitp __getbit, ff_u8_t __n) {
	ff_u8_t ret;
	if (!__getbit->left) {
		__getbit->buf = __getbit->in(__getbit->arg_p);
		__getbit->left = 8;
	}

	if (__getbit->left<__n) {
		ff_u8_t ext = __n-__getbit->left;
		ret = __getbit->buf;
		__getbit->left = 8;
		__getbit->buf = __getbit->in(__getbit->arg_p);
		ret = ret|(ffly_get_bit(__getbit, ext)<<(__n-ext));
		return ret;
	}

    ret = __getbit->buf&(0xff>>(8-__n));
	__getbit->buf>>=__n;
	__getbit->left-=__n;
	return ret;
}
