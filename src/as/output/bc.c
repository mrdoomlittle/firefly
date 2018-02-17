# include "../as.h"
# include "../../bci.h"
# include "../../ffly_def.h"
# include "../stdio.h"

void oust_addr(ffly_addr_t __addr) {
	oust((mdl_u8_t*)&__addr, sizeof(ffly_addr_t));
}

void emit_exit(insp __ins) {
	oustbyte(*__ins->opcode);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
}

void emit_asb(insp __ins) {
	oustbyte(*__ins->opcode);
	oustbyte(1);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
	oustbyte(*(mdl_u8_t*)__ins->r->p);
}

void emit_asw(insp __ins) {
	oustbyte(*__ins->opcode);
	oustbyte(2);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
	oust_16l(*(mdl_u16_t*)__ins->r->p);
}

void emit_asd(insp __ins) {
	oustbyte(*__ins->opcode);
    oustbyte(4);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
	oust_32l(*(mdl_u32_t*)__ins->r->p);
}

void emit_asq(insp __ins) {
	oustbyte(*__ins->opcode);
    oustbyte(8);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
	oust_64l(*(mdl_u64_t*)__ins->r->p);
}

struct ins *bc[] = {
	&(struct ins){"exit", NULL, emit_exit, NULL, NULL, {_op_exit}},
	&(struct ins){"asb", NULL, emit_asb, NULL, NULL, {_op_as}},
	&(struct ins){"asw", NULL, emit_asw, NULL, NULL, {_op_as}},
	&(struct ins){"asd", NULL, emit_asd, NULL, NULL, {_op_as}},
	&(struct ins){"asq", NULL, emit_asq, NULL, NULL, {_op_as}},
	NULL
};
