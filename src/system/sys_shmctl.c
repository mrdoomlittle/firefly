# include "../ffint.h"
# include "../linux/ipc/shm.h"
ff_s32_t shmctl(ff_s32_t __id, ff_s32_t __cmd, struct shmid_ds *__buf) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"mov %2, %%esi\n\t"
			"mov %3, %%rdx\n\t"
			"call __shmctl\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__id), "m"(__cmd), "m"(__buf) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
