# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/amd64_tbl.h"
# include "../../string.h" 
typedef struct {
	char const *name;
	ff_u8_t enc;
} reginfo;

# define regc 3
static reginfo reg[] = {
	{"eax", 0}, {"ebx", 3}, {"ecx", 1}
};

void static
_post(void) {
	ff_u8_t buf[26];
	ff_u8_t *p = buf;

	memcpy(p, op->opcode, op->l);
	p+=op->l;
	void **cur = fak_->p;
	while(*cur != NULL) {
		ff_u8_t off = cur-fak_->p;
		switch(getinfo(fak_, off)) {
			case _o_int:
				memcpy(p, fakget(off), get_ous(op, off));
				p+=get_ous(op, off);
			break;
			case _o_reg:
				*(ff_u8_t*)buf = (*(ff_u8_t*)buf)+((reginfo*)fakget(off))->enc;
			break;
		}
		cur++;
	}
	ff_as_oust(buf, p-buf);
}

void static*
_getreg(char const *__name) {
	reginfo *cur = reg;
	while(cur != reg+regc) {
		if (!strcmp(cur->name, __name))
			return cur;
		cur++;
	}
	return NULL;
}

# include "../../dep/str_len.h"
void
ff_as_amd64(void) {
	struct ff_as_op const **cur;
	struct ff_as_op const *op;

	cur = amd64_optab;
	while(*cur != NULL) {
		op = *(cur++);
		ffly_printf("opname: %s\n", op->name);
		ff_as_hash_put(&env, op->name, ffly_str_len(op->name), op);
		op++;
	}
	post = _post;
	ff_as_getreg = _getreg;
}
