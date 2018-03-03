# include <mdlint.h> 
# include "../linux/ipc/shm.h"
mdl_s32_t shmctl(mdl_s32_t __id, mdl_s32_t __cmd, struct shmid_ds *__buf) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
			"mov %2, %%esi\n\t"
			"mov %3, %%rdx\n\t"
			"call _shmctl\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__id), "m"(__cmd), "m"(__buf) : "edi", "esi", "rdx", "rax");
	return ret;
}
