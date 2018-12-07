# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/amd64.h"
# include "../../string.h" 

# define _suffix_byte 0x0
# define _suffix_word 0x1
# define _suffix_dword 0x2
# define _suffix_qword 0x3


struct ins {
	char buf[24];
};

typedef struct {
	char const *name;
	ff_u8_t enc;
	ff_u8_t mrm;
	ff_u8_t l;
} reginfo;

static struct ff_as_op *op;
# define regc 14
static reginfo reg[] = {
	{"rax", 0, 0, 8}, {"eax", 0, 0, 4}, {"ax", 0, 0, 2}, {"al", 0, 0, 1},
	{"rbx", 3, 0, 8}, {"ebx", 3, 3, 4}, {"bx", 3, 3, 2}, {"bl", 3, 3, 1},
	{"rcx", 1, 0, 8}, {"ecx", 1, 1, 4}, {"cx", 1, 1, 2}, {"cl", 1, 1, 1},
	{"rbp", 5, 5, 8}, {"rsp", 4, 4, 8}
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

ff_u8_t static imm_sz(ff_u16_t __o) {
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

ff_i8_t static suffix;

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
	if (suffix>0) {
		ff_u8_t i = 0;
		ff_u8_t n = fak_->n;
		ff_u16_t *ip;
		while(i != n) {
			ff_u16_t info = *(ip = fak_->info+i);
			if ((info&_o_imm)>0) {
				*ip = _o_imm8<<suffix;
			} else if ((info&_o_reg)>0) {
				*ip = _o_reg8<<suffix;
			}
			i++;
		}
	}
	locate_op();
	if (!op) {
		printf("failed to locate operation.\n");
		return;
	}
	ff_u8_t buf[64];
	ff_u8_t *p = buf;

	if (fak_->n>0 && !is_flag(op->flags, noprefix)) {
		if (!(getinfo(fak_, 0)^_o_reg16)) {
			*(p++) = 0x66;
		} else if (!(getinfo(fak_, 0)^_o_reg64)) {
			*(p++) = 0x4<<4|1<<3;
			printf("--> v: %x\n", *(p-1));
		}
	}

	ff_u8_t *mod;
	ff_u8_t *opbase;
	memcpy(p, op->opcode, op->l);
	opbase = p;
	p+=op->l;
	*p = 0;
	if (is_flag(op->flags, modrm))
		*(mod = p++) = 0x00;
	void **cur = fak_->p;

	ff_u8_t i = 0;
	ff_u8_t n = fak_->n;

	printf("; ; ; ; ; %s, ", op->name);
	while(i != n) {
		ff_u8_t off = i++;
		ff_u16_t info = getinfo(fak_, off);
		if ((info&_o_imm)>0) {
			printf("%u, ", *(ff_u64_t*)fakget(off));
			ff_u8_t sz;
			if (is_flag(op->flags, ssad))
				sz = ((reginfo*)fakget(off-1))->l;
			else
				sz = imm_sz(getinfo(fak_, off));
			memcpy(p, fakget(off), sz);
			p+=sz;
		} else if ((info&_o_reg)>0) {
			reginfo *reg = (reginfo*)fakget(off);
			printf("%s, ", reg->name);
			if (is_flag(op->flags, modrm)) {
				char const *name = reg->name;
				*mod = *mod<<3|reg->mrm;
			} else {
				*opbase = (*(ff_u8_t*)opbase)+reg->enc;
			}
		} else if ((info&_o_label)>0) {
			labelp la = (labelp)__label;
			ff_uint_t sz = p-buf;
			if (sz>0) {
				fgrowb(curfrag, sz);
				ff_as_plant(curfrag, buf, sz);
				p = buf;
			}
			fix(curfrag, la, _fx_dis, 0x00);
			pf_flags |= FR_FIX;
			ff_as_fdone(curfrag);
			ff_as_fnew(); // new frag
		}
		cur++;
	}
	printf("\n");

	if (is_flag(op->flags, modrm))
		*mod |= op->ext<<3|3<<6;
	ff_uint_t sz = p-buf;
	if (sz>0) {
		// grow frag by X amount
		fgrowb(curfrag, sz);
		ff_as_plant(curfrag, buf, sz);
	}
}

void static*
_getreg(char const *__name) {
	reginfo *cur = reg;
	while(cur != reg+regc) {
		if (!strcmp(cur->name, __name)) {
			printf("matched reg: %s to %s\n", cur->name, __name);
			return cur;
		}
		cur++;
	}
	return NULL;
}

ff_u8_t static
_regsz(void *__reg) {
	return ((reginfo*)__reg)->l;
}

ff_i8_t static
_suffix(ff_u8_t __c) {
	switch(__c) {
		case 'b':
			suffix = _suffix_byte;
		break;
		case 'w':
			suffix = _suffix_word;
		break;
		case 'd':
			suffix = _suffix_qword;
		break;
		case 'q':
			suffix = _suffix_qword;
		break;
		default:
			return (suffix = -1);
	}
	printf("got suffix.\n");
	return 0;
}

ff_uint_t static __n(long long __val) {
	ff_uint_t i;
	i = 0;
	while((__val&(0xffffffffffffffff<<i))>0)
		i++;
	return i;
}


void static
_fixins(struct fix_s *__fx) {
	printf("^^^^^^^^^^^^^^^fix.\n");
	if (__fx->type == _fx_dis) {
		labelp l = (labelp)__fx->arg;
		ff_int_t dis;
		printf("----> fix: %u, %u\n", l->f->m, __fx->f->m);
		dis = ((l->f->adr+l->f->m)+l->foffset)-(__fx->f->adr+__fx->f->m+__fx->f->size);
		printf("dis: %d\n", dis);
		ff_uint_t n;
		n = __n(dis<0?-dis:dis);
		ff_uint_t bs;
		ff_int_t *bsp;
		bsp = &__fx->f->bs;
		bs = ((n+1)+((1<<3)-1))>>3;

		*(ff_i64_t*)__fx->f->data = dis-bs;

		printf("before %u, after %u\n", *bsp, bs);
		if (*bsp != bs)
			fix_flgs |= 0x01;
		*bsp+=(bs-*bsp);
		return;
	}
	fix_flgs = 0x00;
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
	ff_as_suffix = _suffix;
	ff_as_fixins = _fixins;
}
