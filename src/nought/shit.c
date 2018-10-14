# include "shit.h"
# include "raise.h"
ff_u8_t nt_bits = 0x00;
void nt_sb(void) {
	ff_u8_t bits;
	bits = *nt_raise_p;
	nt_bits |= bits;
}

void nt_cb(void) {
	ff_u8_t bits;
	bits = *nt_raise_p;
	nt_bits &= ~bits;
}
