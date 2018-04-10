# include "../opt.h"
# include "../system/string.h"
# include "../ffly_def.h"
# include "../bcd.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../depart.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("please provide binfile.\n");
		reterr;
	}

	mdl_u64_t beg = 0, size = 0;
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

	mdl_u8_t *p = (mdl_u8_t*)malloc(size);
	if (beg>0)
		lseek(fd, beg, SEEK_SET);
	read(fd, p, size);
	close(fd);

	printf("beg, %u, size, %u\n", beg, size);
	ffly_bcd(p, p+size);
	free(p);
	ffly_depart(NULL);
	retok;
}
