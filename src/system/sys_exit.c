# include <mdlint.h>
void ffly_exit(int __exit_code) {
	__asm__("mov %0, %%rdi\n\t"
			"call _ffly_exit" : : "r"((mdl_u64_t)__exit_code));
}
