# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/amd64_tbl.h"

void static op_nop(ff_u8_t __op) {
	ff_as_oustbyte(__op);
}

void static op_mov(ff_u8_t __op) {
	ff_as_oustbyte(__op);
}

void static
emit_nop(void) {
	op_nop(*op->opcode);
}

void static
emit_mov(void) {
	op_mov(*op->opcode);
}

static struct berry emit[] = {
	{emit_nop, NULL},
	{emit_mov, NULL}
};

void static
_post(void(*__emit)(void)) {
	__emit();
}

# include "../../dep/str_len.h"
void
ff_as_amd64(void) {
	struct ff_as_op const **cur;
	struct ff_as_op const *op;

	struct berry *ber = emit;
	cur = amd64_optab;
	while(*cur != NULL) {
		op = *(cur++);
		ffly_printf("opname: %s\n", op->name);
		ff_as_hash_put(&env, op->name, ffly_str_len(op->name), ber);
		(ber++)->op = op;
		op++;
	}
	post = _post;
}
