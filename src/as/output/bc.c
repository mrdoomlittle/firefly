# include "../as.h"
# include "../../bci.h"
# include "../../ffly_def.h"
# include "../stdio.h"

void oust_addr(ffly_addr_t __addr) {
	oust((mdl_u8_t*)&__addr, sizeof(ffly_addr_t));
}

void emit_exit(insp __ins) {
	oustbyte(*__ins->opcode);
	symbolp l = __ins->l;	
	oust(l->p, sizeof(ffly_err_t));
}

struct ins *bc[] = {
	&(struct ins){"exit", NULL, emit_exit, NULL, NULL, {_op_exit}},
	NULL
};
