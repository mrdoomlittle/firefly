# include <eint_t.h>
# include "byte_ncpy.h"
void ffly_swp(void *__p1, void *__p2, mdl_uint_t __n) {
	mdl_u8_t tmp[__n];
	ffly_byte_ncpy((void*)tmp, __p1, __n);
	ffly_byte_ncpy(__p1, __p2, __n);
	ffly_byte_ncpy(__p2, (void*)tmp, __n);
}
