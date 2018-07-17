# include "../ffint.h"
# include "../types.h"
# include "../signal.h"
# include "../dep/bzero.h"
# include "../br.h"
# include "../ffly_def.h"
# include "../linux/fcntl.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../brick.h"
# include "../system/io.h"
static struct ffly_br br;

void static sig(int __sig) {
	ffly_br_shutdown(&br);
}

int static fd;
ff_uint_t static sz;

ff_uint_t static bsz;
void static
bread(long __off, void *__p, ff_u8_t __sz) {
	ff_uint_t left;
	ff_uint_t size;

	left = sz-__off;

	size = left<bsz?left:bsz;

	pread(fd, __p, size, __off);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct sigaction sa;
	ffly_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);

	fd = open("test.txt", O_RDONLY, 0);

	if (fd == -1)
		return;
	struct stat st;
	fstat(fd, &st);

	sz = st.st_size;
	ffly_printf("filesize: %u-bytes\n", sz);
	ff_uint_t n;

	bsz = 1<<_ff_brick_32;
	n = (sz>>_ff_brick_32)+((sz&((~(ff_u64_t)0)>>(64-_ff_brick_32)))>0);
	ffly_printf("bricks: %u\n", n);
	ff_uint_t i = 0;

	ffly_br_prep(&br, _ff_brick_32, n);
	while(i != n) {
		ffly_printf("%u:\n", i);
		ffly_br_put(&br, ffly_brick_new(_ff_brick_32, bread, NULL, NULL, i*bsz), i);
		i++;
	}

	ffly_br_open(&br);
	ffly_br_start(&br);
	ffly_br_close(&br);
	ffly_br_free(&br);
	close(fd);
	ffly_brick_cleanup();
}
