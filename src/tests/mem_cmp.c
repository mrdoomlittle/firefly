# include "../data/mem_cmp.h"
# include <time.h>
# include <string.h>
# include <stdio.h>
# include "../system/io.h"
# include "./system/errno.h"
int main(void) {
	ffly_io_init();
	char *a = "abcdefghijkmnopqrstuvwxyz";
	char *b = "abcdefghijkmnopqrstuvwxyz";
	mdl_uint_t len = strlen(a);

	printf("mem_cmp: '%s' ? '%s', bytes: %u\n", a, b, len);
	mdl_u8_t res;
	struct timespec begin, end;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	res = ffly_mem_cmp(a, b, len);

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("mem_cmp, time taken: %uns, both strings %s\n", end.tv_nsec-begin.tv_nsec, res == 1? "match":"don't match");
	ffly_io_closeup();
	return FFLY_FAILURE;
}
