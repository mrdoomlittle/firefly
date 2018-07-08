# include "../ffint.h"
# include "../types.h"
# include "../br.h"
# include "../hexdump.h"
# include "../chrdump.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_uint_t const sz = (1<<_ff_brick_32)*4;
	char buf[sz];
	ffly_br_retrieve(_ff_brick_32, 4, buf);
	ffly_hexdump(buf, sz);
	ffly_chrdump(buf, sz);
}
