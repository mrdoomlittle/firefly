# include "shit.h"
# include "raise.h"
ff_u8_t sr_bits = 0x00;
void sr_sb(void) {
	ff_u8_t bits;
	bits = *sr_raise_p;
	sr_bits |= bits;
}

void sr_cb(void) {
	ff_u8_t bits;
	bits = *sr_raise_p;
	sr_bits &= ~bits;
}
