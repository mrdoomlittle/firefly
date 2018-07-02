# ifndef __ffly__opcodes__resin__h
# define __ffly__opcodes__resin__h
# include "../../ffint.h"
# include "resin_tbl.h"
# include "../as.h"
/*
	im not including resin.h why?
	because there too much uneeded things
	and to minimize dependencies of files
	as such
*/
# define _resin_op_exit 0x0
# define _resin_op_asb 0x1
# define _resin_op_asw 0x2
# define _resin_op_asd 0x3
# define _resin_op_asq 0x4

# define _resin_op_jmp 0x5

# define _resin_op_stb 0x6
# define _resin_op_stw 0x7
# define _resin_op_std 0x8
# define _resin_op_stq 0x9

# define _resin_op_ldb 0xa
# define _resin_op_ldw 0xb
# define _resin_op_ldd 0xc
# define _resin_op_ldq 0xd

# define _resin_op_outb 0xe
# define _resin_op_outw 0xf
# define _resin_op_outd 0x10
# define _resin_op_outq 0x11

# define _resin_op_movb 0x12
# define _resin_op_movw 0x13
# define _resin_op_movd 0x14
# define _resin_op_movq 0x15

# define _resin_op_rin 0x16

# define _resin_op_divb 0x17
# define _resin_op_divw 0x18
# define _resin_op_divd 0x19
# define _resin_op_divq 0x1a

# define _resin_op_mulb 0x1b
# define _resin_op_mulw 0x1c
# define _resin_op_muld 0x1d
# define _resin_op_mulq 0x1e

# define _resin_op_subb 0x1f
# define _resin_op_subw 0x20
# define _resin_op_subd 0x21
# define _resin_op_subq 0x22

# define _resin_op_addb 0x23
# define _resin_op_addw 0x24
# define _resin_op_addd 0x25
# define _resin_op_addq 0x26

# define _resin_op_incb 0x27
# define _resin_op_incw 0x28
# define _resin_op_incd 0x29
# define _resin_op_incq 0x2a

# define _resin_op_decb 0x2b
# define _resin_op_decw 0x2c
# define _resin_op_decd 0x2d
# define _resin_op_decq 0x2e

# define _resin_op_cmpb 0x2f
# define _resin_op_cmpw 0x30
# define _resin_op_cmpd 0x31
# define _resin_op_cmpq 0x32

# define _resin_op_je 0x33
# define _resin_op_jne 0x34
# define _resin_op_jg 0x35
# define _resin_op_jl 0x36
# define _resin_op_call 0x37
# define _resin_op_ret 0x38

# define _resin_out1br0 0x59
# define _resin_out1wr0 0x5a
# define _resin_out1dr0 0x5b
# define _resin_out1qr0 0x5c

# define _resin_out1br1 0x5d
# define _resin_out1wr1 0x5e
# define _resin_out1dr1 0x5f
# define _resin_out1qr1 0x60

# define _resin_as1br0 0x39
# define _resin_as1wr0 0x3a
# define _resin_as1dr0 0x3b
# define _resin_as1qr0 0x3c

# define _resin_as1br1 0x3d
# define _resin_as1wr1 0x3e
# define _resin_as1dr1 0x3f
# define _resin_as1qr1 0x40

# define _resin_r0r0 0x97
# define _resin_r1r0 0x98
# define _resin_r2r0 0x99
# define _resin_r3r0 0x9a

# define _resin_r0r1 0x9b
# define _resin_r1r1 0x9c
# define _resin_r2r1 0x9d
# define _resin_r3r1 0x9e

# define ot(__t0, __t1, __t2, __t3) \
	{__t0, __t1, __t2, __t3}
# define get_ot(__op, __n) \
	((__op)->ots[__n])
# define ous(__o0, __o1, __o2, __o3) \
	((__o0&0xf)|(__o1&0xf)<<4|(__o2&0xf)<<8|(__o3&0xf)<<12)
# define get_ous(__op, __n) \
	(((__op)->ous&(0xf<<(__n*4)))>>(__n*4))
# define newr 0x1
struct ff_as_op {
	char const *name;

	ff_u8_t opcode[8];
	// length of opcode(bytes)
	ff_u8_t l;
	// operand output size
	ff_u16_t ous;
	ff_u16_t ots[4];
	ff_u8_t flags;
};

# define op_\
	&(struct ff_as_op)
# endif /*__ffly__opcodes__resin__h*/
