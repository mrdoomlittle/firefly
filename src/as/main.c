# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "as.h"
# include "../elf.h"
# include "../data/bzero.h"
# include "../data/str_cmp.h"
# include "../stdio.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../exec.h"
int out, in;

struct ins extern* x86[];
struct ins extern* bc[];
mdl_u64_t extern offset;
extern mdl_u8_t format;

extern void prepstack(void);
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	char const *infile = NULL;
	char const *outfile = NULL;
	char const **arg = __argv;
	char const **last = arg+__argc;
	arg++;

	while(arg != last) {
		if (!ffly_str_cmp(*arg, "-o"))
			outfile = *(++arg);
		else if (!ffly_str_cmp(*arg, "-i"))
			infile = *(++arg);
		else if (!ffly_str_cmp(*arg, "-f")) {
			char const *s = *(++arg);
			if (!ffly_str_cmp(s, "bc")) {
				format = _of_bc;
				printf("format bc.\n");
			}
		}
		arg++;
	}

	if (!infile || !outfile) {
		fprintf(stderr, "missing -o or -i\n");
		return -1;
	}

	printf("dest: %s, src: %s\n", outfile, infile);

	if ((in = open(infile, O_RDONLY, 0)) == -1) {
		return -1;
	}

	if ((out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1) {
		return -1;
	}

	if (format == _of_bc) {
		struct ffef_hdr hdr = {
			.format = _ffexec_bc
		};
		write(out, &hdr, ffef_hdr_size);
		offset+=ffef_hdr_size;
	}

	ffas_init();

	load(x86);

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
}
