# include <stdio.h>
# include <mdlint.h>
void test() {
	mdl_uint_t no;
	__asm__("movl 16(%%rbp), %%eax\n\t"
			"movl %%eax, %0": "=m"(no));
	printf("%u\n", no);
}

int main() {
	__asm__("push %%rbp\n\t"
			"mov %%rsp, %%rbp\n\t"
			"sub $4, %%rsp\n\t"
			"movl $33, (%%rsp)\n\t"
			"sub $4, %%rsp\n\t"
			"movl %0, (%%rsp)\n\t"
			"call test\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp" : : "r"((mdl_uint_t)21299));
}
