# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"
# include "../opcodes/resin_tbl.h"

labelp extern curlabel;
// bed of stack
ff_uint_t static bed = 0;

typedef struct {
	char const *name;
	ff_u8_t l;
	ff_uint_t addr;
} reginfo;

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
void static
oust_addr(ff_addr_t __addr) {
	ff_as_oust((ff_u8_t*)&__addr, sizeof(ff_addr_t));
}

reginfo reg[] = {
	{"sp", 8, 0},  {"bp", 8, 8},
	{"rax", 8, 16},	{"eax", 4, 16},	{"ax", 2, 16},	{"al", 1, 16},
	{"rlx", 8, 24},	{"elx", 4, 24},	{"lx", 2, 24},	{"ll", 1, 24},
	{"rel", 8, 32},	{"ael", 4, 32},	{"el", 2, 32},	{"ae", 1, 32},
	{"xes", 8, 40}, {"els", 4, 40}, {"ls", 2, 40},  {"xs", 1, 40},
};

/*
	1, 2, 4, 8, 16, 32, 64, 128
	r, a, x, e, l,	s,	b,	p
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
	sp	=	11111111	= 95
	bp	=	11111111	= 63
*/

ff_u8_t c[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//0
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//1
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//2
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//3
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
	ff_u8_t no = 0xff;
	while(*p != '\0') {
		no ^= 1<<c[(*p)];
		p++;
	}

	printf("reg: %u, %s\n", no, __name);
	switch(no) {
		case 248:	return reg+rax_rg;
		case 241:	return reg+eax_rg;
		case 249:	return reg+ax_rg;
		case 237:	return reg+al_rg;
		case 234:	return reg+rlx_rg;
		case 227:	return reg+elx_rg;
		case 235:	return reg+lx_rg;
		case 255:	return reg+ll_rg;
		case 230:	return reg+rel_rg;
		case 229:	return reg+ael_rg;
		case 231:	return reg+el_rg;
		case 245:	return reg+ae_rg;
		case 211:	return reg+xes_rg;
		case 199:	return reg+els_rg;
		case 207:	return reg+ls_rg;
		case 219:	return reg+xs_rg;
		case 95:	return reg+sp_rg;
		case 63:	return reg+bp_rg;
	}
	printf("failed to get register.\n");
	return NULL;
}

ff_addr_t rgadr(char const *__reg) {
	return getreg(__reg)->addr;
}

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

void op_exit(ff_u8_t __op, ff_addr_t __exit) {
	ff_as_oustbyte(__op);
	oust_addr(__exit);
}

void op_asb(ff_u8_t __op, ff_addr_t __dst, ff_u8_t __val) {
	ff_as_oustbyte(__op);
	oust_addr(__dst);
	ff_as_oustbyte(__val);
}

void op_asw(ff_u8_t __op, ff_addr_t __dst, ff_u16_t __val) {
	ff_as_oustbyte(__op);
	oust_addr(__dst);
	ff_as_oust_16l(__val);
}

void op_asd(ff_u8_t __op, ff_addr_t __dst, ff_u32_t __val) {
	ff_as_oustbyte(__op);
	oust_addr(__dst);
	ff_as_oust_32l(__val);
}

void op_asq(ff_u8_t __op, ff_addr_t __dst, ff_u64_t __val) {
	ff_as_oustbyte(__op);
	oust_addr(__dst);
	ff_as_oust_64l(__val);
}

void op_mov(ff_u8_t __op, ff_addr_t __src, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_inc(ff_u8_t __op, ff_addr_t __adr) {
	ff_as_oustbyte(__op);
	oust_addr(__adr);
}

void op_dec(ff_u8_t __op, ff_addr_t __adr) {
	ff_as_oustbyte(__op);
	oust_addr(__adr);
}

void op_cmp(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void rgasb(char const *__reg, ff_u8_t __to) {
	op_asb(_resin_op_asb, getreg(__reg)->addr, __to);
}

void rgasw(char const *__reg, ff_u16_t __to) {
	op_asw(_resin_op_asw, getreg(__reg)->addr, __to);
}

void rgasd(char const *__reg, ff_u32_t __to) {
	op_asd(_resin_op_asd, getreg(__reg)->addr, __to);
}

void rgasq(char const *__reg, ff_u64_t __to) {
	op_asq(_resin_op_asq, getreg(__reg)->addr, __to);
}

void rgst(char const *__reg, ff_addr_t __dst) {
	reginfo *rg = getreg(__reg);
	ff_u8_t op;
	switch(rg->l) {
		case 1: op = _op_movb; break;
		case 2: op = _op_movw; break;
		case 4: op = _op_movd; break;
		case 8: op = _op_movq; break;
	}
	op_mov(op, rg->addr, __dst);
}

void rgld(char const *__reg, ff_addr_t __src) {
	reginfo *rg = getreg(__reg);
	ff_u8_t op;
	switch(rg->l) {
		case 1: op = _op_movb; break;
		case 2: op = _op_movw; break;
		case 4: op = _op_movd; break;
		case 8: op = _op_movq; break;
	}
	op_mov(op, __src, rg->addr);
}

void emit_exit(void) {
	op_exit(_resin_op_exit, *(ff_addr_t*)fakget(0)->p);
}

void emit_asb(void) {
	op_asb(_resin_op_asb, *(ff_addr_t*)fakget(0)->p,
		*(ff_u8_t*)fakget(1)->p);
}

void emit_asw(void) {
	op_asw(_resin_op_asw, *(ff_addr_t*)fakget(0)->p,
		*(ff_u16_t*)fakget(1)->p);
}

void emit_asd(void) {
	op_asd(_resin_op_asd, *(ff_addr_t*)fakget(0)->p,
		*(ff_u32_t*)fakget(1)->p);
}

void emit_asq(void) {
	op_asq(_resin_op_asq, *(ff_addr_t*)fakget(0)->p,
		*(ff_u64_t*)fakget(1)->p);
}

void op_ld(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
}

void op_st(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
}

void emit_ld(void) {
	op_ld(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p);
}

void emit_st(void) {
	op_st(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p);
}

void op_out(ff_u8_t __op, ff_addr_t __adr) {
	ff_as_oustbyte(__op);
	oust_addr(__adr);
}

void emit_out(void) {
	op_out(*op->opcode, *(ff_addr_t*)fakget(0)->p);
}

void static
emit_jmp(void) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = fakget(0);
	local_labelp ll = NULL;

	labelp la;
	if (is_syll(l)) {
		la = curlabel;
		ll = (local_labelp)l->p;
		ll->p_adr = &curlabel->adr; 
	} else
		la = (labelp)l->p;

	rgasw(rgname, 0);

	void(*p)(ff_u64_t, ff_u8_t, symbolp*, local_labelp) = 
		is_flag(la->flags, LA_LOOSE)?ff_as_hook:ff_as_reloc;

	p(offset-sizeof(ff_addr_t), 2, &la->sy, ll);
	ff_as_oustbyte(*op->opcode);
	oust_addr(getreg(rgname)->addr);
}

void static
op_rin(ff_u8_t __op, ff_u8_t __no, ff_addr_t __arg) {
	ff_as_oustbyte(__op);
	ff_as_oustbyte(__no);
	oust_addr(__arg);
}

void static
emit_rin(void) {
	op_rin(*op->opcode, *(ff_u8_t*)fakget(0)->p, *(ff_addr_t*)fakget(1)->p);
}

void static
op_div(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void static
op_mul(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void static
op_sub(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void static
op_add(ff_u8_t __op, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	ff_as_oustbyte(__op);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void static
op_call(ff_u8_t __op, ff_addr_t __adr) {
	ff_as_oustbyte(__op);
	oust_addr(__adr);
}

void static
op_ret(ff_u8_t __op) {
	ff_as_oustbyte(__op);
}

void static
emit_div(void) {
	op_div(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p, *(ff_addr_t*)fakget(2)->p);
}

void static
emit_mul(void) {
	op_mul(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p, *(ff_addr_t*)fakget(2)->p);
}

void static
emit_sub(void) {
	op_sub(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p, *(ff_addr_t*)fakget(2)->p);
}

void static
emit_add(void) {
	op_add(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p, *(ff_addr_t*)fakget(2)->p);
}

void static
emit_inc(void) {
	op_inc(*op->opcode, *(ff_addr_t*)fakget(0)->p);
}

void static 
emit_dec(void) {
	op_dec(*op->opcode, *(ff_addr_t*)fakget(0)->p);
}

void static
emit_cmp(void) {
	op_cmp(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p, *(ff_addr_t*)fakget(2)->p);
}

void static
emit_cjmp(void) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = fakget(0);
	local_labelp ll = NULL;

	labelp la;
	if (is_syll(l)) {
		la = curlabel;
		ll = (local_labelp)l->p;
		ll->p_adr = &curlabel->adr;
	} else 
		la = (labelp)l->p;

	rgasw(rgname, 0);

	void(*p)(ff_u64_t, ff_u8_t, symbolp*, local_labelp) = 
		is_flag(la->flags, LA_LOOSE)?ff_as_hook:ff_as_reloc;
	
	p(offset-sizeof(ff_addr_t), 2, &la->sy, ll);
	ff_as_oustbyte(*op->opcode);
	oust_addr(getreg(rgname)->addr);
	oust_addr(*(ff_addr_t*)fakget(1)->p);
}

void static
emit_mov(void) {
	op_mov(*op->opcode, *(ff_addr_t*)fakget(0)->p,
		*(ff_addr_t*)fakget(1)->p);
}

void static
emit_call(void) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = fakget(0);
	labelp la = (labelp)l->p;

	rgasw(rgname, 0);

	void(*p)(ff_u64_t, ff_u8_t, symbolp*, local_labelp) = 
		is_flag(la->flags, LA_LOOSE)?ff_as_hook:ff_as_reloc;

	p(offset-sizeof(ff_addr_t), 2, &la->sy, NULL);
	op_call(*op->opcode, getreg(rgname)->addr);
}

void static
emit_ret(void) {
	op_ret(*op->opcode);
}

struct berry emit[] = {
	{emit_exit, NULL},
	{emit_asb, NULL},
	{emit_asw, NULL},
	{emit_asd, NULL},
	{emit_asq, NULL},

	{emit_jmp, NULL},

	{emit_st, NULL},
	{emit_st, NULL},
	{emit_st, NULL},
	{emit_st, NULL},

	{emit_ld, NULL},
	{emit_ld, NULL},
	{emit_ld, NULL},
	{emit_ld, NULL},	

	{emit_out, NULL},
	{emit_out, NULL},
	{emit_out, NULL},
	{emit_out, NULL},

	{emit_mov, NULL},
	{emit_mov, NULL},
	{emit_mov, NULL},
	{emit_mov, NULL},

	{emit_rin, NULL},

	{emit_div, NULL},
	{emit_div, NULL},
	{emit_div, NULL},
	{emit_div, NULL},

	{emit_mul, NULL},
	{emit_mul, NULL},
	{emit_mul, NULL},
	{emit_mul, NULL},

	{emit_sub, NULL},
	{emit_sub, NULL},
	{emit_sub, NULL},
	{emit_sub, NULL},

	{emit_add, NULL},
	{emit_add, NULL},
	{emit_add, NULL},
	{emit_add, NULL},

	{emit_inc, NULL},
	{emit_inc, NULL},
	{emit_inc, NULL},
	{emit_inc, NULL},

	{emit_dec, NULL},
	{emit_dec, NULL},
	{emit_dec, NULL},
	{emit_dec, NULL},

	{emit_cmp, NULL},
	{emit_cmp, NULL},
	{emit_cmp, NULL},
	{emit_cmp, NULL},

	{emit_cjmp, NULL},
	{emit_cjmp, NULL},
	{emit_cjmp, NULL},
	{emit_cjmp, NULL},

	{emit_call, NULL},
	{emit_ret, NULL}
};

void static
_post(void(*__emit)(void)) {
	__emit();
}

# include "../../dep/str_len.h"
void
ff_as_resin(void) {
	struct ff_as_op const **cur;
	struct ff_as_op const *op;

	struct berry *bar = emit;
	cur = resin_optab;
	while(*cur != NULL) {
		op = *(cur++);
		ffly_printf("opname: %s\n", op->name);
		ff_as_hash_put(&env, op->name, ffly_str_len(op->name), bar);
		(bar++)->op = op;
		op++;
	}
	post = _post;
}
