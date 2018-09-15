# include "exec.h"
# include "../rosin.h"
static ffly_tapep tape;
ff_u32_t static ip;

ff_u8_t static
fetch_byte(ff_off_t __off) {
	ff_u8_t rt;
	ffly_tape_get(tape, ip+__off, &rt, 1);
	return rt;
}

void static
ip_incr(ff_uint_t __by) {
	ip+=__by;
}

ff_addr_t static
get_ip(void) {
	return ip;
}

void static
set_ip(ff_addr_t __to) {
	ip = __to;
}

static struct ffly_rosin ctx = {
	.stack_size = 700,
	.fetch_byte = fetch_byte,
	.ip_incr = ip_incr,
	.get_ip = get_ip,
	.set_ip = set_ip
};

void ffros_exec(ffly_tapep __tape) {
	ip = 0;
	tape = __tape;
	ff_rosin_init(&ctx);
	ff_rosin_run(&ctx);
	ff_rosin_de_init(&ctx);
}
//# define DEBUG
# ifdef DEBUG
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_tapep tape;

	tape = ffly_tape_new();
	ff_u8_t code[] = {	
		0x08, 0x00, 0x00,
		0x07, 0x00, 0x00,
		0x09,
		0x0b, 0x00, 0x00,
		0x0c, 0x00, 0x00,
		0x00, 0x00, 0x18, 0x00,
		0x0d, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
		0x0a
	};

	ffly_tape_insert(tape, code, sizeof(code));
	ffros_exec(tape);

	ffly_tape_raze(tape);
}
# endif
