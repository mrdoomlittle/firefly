# include "../opt.h"
# include "../system/string.h"
# include "../ffly_def.h"
# include "../rdm.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../depart.h"
# define MIN 2048
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("please provide binfile.\n");
		reterr;
	}

	ff_u64_t beg = 0, size = 0;
	if (__argc>2) {
		struct ffpcll pcl;
		ffoe_prep();
		pcl.cur = __argv+2;
		pcl.end = __argv+__argc;
		ffoe(ffoe_pcll, (void*)&pcl);
		beg = ffly_stno(ffoptval(ffoe_get("b")));
		size = ffly_stno(ffoptval(ffoe_get("s")));
		ffoe_end();
	}

	char const *bin = __argv[1];
	int fd;
	if ((fd = open(bin, O_RDONLY, 0)) < 0) {
		fprintf(stderr, "failed to open file\n");
		reterr;
	}

	struct stat st;
	stat(bin, &st);
	if (!size) {
		size = st.st_size;
		if (beg>0)
			size-=beg;
	}
	/*
		if file size is over the MIN then use rdmf as it will load from file bit by bit.
	*/
	if (st.st_size<MIN) {
		ff_u8_t *p = (ff_u8_t*)malloc(size);
		if (beg>0)
			lseek(fd, beg, SEEK_SET);
		read(fd, p, size);
		close(fd);

		printf("beg, %u, size, %u\n", beg, size);
		ffly_rdmp(p, size);
		free(p);
	} else {
		printf("beg, %u, size, %u\n", beg, size);
		ffly_rdmf(bin, beg, size);
	}

	ffly_depart(NULL);
	retok;
}
