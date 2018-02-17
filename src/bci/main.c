# include "../bci.h"
# include "../stdio.h"
# include <mdlint.h>
# include "../linux/unistd.h"
# include "../linux/stat.h"
mdl_u8_t *bin = NULL;
ffly_addr_t static ip = 0;
mdl_u8_t fetch_byte(ffly_off_t __off) {
	return *(bin+ip+__off);
}

void
ip_incr(mdl_uint_t __by) {
	ip+=__by;
}


ffly_addr_t get_ip() {
	return ip;
}

void set_ip(ffly_addr_t __to) {
	ip = __to;
}

struct ffly_bci ctx = {
	.stack_size = 200,
	.fetch_byte = fetch_byte,
	.ip_incr = ip_incr,
	.get_ip = get_ip,
	.set_ip = set_ip
};

# include "../system/err.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		ffly_printf("please provide binary file.\n");
		reterr;
	}

	int fd = open(__argv[1], O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);

	ffly_bci_init(&ctx);

	ffly_err_t exit_code = 0;
	ffly_bci_exec(&ctx, &exit_code);		
	printf("exit code: %u\n", exit_code);

	ffly_bci_de_init(&ctx);
	close(fd);
	retok;
}
