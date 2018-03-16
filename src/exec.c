# include "exec.h"
void ffexec(void *__p, mdl_u8_t __format) {
	switch(__format) {
		case _ffexec_bc:
			ffbci_exec(__p);	
		break;
	}
}

# include "linux/unistd.h"
# include "linux/stat.h"
# include "linux/fcntl.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "ffef.h"
void ffexecf(char const *__file) {
	int fd = open(__file, O_RDONLY, 0);

	struct stat st;
	fstat(fd, &st);
	mdl_u8_t *p = (mdl_u8_t*)__ffly_mem_alloc(st.st_size);

	read(fd, p, st.st_size);

	ffef_hdrp hdr = (ffef_hdrp)p;	
	ffexec(p+ffef_hdr_size, hdr->format);
	__ffly_mem_free(p);
	close(fd);
}
