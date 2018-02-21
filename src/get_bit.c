# include "get_bit.h"
mdl_u8_t ffly_get_bit(ffly_getbitp __getbit, mdl_u8_t __n) {
	if (!__getbit->left) {
		__getbit->buf = __getbit->in(__getbit->arg_p);
		__getbit->left = 8;
	}

    mdl_u8_t ret = __getbit->buf>>(8-__n);
	__getbit->buf <<= __n;
	__getbit->left-=__n;
	return ret;
}