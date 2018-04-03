# include "../bci.h"
# include "../stdio.h"
# include <mdlint.h>
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../ffef.h"
mdl_u8_t static *bin = NULL, *end = NULL;
mdl_u32_t static ip;
mdl_u8_t static
fetch_byte(ffly_off_t __off) {
	if (bin+ip+__off >= end) return 0x0;
	return *(bin+ip+__off);
}

void static
ip_incr(mdl_uint_t __by) {
	ip+=__by;
}

ffly_addr_t static
get_ip() {
	return ip;
}

void static
set_ip(ffly_addr_t __to) {
	ip = __to;
}

void* ring(mdl_u8_t __no, void *__argp) {
	printf("ring ring hello?\n");
}

static struct ffly_bci ctx = {
	.stack_size = 200,
	.fetch_byte = fetch_byte,
	.ip_incr = ip_incr,
	.get_ip = get_ip,
	.set_ip = set_ip,
	.rin = ring
};

void ffbci_exec(void *__bin, void *__end, void(*__prep)(void*, void*), void *__hdr, mdl_u32_t __entry) {
	bin = (mdl_u8_t*)__bin;
	end = (mdl_u8_t*)__end;
	ip = __entry;
	ffly_bci_init(&ctx);
	if (__prep != NULL)
		__prep(__hdr, (void*)&ctx);
	ffly_err_t exit_code;
	ffly_bci_exec(&ctx, &exit_code);
	printf("exit code: %u\n", exit_code);
	ffly_bci_de_init(&ctx);
}
