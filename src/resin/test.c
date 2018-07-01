# include "exec.h"
# include "../ffint.h"
# include "../types.h"
# include "../stdio.h"
# include "../resin.h"
# include "../string.h"
ff_u8_t static bin[] = {	
	_op_r0r0,
	_op_r1r1,
	_op_as1qr0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	_op_as1qr1, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	_op_push1qr0,
	_op_push1qr1,
	_op_as1qr0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	_op_as1qr1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	_op_pop1qr1,
	_op_pop1qr0,
//op_lspr0,
	_op_out1qr0,
	_op_out1qr1,
	_op_exit, 0x0, 0x0
};

ff_u8_t static *p = bin;
void static
get(ff_uint_t __from, ff_uint_t __offset, ff_uint_t __size, void *__buf) {
	memcpy((ff_u8_t*)__buf+__offset, p+__from, __size);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	printf("start.\n");
	ffres_exec(get, sizeof(bin), NULL, NULL, 0);
	printf("done.\n");
}
