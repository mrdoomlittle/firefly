# include <stdio.h>
# include <mdlint.h>
/*
void test() {
	mdl_uint_t no;
	__asm__("movl 16(%%rbp), %%eax\n\t"
			"movl %%eax, %0": "=m"(no));
	printf("%u\n", no);
}
*/
# define rodno(__p) \
    (((((mdl_u64_t)(__p))&0xff)^(((mdl_u64_t)(__p))>>8&0xff)^(((mdl_u64_t)(__p))>>16&0xff)^(((mdl_u64_t)(__p))>>24&0xff)^\
	    (((mdl_u64_t)(__p))>>32&0xff)^(((mdl_u64_t)(__p))>>40&0xff)^(((mdl_u64_t)(__p))>>48&0xff)^(((mdl_u64_t)(__p))>>56&0xff))>>2)
# include <unistd.h>
int main() {
	while(1) {
		void *p = malloc(1);
		printf("%lu, %lu\n", p, rodno(p));
		usleep(100000);
	}
/*
__asm__("push %%rbp\n\t"
			"mov %%rsp, %%rbp\n\t"
			"sub $4, %%rsp\n\t"
			"movl $33, (%%rsp)\n\t"
			"sub $4, %%rsp\n\t"
			"movl %0, (%%rsp)\n\t"
			"call test\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp" : : "r"((mdl_uint_t)21299));
*/
}
