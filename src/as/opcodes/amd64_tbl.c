# include "amd64.h"
# include "../../types.h"
# include "../as.h"
struct ff_as_op const *amd64_optab[] = {
	op_{"nop", 3, {0x90}, 1, 0, 0, 0, 0},
	op_{"mov", 4, {0xb0}, 1, 0, 0, ot(_o_reg8, _o_imm8, 0, 0), 2},
	op_{"mov", 4, {0xb8}, 1, ssad, 0, ot(_o_reg16|_o_reg32, _o_imm16|_o_imm32|_o_imm8, 0, 0), 2},
	op_{"mov", 4, {0xb8}, 1, ssad, 0, ot(_o_reg64, _o_imm64|_o_imm8, 0, 0), 2},
	op_{"xor", 3, {0x81}, 1, modrm, 0x6, ot(_o_reg32, _o_imm32, 0, 0), 2},
	op_{"syscall", 7, {0x0f, 0x05}, 2, 0, 0, 0},
	NULL
};
