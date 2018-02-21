# include "types/err_t.h"
# include "linux/unistd.h"

# include "memory/allocr.h"
# include "system/io.h"
# include "system/thread.h"
# include "system/config.h"
# include "arcs.h"
# include "memory/alloca.h"
/*
extern void*(*ffly_allocp)(mdl_uint_t);
extern void(*ffly_freep)(void*);
extern void*(*ffly_reallocp)(void*, mdl_uint_t);
*/
ffly_err_t ffmain(int, char const**);
char const static *by = "mrdoomlittle";
# include "data/str_cpy.h"
void _start(void) {
	int long argc;
	char const **argv;
	__asm__("mov 8(%%rbp), %0\t\n"
			"mov %%rbp, %%rax\n\t"
			"add $16, %%rax\n\t"
			"mov %%rax, %1" : "=r"(argc), "=r"(argv) :);
	void *frame;
	ffly_ar_init();
	frame = ffly_frame();
	void **p = ffly_alloca(sizeof(void*), NULL);
 	ffly_io_init();

	if (argc < 2) {
		//ffly_printf("please provide sysconf.\n");
		//goto _end;
	}

	//ffly_ld_sysconf(argv[1]);
	ffly_arcs_init();

	*p = (void*)by;
	ffly_arcs_creatarc("info");
	ffly_arcs_tun("info");
	ffly_arcs_creatrec("created-by", p, _ffly_rec_def, 0);
	ffly_arcs_bk();

	ffmain(argc, argv);
	ffly_printf("\n\n");

	//ffly_free_sysconf();
	_end:
	ffly_arstat();
	ffly_arcs_de_init();
	ffly_io_closeup();
	ffly_collapse(frame);
	ffly_alloca_cleanup();
    ffly_ar_cleanup();
	exit(0);
}
