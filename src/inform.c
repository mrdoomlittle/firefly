# include "inform.h"
# include "ffly_def.h"
void static _inform(ff_u16_t __what) {
	switch(__what) {
		default:
			if (ffly_inform_user != NULL)
				ffly_inform_user(__what);
	}
}

void(*ffly_inform)(ff_u16_t) = _inform;
void(*ffly_inform_user)(ff_u16_t) = NULL;
