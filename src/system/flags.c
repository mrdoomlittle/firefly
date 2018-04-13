# include "flags.h"
ff_bool_t ffly_is_flag(ff_flag_t __flags, ff_flag_t __flag) {
	return (__flags&__flag) == __flag;
}

void ffly_add_flag(ff_flag_t *__flags, ff_flag_t __flag, ff_bool_t __ow) {
	if (__ow) {
		*__flags = __flag;
		return;
	}

	*__flags |= __flag;
}
// change to ffly_clr_flag
void ffly_rm_flag(ff_flag_t *__flags, ff_flag_t __flag) {
	*__flags ^= (*__flags)&__flag;
}
