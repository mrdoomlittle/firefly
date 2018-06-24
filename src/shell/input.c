# include "input.h"
void(*get)(ff_uint_t, ff_uint_t, void*);
ff_u8_t(*at_eof)(void);
ff_u8_t static buf;
ff_i8_t static pres = -1;
void ugetc(ff_u8_t __b) {
	buf = __b;
	pres = 0;
}

ff_u8_t getc(void) {
	if (!pres) {
		pres = -1;
		return buf;
	}
	ff_u8_t ret;
	get(1, 0, &ret);
	return ret;
}
