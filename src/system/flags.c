# include "flags.h"
ffly_bool_t ffly_is_flag(ffly_flag_t __flags, ffly_flag_t __flag) {
	return (__flags & __flag) == __flag;
}

void ffly_add_flag(ffly_flag_t *__flags, ffly_flag_t __flag, ffly_bool_t __ow) {
	if (__ow) {
		*__flags = __flag;
		return;
	}

	*__flags |= __flag;
}

void ffly_rm_flag(ffly_flag_t *__flags, ffly_flag_t __flag) {
	*__flags ^= (*__flags) & __flag;
}
