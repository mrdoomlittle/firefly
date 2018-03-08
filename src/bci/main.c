# include "../types/err_t.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include <mdlint.h>
# include "../stdio.h"
# include "../exec.h"
# include "../ffef.h"
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

	ffef_hdrp hdr = (ffef_hdrp)bin;
	ffexec(bin+ffef_hdr_size, hdr->format);

	free(bin);
	retok;
}
