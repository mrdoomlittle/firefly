# include "../clay.h"
void(*clay_get)(ff_uint_t, ff_uint_t, void*);
ff_u8_t(*clay_at_eof)(void);
ff_u8_t static buf;
ff_i8_t static pres = -1;
void clay_ugetc(ff_u8_t __b) {
	buf = __b;
	pres = 0;
}

ff_u8_t clay_getc(void) {
	if (!pres) {
		pres = -1;
		return buf;
	}

	ff_u8_t rt;
	clay_get(1, 0, &rt);
	return rt;
}
