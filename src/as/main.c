# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "as.h"
# include "../elf.h"
# include "../dep/bzero.h"
# include "../string.h"
# include "../stdio.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../exec.h"
# include "../opt.h"
# include "../depart.h"
int out, in;

struct ins extern* x86[];
struct ins extern* bc[];
ff_u64_t extern offset;

extern void prepstack(void);
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffoe_prep();
	char const *infile = NULL;
	char const *outfile = NULL;
	char const *format = NULL;
	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);

	infile = ffoptval(ffoe_get("i"));
	outfile = ffoptval(ffoe_get("o"));
	format = ffoptval(ffoe_get("f"));
	ffoe_end();

	if (!infile || !outfile || !format) {
		fprintf(stderr, "missing -o -i, or -f\n");
		return -1;
	}

	printf("dest: %s, src: %s, format: %s\n", outfile, infile, format);

	if (!strcmp(format, "ffef")) {
		of = _of_ffef;
		offset+=ffef_hdr_size;
	}

	if ((in = open(infile, O_RDONLY, 0)) == -1) {
		return -1;
	}

	if ((out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1) {
		return -1;
	}

	ffas_init();

	load(bc);

	struct stat st;
	stat(infile, &st);

	char buf[256];
	char *file = (char*)malloc(st.st_size); 
	char *p = file;
	char *end = p+st.st_size;
	read(in, p, st.st_size);

	prepstack();
	assemble(p, end);
	finalize();

	free(file);
	close(in);
	close(out);
	_cleanup();
	ffly_depart(NULL);
}
