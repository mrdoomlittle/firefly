void* brk(void *__p) {
	void *ret;
    __asm__("movq %1, %%rdi\n\t"
        "call __brk\n\t"
		"movq %%rax, %0": "=m"(ret) : "m"(__p) : "rdi", "rax");
	return ret;
}
