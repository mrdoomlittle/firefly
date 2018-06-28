# ifndef __ffly__opcodes__amd64__h
# define __ffly__opcodes__amd64__h
# include "../../ffint.h"
# include "amd64_tbl.h"
# define ot(__t0, __t1, __t2, __t3) \
	{__t0, __t1, __t2, __t3}
# define get_ot(__op, __n) \
	((__op)->ots[__n])
# define modrm 0x1
// same size as dst
# define ssad 0x2
# define noprefix 0x4
struct ff_as_op {
	char const *name;
	ff_u8_t nme_len;

	ff_u8_t opcode[8];
	// length of opcode(bytes)
	ff_u8_t l;
	ff_u8_t flags;
	ff_u8_t ext;
	ff_u16_t ots[4];
	ff_u8_t oc;
};

# define op_\
	&(struct ff_as_op)
# endif /*__ffly__opcodes__amd64__h*/
