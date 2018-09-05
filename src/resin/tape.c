# include "exec.h"
# include "../ffly_def.h"
static ffly_tapep tape;
void static
get(ff_uint_t __from, ff_uint_t __size, void *__buf) {
	ffly_tape_get(tape, __from, __buf, __size);
}

void ffres_xt(ffly_tapep __tape, ff_u32_t __entry) {
	tape = __tape;
	ffres_exec(get, tape->len, NULL, NULL, __entry);
}
