# ifndef __ffly__opcodes__amd64__h
# define __ffly__opcodes__amd64__h
# include "../../ffint.h"
# include "amd64_tbl.h"
# define ot(__t0, __t1, __t2, __t3) \
	{__t0, __t1, __t2, __t3}
# define get_ot(__op, __n) \
	((__op)->ots[__n])
#define modrm 0x01
// same size as dst
#define ssad 0x02
#define noprefix 0x04
#define _d8s	0
#define _d16s	1
#define _d32s	2
#define _d64s	3
#define _d8		0x01
#define _d16	0x02
#define _d32	0x04
#define _d64 	0x08
#define osof16 0x08
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
	ff_u16_t ad;
};

# define op_\
	&(struct ff_as_op)
# endif /*__ffly__opcodes__amd64__h*/
