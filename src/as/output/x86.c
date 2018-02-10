# include "../as.h"
# include "../../ffly_def.h"
# include <stdio.h>
void emit_nop(insp __ins) {
	oustbyte(*__ins->opcode);
}

struct ins *x86[] = {
	&(struct ins){"mov", NULL, emit_nop},
	&(struct ins){"movb", NULL, NULL, {0x88}},
	&(struct ins){"nop", NULL, emit_nop, {0x90}},
	&(struct ins){"call", NULL, emit_nop},
	&(struct ins){"jmp", NULL, emit_nop},
	NULL
};
