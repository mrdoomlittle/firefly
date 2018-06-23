# include "resin_tbl.h"
# include "../../types.h"

struct ff_as_op const *resin_optab[] = {
	op_{"exit", {_resin_op_exit}, 1, ous(0, 0, 0, 0)},
	op_{"asb", {_resin_op_asb}, 1, ous(0, 1, 0, 0)},
	op_{"asw", {_resin_op_asw}, 1, ous(0, 2, 0, 0)},
	op_{"asd", {_resin_op_asd}, 1, ous(0, 4, 0, 0)},
	op_{"asq", {_resin_op_asq}, 1, ous(0, 8, 0, 0)},

	op_{"jmp", {_resin_op_jmp}, 1, ous(0, 0, 0, 0)},

	op_{"stb", {_resin_op_stb}, 1, ous(0, 0, 0, 0)},
	op_{"stw", {_resin_op_stw}, 1, ous(0, 0, 0, 0)},
	op_{"std", {_resin_op_std}, 1, ous(0, 0, 0, 0)},
	op_{"stq", {_resin_op_stq}, 1, ous(0, 0, 0, 0)},

	op_{"ldb", {_resin_op_ldb}, 1, ous(0, 0, 0, 0)},
	op_{"ldw", {_resin_op_ldw}, 1, ous(0, 0, 0, 0)},
	op_{"ldd", {_resin_op_ldd}, 1, ous(0, 0, 0, 0)},
	op_{"ldq", {_resin_op_ldq}, 1, ous(0, 0, 0, 0)},

	op_{"outb", {_resin_op_outb}, 1, ous(0, 0, 0, 0)},
	op_{"outw", {_resin_op_outw}, 1, ous(0, 0, 0, 0)},
	op_{"outd", {_resin_op_outd}, 1, ous(0, 0, 0, 0)},
	op_{"outq", {_resin_op_outq}, 1, ous(0, 0, 0, 0)},

	op_{"movb", {_resin_op_movb}, 1, ous(0, 0, 0, 0)},
	op_{"movw", {_resin_op_movw}, 1, ous(0, 0, 0, 0)},
	op_{"movd", {_resin_op_movd}, 1, ous(0, 0, 0, 0)},
	op_{"movq", {_resin_op_movq}, 1, ous(0, 0, 0, 0)},

	op_{"rin", {_resin_op_rin}, 1, ous(1, sizeof(ff_addr_t), 0, 0)},

	op_{"divb", {_resin_op_divb}, 1, ous(0, 0, 0, 0)},
	op_{"divw", {_resin_op_divw}, 1, ous(0, 0, 0, 0)},
	op_{"divd", {_resin_op_divd}, 1, ous(0, 0, 0, 0)},
	op_{"divq", {_resin_op_divq}, 1, ous(0, 0, 0, 0)},

	op_{"mulb", {_resin_op_mulb}, 1, ous(0, 0, 0, 0)},
	op_{"mulw", {_resin_op_mulw}, 1, ous(0, 0, 0, 0)},
	op_{"muld", {_resin_op_muld}, 1, ous(0, 0, 0, 0)},
	op_{"mulq", {_resin_op_mulq}, 1, ous(0, 0, 0, 0)},

	op_{"subb", {_resin_op_subb}, 1, ous(0, 0, 0, 0)},
	op_{"subw", {_resin_op_subw}, 1, ous(0, 0, 0, 0)},
	op_{"subd", {_resin_op_subd}, 1, ous(0, 0, 0, 0)},
	op_{"subq", {_resin_op_subq}, 1, ous(0, 0, 0, 0)},

	op_{"addb", {_resin_op_addb}, 1, ous(0, 0, 0, 0)},
	op_{"addw", {_resin_op_addw}, 1, ous(0, 0, 0, 0)},
	op_{"addd", {_resin_op_addd}, 1, ous(0, 0, 0, 0)},
	op_{"addq", {_resin_op_addq}, 1, ous(0, 0, 0, 0)},

	op_{"incb", {_resin_op_incb}, 1, ous(0, 0, 0, 0)},
	op_{"incw", {_resin_op_incw}, 1, ous(0, 0, 0, 0)},
	op_{"incd", {_resin_op_incd}, 1, ous(0, 0, 0, 0)},
	op_{"incq", {_resin_op_incq}, 1, ous(0, 0, 0, 0)},

	op_{"decb", {_resin_op_decb}, 1, ous(0, 0, 0, 0)},
	op_{"decw", {_resin_op_decw}, 1, ous(0, 0, 0, 0)},
	op_{"decd", {_resin_op_decd}, 1, ous(0, 0, 0, 0)},
	op_{"decq", {_resin_op_decq}, 1, ous(0, 0, 0, 0)},

	op_{"cmpb", {_resin_op_cmpb}, 1, ous(0, 0, 0, 0)},
	op_{"cmpw", {_resin_op_cmpw}, 1, ous(0, 0, 0, 0)},
	op_{"cmpd", {_resin_op_cmpd}, 1, ous(0, 0, 0, 0)},
	op_{"cmpq", {_resin_op_cmpq}, 1, ous(0, 0, 0, 0)},

	op_{"je", {_resin_op_je}, 1, ous(0, 0, 0, 0)},
	op_{"jne", {_resin_op_jne}, 1, ous(0, 0, 0, 0)},
	op_{"jg", {_resin_op_jg}, 1, ous(0, 0, 0, 0)},
	op_{"jl", {_resin_op_jl}, 1, ous(0, 0, 0, 0)},

	op_{"call", {_resin_op_call}, 1, ous(0, 0, 0, 0)},
	op_{"ret", {_resin_op_ret}, 1, ous(0, 0, 0, 0)},
	NULL
};
