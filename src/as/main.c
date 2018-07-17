# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "as.h"
# include "../elf.h"
# include "../dep/bzero.h"
# include "../string.h"
# include "../stdio.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../remf.h"
# include "../exec.h"
# include "../opt.h"
# include "../depart.h"
int out, in;

struct ins extern* x86[];
struct ins extern* resin[];
ff_u64_t extern offset;

void ff_as_prepstack(void);
//# define testing
# include "frag.h"
# include "../string.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
# ifdef testing
	/*
		ignore
	*/

	char const s[] = "juhiufuhiufhiuhduihfiuudhifuhdiufhiuidhffshijfhsdofhsojhfosdjhfjdshjfidfjhjfdshjfdshifhsihjfhoshfijsdhfijshdiufhsdiujfhdsihfdhishgfihdshihgfdjdf";
	char buf[sizeof(s)];
	memset(buf, 0, sizeof(s));
	struct frag *f;
	f = ff_as_falloc();

	ff_as_fgrow(f, sizeof(s));
	ff_as_fwrite(f, 0, s, sizeof(s));
	ff_as_fread(f, 0, buf, sizeof(s));
	ffly_printf("buf: %s\n", buf);
# endif

# ifndef testing
	ffoe_prep();
	char const *infile = NULL;
	char const *outfile = NULL;
	char const *format = NULL;
	char const *processor = NULL;
	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);

	infile = ffoptval(ffoe_get("i"));
	outfile = ffoptval(ffoe_get("o"));
	format = ffoptval(ffoe_get("f"));
	processor = ffoptval(ffoe_get("p"));
	ffoe_end();

	if (!infile || !outfile || !format) {
		fprintf(stderr, "missing -o -i, or -f\n");
		return -1;
	}

	printf("dest: %s, src: %s, format: %s\n", outfile, infile, format);

	if (!strcmp(format, "remf")) {
		of = _of_remf;
		offset+=remf_hdrsz;
	} else if (!strcmp(format, "elf")) {
		of = _of_elf;
		offset+=elf64_hdr_size;
	} else if (!strcmp(format, "raw"))
		of = _of_raw;

	if ((in = open(infile, O_RDONLY, 0)) == -1) {
		return -1;
	}

	if ((out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		return -1;
	}

	ff_as_init();

	if (!processor)
		ff_as_resin();
	else if (!strcmp(processor, "amd64"))
		ff_as_amd64();

	if (of == _of_remf)
		ff_as_remf();
	else if (of == _of_elf)
		ff_as_elf();
	
	struct stat st;
	stat(infile, &st);

	// 
	char buf[256];
	char *file = (char*)malloc(st.st_size); 
	char *p = file;
	char *end = p+st.st_size;
	read(in, p, st.st_size);

	ff_as_prepstack();
	ff_as(p, end);
	ff_as_final();

	free(file);
	ff_as_de_init();
	close(in);
	close(out);
# endif
	ff_as_al_cu();
	ffly_depart(NULL);
}
