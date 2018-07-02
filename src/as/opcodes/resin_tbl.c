# include "resin_tbl.h"
# include "../../types.h"
/*
	op = opcode+(oplen*size)
*/
struct ff_as_op const *resin_optab[] = {
	op_{"exit", 4, {_resin_op_exit, _resin_exit2r0, _resin_exit2r1}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), 0, {1, 2}, 1},
	op_{"exit", 4, {_resin_exit1}, 1, ous(0, 0, 0, 0), ot(_o_imm8, 0, 0, 0), 0, {0, 0}, 1},

	op_{"st", 2, {_resin_op_stb, _resin_op_stw, _resin_op_std, _resin_op_stq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"ld", 2, {_resin_op_ldb, _resin_op_ldw, _resin_op_ldd, _resin_op_ldq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"mov", 3, {_resin_op_movb, _resin_op_movw, _resin_op_movd, _resin_op_movq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},

	op_{"rin", 3, {_resin_op_rin}, 1, ous(1, sizeof(ff_addr_t), 0, 0), ot(_o_imm8, _o_reg64|_o_imm, 0, 0), 0, {0, 0}, 2},

	op_{"div", 3, {_resin_op_divb, _resin_op_divw, _resin_op_divd, _resin_op_divq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"mul", 3, {_resin_op_mulb, _resin_op_mulw, _resin_op_muld, _resin_op_mulq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"sub", 3, {_resin_op_subb, _resin_op_subw, _resin_op_subd, _resin_op_subq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"add", 3, {_resin_op_addb, _resin_op_addw, _resin_op_addd, _resin_op_addq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},
	op_{"inc", 3, {_resin_op_incb, _resin_op_incw, _resin_op_incd, _resin_op_incq}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), asto, {0, 0}, 1},
	op_{"dec", 3, {_resin_op_decb, _resin_op_decw, _resin_op_decd, _resin_op_decq}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), asto, {0, 0}, 1},

	op_{"cmp", 3, {_resin_op_cmpb, _resin_op_cmpw, _resin_op_cmpd, _resin_op_cmpq}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_reg, 0, 0), asto, {0, 0}, 2},

	op_{"je", 2, {_resin_op_je}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},
	op_{"jne", 3, {_resin_op_jne}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},
	op_{"jg", 2, {_resin_op_jg}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},
	op_{"jl", 2, {_resin_op_jl}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},

	op_{"jmp", 3, {_resin_op_jmp}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},
	op_{"call", 4, {_resin_op_call}, 1, ous(0, 0, 0, 0), ot(_o_label, 0, 0, 0), 0, {0, 0}, 1},
	op_{"ret", 3, {_resin_op_ret}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},

	op_{"as", 2, {_resin_op_asb, _resin_op_asw, _resin_op_asd, _resin_op_asq,
		_resin_as1br0, _resin_as1wr0, _resin_as1dr0, _resin_as1qr0,
		_resin_as1br1, _resin_as1wr1, _resin_as1dr1, _resin_as1qr1}, 1, ous(0, 0, 0, 0), ot(_o_reg, _o_imm, 0, 0), asto, {4, 8}, 2},
	op_{"out", 3, {_resin_op_outb, _resin_op_outw, _resin_op_outd, _resin_op_outq,
		_resin_out1br0, _resin_out1wr0, _resin_out1dr0, _resin_out1qr0,	
		_resin_out1br1, _resin_out1wr1, _resin_out1dr1, _resin_out1qr1}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), asto, {4, 8}, 1},
	op_{"push", 4, {_resin_push1br0, _resin_push1wr0, _resin_push1dr0, _resin_push1qr0,
		_resin_push1br1, _resin_push1wr1, _resin_push1dr1, _resin_push1qr1}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), asto, {0, 4}, 1},
	op_{"pop", 3, {_resin_pop1br0, _resin_pop1wr0, _resin_pop1dr0, _resin_pop1qr0,
		_resin_pop1br1, _resin_pop1wr1, _resin_pop1dr1, _resin_pop1qr1}, 1, ous(0, 0, 0, 0), ot(_o_reg, 0, 0, 0), asto, {0, 4}, 1},

	op_{"r0r0", 4, {_resin_r0r0}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r1r0", 4, {_resin_r1r0}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r2r0", 4, {_resin_r2r0}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r3r0", 4, {_resin_r3r0}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r0r1", 4, {_resin_r0r1}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r1r1", 4, {_resin_r1r1}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r2r1", 4, {_resin_r2r1}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	op_{"r3r1", 4, {_resin_r3r1}, 1, ous(0, 0, 0, 0), ot(0, 0, 0, 0), 0, {0, 0}, 0},
	NULL
};
