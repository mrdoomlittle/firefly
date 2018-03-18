# include "../types/err_t.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include <mdlint.h>
# include "../stdio.h"
# include "../exec.h"
/*
	execute raw byte code, no headers or anything just raw code
*/
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		ffly_printf("please provide binary file.\n");
		reterr;
	}

	int fd = open(__argv[1], O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	
	mdl_u8_t *bin = (mdl_u8_t*)malloc(st.st_size);
	read(fd, bin, st.st_size);
	close(fd);

	ffexec(bin, bin+st.st_size, _ffexec_bc, NULL, NULL);
	free(bin);
	retok;
}
