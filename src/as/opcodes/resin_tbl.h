# ifndef __ffly__resin__tbl__h
# define __ffly__resin__tbl__h
# include "../../ffint.h"
# include "../opcodes.h"
# include "../../ffly_def.h"

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
struct ff_as_op const *resin_optab[] = {
	mkop{"exit", {_resin_op_exit}, 1},
	mkop{"asb", {_resin_op_asb}, 1},
	mkop{"asw", {_resin_op_asw}, 1},
	mkop{"asd", {_resin_op_asd}, 1},
	mkop{"asq", {_resin_op_asq}, 1},

	mkop{"jmp", {_resin_op_jmp}, 1},

	mkop{"stb", {_resin_op_stb}, 1},
	mkop{"stw", {_resin_op_stw}, 1},
	mkop{"std", {_resin_op_std}, 1},
	mkop{"stq", {_resin_op_stq}, 1},

	mkop{"ldb", {_resin_op_ldb}, 1},
	mkop{"ldw", {_resin_op_ldw}, 1},
	mkop{"ldd", {_resin_op_ldd}, 1},
	mkop{"ldq", {_resin_op_ldq}, 1},

	mkop{"outb", {_resin_op_outb}, 1},
	mkop{"outw", {_resin_op_outw}, 1},
	mkop{"outd", {_resin_op_outd}, 1},
	mkop{"outq", {_resin_op_outq}, 1},

	mkop{"movb", {_resin_op_movb}, 1},
	mkop{"movw", {_resin_op_movw}, 1},
	mkop{"movd", {_resin_op_movd}, 1},
	mkop{"movq", {_resin_op_movq}, 1},

	mkop{"rin", {_resin_op_rin}, 1},

	mkop{"divb", {_resin_op_divb}, 1},
	mkop{"divw", {_resin_op_divw}, 1},
	mkop{"divd", {_resin_op_divd}, 1},
	mkop{"divq", {_resin_op_divq}, 1},

	mkop{"mulb", {_resin_op_mulb}, 1},
	mkop{"mulw", {_resin_op_mulw}, 1},
	mkop{"muld", {_resin_op_muld}, 1},
	mkop{"mulq", {_resin_op_mulq}, 1},

	mkop{"subb", {_resin_op_subb}, 1},
	mkop{"subw", {_resin_op_subw}, 1},
	mkop{"subd", {_resin_op_subd}, 1},
	mkop{"subq", {_resin_op_subq}, 1},

	mkop{"addb", {_resin_op_addb}, 1},
	mkop{"addw", {_resin_op_addw}, 1},
	mkop{"addd", {_resin_op_addd}, 1},
	mkop{"addq", {_resin_op_addq}, 1},

	mkop{"incb", {_resin_op_incb}, 1},
	mkop{"incw", {_resin_op_incw}, 1},
	mkop{"incd", {_resin_op_incd}, 1},
	mkop{"incq", {_resin_op_incq}, 1},

	mkop{"decb", {_resin_op_decb}, 1},
	mkop{"decw", {_resin_op_decw}, 1},
	mkop{"decd", {_resin_op_decd}, 1},
	mkop{"decq", {_resin_op_decq}, 1},

	mkop{"cmpb", {_resin_op_cmpb}, 1},
	mkop{"cmpw", {_resin_op_cmpw}, 1},
	mkop{"cmpd", {_resin_op_cmpd}, 1},
	mkop{"cmpq", {_resin_op_cmpq}, 1},

	mkop{"je", {_resin_op_je}, 1},
	mkop{"jne", {_resin_op_jne}, 1},
	mkop{"jg", {_resin_op_jg}, 1},
	mkop{"jl", {_resin_op_jl}, 1},

	mkop{"call", {_resin_op_call}, 1},
	mkop{"ret", {_resin_op_ret}, 1},
	NULL
};

# endif /*__ffly__resin__tbl__h*/
