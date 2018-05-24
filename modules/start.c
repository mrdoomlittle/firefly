# include "../src/ffint.h"
# include "../src/system/string.h"
# include "../src/mod/port.h"
# include "../src/mod/ring.h"
# include "../src/system/port.h"
# include "../src/mod/malloc.h"
# include "../src/mod.h"
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
	ff_uint_t shm_id = ffly_stno((char*)*argv);
	ffmod_port_open(shm_id);
	ff_u32_t band = PORT_BAND;
	ffly_port_send(MOD_LISTEN, &band, sizeof(ff_u32_t), ffmod_portno());

	main();
	//pr();
	//pf();
	setmalopt(FF_MAL_O_LOC);

	ff_u8_t exit = 0xff;
	ffly_port_send(PORT_BAND, &exit, 1, ffmod_portno());
	ffmod_port_close();
	__asm__("xorq %rdi, %rdi\n\t"
			"mov $60, %rax\n\t"
			"syscall");
}
