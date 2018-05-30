# ifndef __ffly__amd64__tbl__h
# define __ffly__amd64__tbl__h
# include "../../ffint.h"
# include "../opcodes.h"
# include "../../ffly_def.h"

struct ff_as_op const *amd64_optab[] = {
	op_{"nop", {0x90}, 1},
	op_{"mov", {0x88}, 1},
	op_{"syscall", {0xf0, 0x5}},
	NULL
};
# endif /*__ffly__amd64__tbl__h*/
