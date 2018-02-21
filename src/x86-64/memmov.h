# ifndef __ffly__memmov__h
# define __ffly__memmov__h
void __inline__
ffly_memmov128(void *__src, void *__dst) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_memmov128" : : "m"(__dst), "m"(__src));
}
void __inline__
ffly_memmov256(void *__src, void *__dst) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_memmov256" : : "m"(__dst), "m"(__src));
}
void __inline__
ffly_memmov512(void *__src, void *__dst) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_memmov512" : : "m"(__dst), "m"(__src));
}
# endif /*__ffly__memmov__h*/
