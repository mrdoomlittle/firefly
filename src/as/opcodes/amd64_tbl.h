# ifndef __ffly__amd64__tbl__h
# define __ffly__amd64__tbl__h
# include "../../ffint.h"
# include "../opcodes.h"
# include "../../ffly_def.h"

struct ff_as_op const *amd64_optab[] = {
	op_{"nop", {0x90}, 1, ous(0, 0, 0, 0)},
	op_{"movd", {0xb8}, 1, ous(4, 0, 0, 0)},
	op_{"syscall", {0x0f, 0x05}, 2, ous(0, 0, 0, 0)},
	NULL
};
# endif /*__ffly__amd64__tbl__h*/
