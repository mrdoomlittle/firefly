# include "mode.h"
mdl_i8_t ffmode = _ff_mod_def;
void ffset_mode(mdl_i8_t __mode) {
	ffmode = __mode;
}

mdl_u8_t ffis_mode(mdl_i8_t __mode) {
	return ffmode == __mode;
}

mdl_i8_t ffget_mode() {
	return ffmode;
}
