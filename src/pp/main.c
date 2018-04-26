# include "../types.h"
# include "pp.h"
# include "../linux/unistd.h"
# include "../string.h"
# include "../system/err.h"
# include "../malloc.h"
# include "../linux/stat.h"
# include "../linux/fcntl.h"
# include "../stdio.h"
# include "../system/util/hash.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const **arg = __argv;
	char const **last = arg+__argc;

	char const *s = NULL, *d = NULL;
	arg++;
	prep();
	while(arg != last) {
		if (!strcmp(*arg, "-s"))
			s = *(++arg);
		else if (!strcmp(*arg, "-d"))
			d = *(++arg);
		else if (!strcmp(*arg, "-D")) {
			char const *name = *(++arg);
			printf("define, %s\n", name);
			define(name, ffly_hash(name, strlen(name)), NULL, NULL);
		}
		arg++;
	}

	if (!s || !d) {
		printf("please provide dest and src files.\n");
		reterr;
	}

	if ((src = open(s, O_RDONLY, 0)) == -1) {
		reterr;
	}
	
	ff_u8_t *bed;

	struct stat st;
	fstat(src, &st);
	p = (ff_u8_t*)malloc(st.st_size);
	bed = p;
	read(src, p, st.st_size);
	end = p+st.st_size;
	close(src);

	if ((dst = open(d, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		// error
	}

	prossess();
	flushbuf();

	close(dst);
	free(bed);
	finish();
	retok;
}
