# include "../as.h"
# include "../../ffly_def.h"
# include <stdio.h>
void emit_mov(insp __ins) {
	oustbyte(*__ins->opcode);

}

void emit_movb(insp __ins) {

}

void emit_nop(insp __ins) {

}

void emit_call(insp __ins) {

}

void emit_jmp(insp __ins) {

}

void emit_push(insp __ins) {

}

void emit_pop(insp __ins) {

}

void emit_add(insp __ins) {

}

struct ins *x86[] = {
	&(struct ins){"mov", NULL, emit_mov, {0x88, 0x89}},
	&(struct ins){"movb", NULL, emit_movb},
	&(struct ins){"nop", NULL, emit_nop},
	&(struct ins){"call", NULL, emit_call},
	&(struct ins){"jmp", NULL, emit_jmp},
	&(struct ins){"push", NULL, emit_push},
	&(struct ins){"pop", NULL, emit_pop},
	&(struct ins){"add", NULL, emit_add},
	NULL
};
