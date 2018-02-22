# include "mem_mov.h"
# include "../typesize.h"
# include <mdlint.h>
# include <stdio.h>
# include "str_len.h"
# include "mem_cpy.h"
int main() {
	//mdl_u8_t dst[__256__], src[__256__];
	//ffly_mem_mov256(dst, src);
	char buf[200];
	ffly_mem_cpy(buf, "Hello\n", ffly_str_len("hello\n")+1);
	printf("%s", buf);
}
