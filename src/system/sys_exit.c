# include <mdlint.h>
void ffly_exit(mdl_s32_t __exit_code) {
	__asm__("mov %0, %%edi\n\t"
			"call _ffly_exit" : : "r"(__exit_code));
}
