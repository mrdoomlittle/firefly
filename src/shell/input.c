# include "input.h"
void(*get)(ff_uint_t, ff_uint_t, void*);
ff_u8_t(*at_eof)(void);
ff_u8_t getc(void) {
	ff_u8_t ret;
	get(1, 0, &ret);
	return ret;
}
