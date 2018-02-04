# include <mdlint.h>
mdl_s32_t ffly_fcntl(mdl_u32_t __fd, mdl_s32_t __cmd, mdl_u64_t __arg) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
        "mov %2, %%esi\n\t"
        "mov %3, %%rdx\n\t"
        "call _ffly_fcntl\n\t"
		"mov %%eax, %0" : "=r"(ret) : "r"(__fd), "r"(__cmd), "r"(__arg));
	return ret;
}
