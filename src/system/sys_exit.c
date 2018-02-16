# include <mdlint.h>
void exit(mdl_s32_t __exit_code) {
	__asm__("mov %0, %%edi\n\t"
			"call _exit" : : "m"(__exit_code));
}
