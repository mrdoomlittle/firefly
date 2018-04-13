# include "../mat.h"
# include "../string.h"
# include "../linux/fcntl.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../stdio.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if(__argc<2) {
		fprintf(stderr, "please provide file.\n");
		return -1;
	}

	struct ffly_mat mt;
	int fd;

	fd = open(__argv[1], O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	void *p = malloc(st.st_size);
	read(fd, p, st.st_size);

	printf("filesize: %u\n", st.st_size);
	mt.p = (char const*)p;//"<colour: r:222><pos: padl:2, padt:2>Hello World!</>mrdoomlittle</>";
	mt.end = mt.p+st.st_size;
	ffly_matact(&mt);
	free(p);
	return 0;
}
