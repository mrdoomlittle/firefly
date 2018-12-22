# include "mode.h"
ff_i8_t ffmode = _ff_mod_def;
void ffset_mode(ff_i8_t __mode) {
	ffmode = __mode;
}

ff_u8_t ffis_mode(ff_i8_t __mode) {
	return ffmode == __mode;
}

ff_i8_t ffget_mode(void) {
	return ffmode;
}
