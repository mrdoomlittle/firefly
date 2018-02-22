void ffly_mem_mov128(void *__dst, void *__src) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_mem_mov128" : : "m"(__dst), "m"(__src));
}

void ffly_mem_mov256(void *__dst, void *__src) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_mem_mov256" : : "m"(__dst), "m"(__src));
}

void ffly_mem_mov512(void *__dst, void *__src) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_mem_mov512" : : "m"(__dst), "m"(__src));
}
