# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/resin.h"
# include "../../string.h"
# define _suffix_byte 0x0
# define _suffix_word 0x1
# define _suffix_dword 0x2
# define _suffix_qword 0x3

labelp extern curlabel;
// bed of stack
ff_uint_t static bed = 0;

typedef struct {
	char const *name;
	ff_u8_t l;
	ff_uint_t addr;
	ff_u8_t flags;
	ff_u8_t start;
} reginfo;

static struct ff_as_op *op;
void static *p0, *p1, *p2, *p4;

# define sp_rg 0x0
# define bp_rg 0x1

# define rax_rg 0x2
# define eax_rg 0x3
# define ax_rg 0x4
# define al_rg 0x5

# define rlx_rg 0x6
# define elx_rg 0x7
# define lx_rg 0x8
# define ll_rg 0x9

# define rel_rg 0xa
# define ael_rg 0xb
# define el_rg 0xc
# define ae_rg 0xd

# define xes_rg 0xe 
# define els_rg 0xf
# define ls_rg 0x10
# define xs_rg 0x11
# define r0_rg 0x12
# define r1_rg 0x13
void static
oust_addr(ff_addr_t __addr) {
	ff_as_oust((ff_u8_t*)&__addr, sizeof(ff_addr_t));
}

# define reg_spl 0x1
reginfo reg[] = {
	{"sp", 8, 0, 0},  {"bp", 8, 8, 0},
	{"rax", 8, 16, 0},	{"eax", 4, 16, 0},	{"ax", 2, 16, 0},	{"al", 1, 16, 0},
	{"rlx", 8, 24, 0},	{"elx", 4, 24, 0},	{"lx", 2, 24, 0},	{"ll", 1, 24, 0},
	{"rel", 8, 32, 0},	{"ael", 4, 32, 0},	{"el", 2, 32, 0},	{"ae", 1, 32, 0},
	{"xes", 8, 40, 0}, {"els", 4, 40, 0}, {"ls", 2, 40, 0},  {"xs", 1, 40, 0},
	{"r0", 8, 0, reg_spl, 0}, {"r1", 8, 0, reg_spl, 1},
};

/*
	1, 2, 4, 8, 16, 32, 64, 128
	r, a, x, e, l,	s,	b,	p
	   0,				1
*/

/*
	rax	=	00011111	= 248
	eax	=	10001111	= 241
	ax	=	10011111	= 249
	al	=	10110111	= 237
	rlx	=	01010111	= 234	
	elx	=	11000111	= 227
	lx	=	11010111	= 235
	ll	=	11111111	= 255
	rel	=	01100111	= 230
	ael	=	10100111	= 229
	el	=	11100111	= 231
	ae	=	10101111	= 245
	xes	=				= 211
	els	=				= 199
	ls	=				= 207
	xs	=				= 219
	sp	=				= 95
	bp	=				= 63
	r0/ra	=			= 253
	r1/rb	=			= 191
*/

ff_u8_t c[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//0
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//1
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//2
	1,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//3
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//4
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//5
	0,1,6,0,0,3,0,0,0,0,0,0,4,0,0,0,	//6
	7,0,0,5,0,0,0,0,2,0,0,0,0,0,0,0,	//7
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//8
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//9
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//10
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//11
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//12
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//13
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//14
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 	//15
};

char const* rbl(ff_u8_t __l) {
	switch(__l) {
		case 1: return "al";
		case 2: return "ax";
		case 4: return "eax";
		case 8: return "rax";
	}
}

reginfo*
getreg(char const *__name) {
	char const *p = __name;
	ff_u8_t no;

	no = 0xff;
	while(*p != '\0') {
		no ^= 1<<c[(*p)];
		p++;
	}

	printf("reg: %u, %s\n", no, __name);
/*
	put in array is __ffly_mscars if not defined
	might be a wast of memory but we will give the option
*/
	switch(no) {
		case 0xf8:	return reg+rax_rg;
		case 0xf1:	return reg+eax_rg;
		case 0xf9:	return reg+ax_rg;
		case 0xed:	return reg+al_rg;
		case 0xea:	return reg+rlx_rg;
		case 0xe3:	return reg+elx_rg;
		case 0xeb:	return reg+lx_rg;
		case 0xff:	return reg+ll_rg;
		case 0xe6:	return reg+rel_rg;
		case 0xe5:	return reg+ael_rg;
		case 0xe7:	return reg+el_rg;
		case 0xf5:	return reg+ae_rg;
		case 0xd3:	return reg+xes_rg;
		case 0xc7:	return reg+els_rg;
		case 0xcf:	return reg+ls_rg;
		case 0xdb:	return reg+xs_rg;
		case 0x5f:	return reg+sp_rg;
		case 0x3f:	return reg+bp_rg;
		case 0xfc:	return reg+r0_rg;
		case 0xbe:	return reg+r1_rg;
	}
	printf("failed to get register.\n");
	return NULL;
}

ff_addr_t rgadr(char const *__reg) {
	return getreg(__reg)->addr;
}

void static
emit_label(void) {

}
/*
	at the moment some registers might use memory
*/
# define POSTST 48//registers
void ff_as_prepstack(void) {
	bed+=POSTST;		
}

ff_uint_t ff_as_stackadr() {
	return bed;
}

void ff_as_isa(ff_uint_t __by) {
	bed+=__by;
}

void op_asw(ff_u8_t __op, ff_addr_t __dst, ff_u16_t __val) {
	ff_u8_t buf[5];
	*buf = __op;
	*(ff_u16_t*)(buf+1) = __dst;
	*(ff_u16_t*)(buf+3) = __val;

	fgrowb(curfrag, 5);
	ff_as_plant(curfrag, buf, 5);
}

void rgasw(char const *__reg, ff_u16_t __to) {
	op_asw(_resin_op_asw, getreg(__reg)->addr, __to);
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

ff_i8_t static suffix;

# include "../remf.h"
void static
_post(void) {
	locate_op();
	if (!op) {
		printf("failed to locate operation.\n");
		return;
	}

	ff_u8_t buf[64];
	ff_u8_t *p = buf;
	ff_u8_t *oc = op->opcode;
	ff_u8_t *opbase;
	opbase = p;
	p+=op->l;
	void **cur = fak_->p;
	
	ff_u16_t info;
	printf("%s, %x, ", op->name, *op->opcode);
	if (is_flag(op->flags, asto) && suffix>0)
		oc+=suffix;
	ff_u8_t i = 0;
	ff_u8_t n = fak_->n;
	while(i != n) {
		ff_u8_t off = i++;
		info = getinfo(fak_, off);
		if ((info&_o_imm)>0) {
			if (suffix>0) {
				info = _o_imm8<<suffix;
			}

			printf("{imm}, ");
			printf("%u, ", *(ff_u64_t*)fakget(off));
			ff_u8_t os = get_ous(op, off);
			if (!os)
				os = imm_sz(info);
			memcpy(p, fakget(off), os);
			p+=os;
		} else if ((info&_o_reg)>0) {
			printf("{reg}, ");
			printf("%s, ", ((reginfo*)fakget(off))->name);
			reginfo *reg = (reginfo*)fakget(off);
			if (is_flag(reg->flags, reg_spl)) {
				oc+=op->splr[reg->start];
			} else {
				memcpy(p, &reg->addr, sizeof(ff_addr_t));
				p+=sizeof(ff_addr_t);
			}
		} else if ((info&_o_dis)>0) {
			printf("{dis}, ");
		} else if ((info&_o_label)>0) {
			printf("{label}, ");
			char const *rgname = rbl(sizeof(ff_addr_t));
			labelp la;
			if (!_local) {
				printf("local label, parent: %s\n", curlabel->s);
				la = curlabel;
				local_labelp ll = (local_labelp)__label;
				ll->p_f = &la->f;
				ll->p_foffset = &la->foffset;
				ll->parent = la;
			} else {
				la = (labelp)__label;
			}

			/* LATER:
					remove 'movw' as we dont need it
				output:
				movw addr, %ax {src -> dst}
				call/jmp %ax
			*/

			struct frag *f;
			f = curfrag;
			ff_u32_t ob;
			ob = frag_offset(curfrag);
			ff_u8_t buf0[3];
			*buf0 = _resin_op_asw;
			*(ff_u16_t*)(buf0+1) = getreg(rgname)->addr;
			fgrowb(curfrag, 3);
			ff_as_plant(curfrag, buf0, 3);

			ff_u32_t fo = frag_offset(curfrag);

			ff_as_fdone(curfrag);
			ff_as_fnew();

			*(ff_addr_t*)p = getreg(rgname)->addr;
			p+=sizeof(ff_addr_t);	

			memcpy(opbase, oc, op->l);

			ff_uint_t sz = p-buf;
			fgrowb(curfrag, sz);
			ff_as_plant(curfrag, buf, sz);

			if (is_flag(la->flags, LA_LOOSE))
				_remf_hook(f, fo, ob, 2);
			else
				_remf_reloc(f, fo, ob, 2);
	//		fix(curfrag, la, _fx_dis, 0x00);
			p = buf;
		} else {
			printf("error unknown operand.\n");
			break;
		}
	
		cur++;
	}

	memcpy(opbase, oc, op->l);
	printf("\n");
	ff_uint_t sz = p-buf;
	if (sz>0) {
		// grow frag by X amount
		fgrowb(curfrag, sz);
		ff_as_plant(curfrag, buf, sz);
	}
}

void static*
_getreg(char const *__name) {
	return getreg(__name);
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
		ff_u8_t neg;
		ff_int_t dis;
		dis = ((l->f->adr+l->f->m)+l->foffset)-(__fx->f->adr+__fx->f->m+__fx->f->size);
		if ((neg = (dis<0)))
			dis = -dis;
		ff_uint_t n;
		n = __n(dis);
		ff_uint_t bs;
		ff_int_t *bsp;
		bsp = &__fx->f->bs;
		bs = ((n+((1<<3)-1))>>3);
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
ff_as_resin(void) {
	struct ff_as_op const **cur;
	struct ff_as_op const *op;
	cur = resin_optab;
	void **tray;
	while(*cur != NULL) {
		op = *(cur++);
		ffly_printf("opname: %s:%u\n", op->name, op->nme_len);
		if ((cur-1)>resin_optab) {
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
		ff_as_hash_put(&env, op->name, op->nme_len, tray);
		op++;
	}

	post = _post;
	ff_as_getreg = _getreg;
	ff_as_regsz = _regsz;
	ff_as_suffix = _suffix;
	ff_as_fixins = _fixins;
}
