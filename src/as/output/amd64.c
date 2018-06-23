# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/amd64.h"
# include "../../string.h" 
typedef struct {
	char const *name;
	ff_u8_t enc;
	ff_u8_t mrm;
	ff_u8_t l;
} reginfo;

static struct ff_as_op *op;
# define regc 6
static reginfo reg[] = {
	{"eax", 0, 0|3<<6, 4}, {"ebx", 3, 3|3<<6, 4}, {"ecx", 1, 1|3<<6, 4},
	{"al", 0, 0|3<<6, 1}, {"ax", 0, 0|3<<6, 2}, {"rax", 0, 0, 8}
};

char const *prot(ff_u16_t __ot) {
	switch(__ot) {
		case _o_reg8:
			return "reg8";
		case _o_reg16:
			return "reg16";
		case _o_reg32:
			return "reg32";
		case _o_reg64:
			return "reg64";
		case _o_imm8:
			return "imm8";
		case _o_imm16:
			return "imm16";
		case _o_imm32:
			return "imm32";
		case _o_imm64:
			return "imm64";
	}
	return "unknown.";
}

ff_u8_t imm_sz(ff_u16_t __o) {
	switch(__o) {
		case _o_imm8:
			return 1;
		case _o_imm16:
			return 2;
		case _o_imm32:
			return 4;
		case _o_imm64:
			return 8;
	}
	return 0;
}

void static
locate_op(void) {
	while(*(--op_tray) != NULL) {
		op = (struct ff_as_op*)*op_tray;
		printf("%s, %u == %u\n", op->name, op->oc, fak_->n);
		if (op->oc == fak_->n) {
			if (!fak_->n)
				return;
			ff_u8_t o = 0;
			while(o != fak_->n) {
				printf("O: %s\n", prot(getinfo(fak_, o)));
				if (!(getinfo(fak_, o)&get_ot(op, o)))
					break;
				o++;
			}
			if (o == fak_->n)
				return;
		}
	}
	op = NULL;
}

void static
_post(void) {
	locate_op();
	if (!op) {
		printf("error.\n");
		return;
	}
	ff_u8_t buf[64];
	ff_u8_t *p = buf;

	if (fak_->n>0) {
		if (!(getinfo(fak_, 0)^_o_reg16)) {
			*(p++) = 0x66;
		} else if (!(getinfo(fak_, 0)^_o_reg64)) {
			*(p++) = 0x4<<4|1<<3;
		}
	}

	memcpy(p, op->opcode, op->l);
	p+=op->l;
	*p = 0;
	if (is_flag(op->flags, modrm))
		p++;
	void **cur = fak_->p;
	while(*cur != NULL) {
		ff_u8_t off = cur-fak_->p;
		switch(getinfo(fak_, off)) {
			case _o_imm8: case _o_imm16: case _o_imm32: case _o_imm64: {
				ff_u8_t sz;
				if (is_flag(op->flags, ssad))
					sz = ((reginfo*)fakget(off-1))->l;
				else
					sz = imm_sz(getinfo(fak_, off));
				memcpy(p, fakget(off), sz);
				p+=sz;
				break;
			}
			case _o_reg8: case _o_reg16: case _o_reg32: case _o_reg64: {
				reginfo *reg = (reginfo*)fakget(off);
				if (is_flag(op->flags, modrm)) {
					char const *name = reg->name;
					buf[1] |= (op->ext<<3)|reg->mrm;
				} else
					*(ff_u8_t*)buf = (*(ff_u8_t*)buf)+reg->enc;
				break;
			}
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

ff_u8_t static
_regsz(void *__reg) {
	return ((reginfo*)__reg)->l;
}

# include "../../dep/str_len.h"
void
ff_as_amd64(void) {
	struct ff_as_op const **cur;
	struct ff_as_op const *op;	
	cur = amd64_optab;
	void **tray;
	while(*cur != NULL) {
		op = *(cur++);
		ffly_printf("opname: %s\n", op->name);
		if ((cur-1)>amd64_optab) {
			if (!strcmp(op->name, (*(cur-2))->name)) {
				goto _sk;		
			}
		}
		tray = (void**)ff_as_hash_get(&env, op->name, op->nme_len);
		if (!tray) {
			tray = (void**)ff_as_al(20*sizeof(void*));
			*(tray++) = NULL;
		}
	_sk:
		*(tray++) = op;
		ff_as_hash_put(&env, op->name, ffly_str_len(op->name), tray);
		op++;
	}
	post = _post;
	ff_as_getreg = _getreg;
	ff_as_regsz = _regsz;
}
