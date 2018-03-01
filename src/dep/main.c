# include "mem_mov.h"
# include "../typesize.h"
# include <mdlint.h>
# include <stdio.h>
# include "str_len.h"
# include "mem_cpy.h"
# include <malloc.h>
# include "mem_dup.h"
# include "str_cpy.h"
# include "bzero.h"
# include "bcopy.h"
# include "str_dup.h"
# include "str_cmp.h"
# include "mem_cmp.h"
void *__ffly_mal() {
	mdl_uint_t __attribute__ ((aligned (8))) bc;
	__asm__("movq 16(%%rbp), %%rax\n\t"
			"movq %%rax, %0" : "=m"(bc) : );
	printf("alloc: %u\n", bc);
	void *ret = malloc(bc);
	printf("%p\n", ret);
	return ret;
}

int main() {
	printf("%d, %d\n", ffly_str_cmp("Hello", "Hellx"), ffly_mem_cmp("hello", "hello", 5));
}
