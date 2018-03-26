# include <mdlint.h>
# include "../src/system/string.h"
# include "../src/mod/pipe.h"
# include "../src/system/pipe.h"
# include "../src/mod/malloc.h"
void pr();
void pf();
void main(void);
void _start(void) {
	int long argc;
	char const **argv;
	__asm__("mov 8(%%rbp), %0\t\n"
			"mov %%rbp, %%rax\n\t"
			"add $16, %%rax\n\t"
			"mov %%rax, %1" : "=r"(argc), "=r"(argv) : : "rax");
	mdl_uint_t shm_id = ffly_stno((char*)*argv);
	ffmod_pipe_open(shm_id);
	ffly_pipe_connect(ffmod_pipeno());
	main();
	pr();
	pf();
	setmalopt(FF_MAL_O_LOC);
	ffly_pipe_wr8l(0xff, ffmod_pipeno());
	ffmod_pipe_close();
	__asm__("xorq %rdi, %rdi\n\t"
			"mov $60, %rax\n\t"
			"syscall");
}
