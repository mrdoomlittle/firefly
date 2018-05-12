# include "put_bit.h"
void ffly_put_bit(ffly_putbitp __putbit, ff_u8_t __bits, ff_u8_t __n) {
	if (!__putbit->left) {
		__putbit->left = 8;
		__putbit->out(__putbit->buf);
		__putbit->buf = 0x0;
	}

	if (__putbit->left<__n) {
		ff_u8_t ext = __n-__putbit->left;
		__putbit->buf = __putbit->buf>>__putbit->left|((__bits&(0xff>>(8-__putbit->left)))<<(8-__putbit->left));
		__putbit->out(__putbit->buf);
		__putbit->left = 8;
		__putbit->buf = 0x0;
		ffly_put_bit(__putbit, __bits>>(__n-ext), ext);
		return;
	}

	__putbit->buf = __putbit->buf>>__n|((__bits&(0xff>>(8-__n)))<<(8-__n));
	__putbit->left-=__n;
}
