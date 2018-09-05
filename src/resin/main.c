# include "../types.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../ffint.h"
# include "../stdio.h"
# include "../string.h"
# include "exec.h"
/*
	execute raw byte code, no headers or anything just raw code
*/

ff_u8_t static *bin;
void static
get(ff_uint_t __from, ff_uint_t __size, void *__buf) {
	memcpy(__buf, bin+__from, __size);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		ffly_printf("please provide binary file.\n");
		reterr;
	}

	int fd = open(__argv[1], O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	
	bin = (ff_u8_t*)malloc(st.st_size);
	read(fd, bin, st.st_size);
	close(fd);

	ffres_exec(get, st.st_size, NULL, NULL, 0);
	free(bin);
	retok;
}
