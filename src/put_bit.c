# include "put_bit.h"
void ffly_put_bit(ffly_putbitp __putbit, mdl_u8_t __bits, mdl_u8_t __n) {
	if (!__putbit->left) {
		__putbit->left = 8;
		__putbit->out(__putbit->buf);
		__putbit->buf = 0x0;
	}
	__putbit->buf = __putbit->buf>>__n|((__bits&(0xff>>(8-__n)))<<(8-__n));
	__putbit->left-=__n;
}
