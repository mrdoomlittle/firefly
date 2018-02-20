# include "types/err_t.h"
# include "linux/unistd.h"

# include "memory/allocr.h"
# include "system/io.h"
# include "system/thread.h"
# include "system/config.h"

/*
extern void*(*ffly_allocp)(mdl_uint_t);
extern void(*ffly_freep)(void*);
extern void*(*ffly_reallocp)(void*, mdl_uint_t);
*/
ffly_err_t ffmain(int, char const**);
void _start(void) {
	int long argc;
	char const **argv;
	__asm__("mov 24(%%rsp), %0\t\n"
			"mov %%rsp, %%rax\n\t"
			"add $32, %%rax\n\t"
			"mov %%rax, %1" : "=r"(argc), "=r"(argv) :);
	ffly_ar_init();
 	ffly_io_init();
/*
	ffly_allocp = ffly_alloc;
	ffly_freep = ffly_free;
	ffly_reallocp = ffly_realloc;
	*/
	/*
	if (argc < 2) {
		ffly_printf("please provide sysconf.\n");
		goto _end;
	}
	ffly_ld_sysconf(*(char const**)(argv+1));
	ffmain(argc, (char const**)argv);
	_end:
	ffly_free_sysconf();
	ffly_thread_cleanup();
	*/
	ffmain(argc, argv);
	ffly_arstat();
	ffly_io_closeup();
    ffly_ar_cleanup();
	exit(0);
}
