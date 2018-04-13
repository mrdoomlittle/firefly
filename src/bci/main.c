# include "../types.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../ffint.h"
# include "../stdio.h"
# include "exec.h"
/*
	execute raw byte code, no headers or anything just raw code
*/
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		ffly_printf("please provide binary file.\n");
		reterr;
	}

	int fd = open(__argv[1], O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	
	ff_u8_t *bin = (ff_u8_t*)malloc(st.st_size);
	read(fd, bin, st.st_size);
	close(fd);

	ffbci_exec(bin, bin+st.st_size, NULL, NULL, 0);
	free(bin);
	retok;
}
