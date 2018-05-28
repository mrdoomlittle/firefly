# ifndef __ffly__opcodes__h
# define __ffly__opcodes__h
struct ff_as_op {
	char const *name;
	ff_u8_t opcode[8];
	ff_u8_t l;
};

# define mkop \
	&(struct ff_as_op)

# include "opcodes/resin_tbl.h"
# endif /*__ffly__opcodes__h*/
