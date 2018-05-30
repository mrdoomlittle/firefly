# ifndef __ffly__opcodes__h
# define __ffly__opcodes__h
struct ff_as_op {
	char const *name;
	ff_u8_t opcode[8];
	ff_u8_t l;
};

# define op_\
	&(struct ff_as_op)
# endif /*__ffly__opcodes__h*/
