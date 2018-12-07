# include "amd64.h"
# include "../../types.h"
# include "../as.h"
struct ff_as_op const *amd64_optab[] = {
	op_{"nop", 3, {0x90}, 1, 0, 0, 0, 0},
	op_{"mov", 3, {0xb0}, 1, 0, 0, ot(_o_reg8, _o_imm8, 0, 0), 2},
	op_{"mov", 3, {0xb8}, 1, 0, 0, ot(_o_reg16|_o_reg32|_o_reg64, _o_imm16|_o_imm32|_o_imm64, 0, 0), 2},
	op_{"mov", 3, {0x88}, 1, modrm, 0x0, ot(_o_reg8, _o_reg8, 0, 0), 2},
	op_{"mov", 3, {0x89}, 1, modrm, 0x0, ot(_o_reg16|_o_reg32|_o_reg64, _o_reg16|_o_reg32|_o_reg64, 0, 0), 2},
	op_{"xor", 3, {0x80}, 1, modrm, 0x6, ot(_o_reg8, _o_imm8, 0, 0), 2},
	op_{"xor", 3, {0x81}, 1, modrm, 0x6, ot(_o_reg16|_o_reg32, _o_imm16|_o_imm32, 0, 0), 2},
	op_{"xor", 3, {0x30}, 1, modrm, 0x0, ot(_o_reg8, _o_reg8, 0, 0), 2},
	op_{"xor", 3, {0x31}, 1, modrm, 0x0, ot(_o_reg16|_o_reg32|_o_reg64, _o_reg16|_o_reg32|_o_reg64, 0, 0), 2},
	op_{"pop", 3, {0x58}, 1, noprefix, 0, ot(_o_reg16|_o_reg32|_o_reg64, 0, 0, 0), 1},
	op_{"push", 4, {0x50}, 1, noprefix, 0, ot(_o_reg16|_o_reg32|_o_reg64, 0, 0, 0), 1},
	op_{"push", 4, {0x68}, 1, noprefix, 0, ot(_o_imm16|_o_imm32|_o_imm64, 0, 0, 0), 1},
	op_{"syscall", 7, {0x0f, 0x05}, 2, 0, 0, 0},
	op_{"jmp", 4, {0xeb}, 1, 0, 0, ot(_o_label, 0, 0, 0), 1},
	NULL
};
