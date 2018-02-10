# include <unistd.h>
# include <fcntl.h>
# include "as.h"
# include "../elf.h"
# include "../data/bzero.h"
# include "../data/str_cmp.h"
# include <stdio.h>
# include <sys/stat.h>
# include <malloc.h>
int out, in;

struct ins extern* x86[];
mdl_u64_t extern offset;

int main(int __argc, char const *__argv[]) {
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
		arg++;
	}

	if (!infile || !outfile) {
		fprintf(stderr, "missing -o or -i\n");
		return -1;
	}

	if ((in = open(infile, O_RDONLY)) == -1) {
		return -1;
	}

	hash_init(&symbols);
	hash_init(&globl);

	if ((out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1) {
		return -1;
	}

	load(x86);
	struct stat st;
	stat(infile, &st);

	char buf[256];
	char *file = (char*)malloc(st.st_size); 
	char *p = file;
	char *end = p+st.st_size;
	read(in, p, st.st_size);
	assemble(p, end);

	free(file);
	close(in);
	close(out);
	_cleanup();
}
