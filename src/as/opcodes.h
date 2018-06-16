# ifndef __ffly__opcodes__h
# define __ffly__opcodes__h
# define ous(__o0, __o1, __o2, __o3) \
	((__o0&0xf)|(__o1&0xf)<<4|(__o2&0xf)<<8|(__o3&0xf)<<12)
# define get_ous(__op, __n) \
	(((__op)->ous&(0xf<<(__n*4)))>>(__n*4))
struct ff_as_op {
	char const *name;
	ff_u8_t opcode[8];
	ff_u8_t l;
	// operand output size
	ff_u16_t ous;
};

# define op_\
	&(struct ff_as_op)
# endif /*__ffly__opcodes__h*/
