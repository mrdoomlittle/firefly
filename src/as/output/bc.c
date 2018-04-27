# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"

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
void oust_addr(ff_addr_t __addr) {
	oust((ff_u8_t*)&__addr, sizeof(ff_addr_t));
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
void prepstack(void) {
	bed+=POSTST;		
}

ff_uint_t stackadr() {
	return bed;
}

void isa(ff_uint_t __by) {
	bed+=__by;
}

void op_exit(ff_u8_t __op, ff_addr_t __exit) {
	oustbyte(__op);
	oust_addr(__exit);
}

void op_asb(ff_u8_t __op, ff_addr_t __dst, ff_u8_t __val) {
	oustbyte(__op);
	oustbyte(1);
	oust_addr(__dst);
	oustbyte(__val);
}

void op_asw(ff_u8_t __op, ff_addr_t __dst, ff_u16_t __val) {
	oustbyte(__op);
	oustbyte(2);
	oust_addr(__dst);
	oust_16l(__val);
}

void op_asd(ff_u8_t __op, ff_addr_t __dst, ff_u32_t __val) {
	oustbyte(__op);
	oustbyte(4);
	oust_addr(__dst);
	oust_32l(__val);
}

void op_asq(ff_u8_t __op, ff_addr_t __dst, ff_u64_t __val) {
	oustbyte(__op);
	oustbyte(8);
	oust_addr(__dst);
	oust_64l(__val);
}

void op_mov(ff_u8_t __op, ff_u8_t __l, ff_addr_t __src, ff_addr_t __dst) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_inc(ff_u8_t __op, ff_u8_t __l, ff_addr_t __adr) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__adr);
}

void op_dec(ff_u8_t __op, ff_u8_t __l, ff_addr_t __adr) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__adr);
}

void op_cmp(ff_u8_t __op, ff_u8_t __l, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void rgasb(char const *__reg, ff_u8_t __to) {
	op_asb(_op_as, getreg(__reg)->addr, __to);
}

void rgasw(char const *__reg, ff_u16_t __to) {
	op_asw(_op_as, getreg(__reg)->addr, __to);
}

void rgasd(char const *__reg, ff_u32_t __to) {
	op_asd(_op_as, getreg(__reg)->addr, __to);
}

void rgasq(char const *__reg, ff_u64_t __to) {
	op_asq(_op_as, getreg(__reg)->addr, __to);
}

void rgst(char const *__reg, ff_addr_t __dst) {
	reginfo *rg = getreg(__reg);
	op_mov(_op_mov, rg->l, rg->addr, __dst);
}

void rgld(char const *__reg, ff_addr_t __src) {
	reginfo *rg = getreg(__reg);
	op_mov(_op_mov, rg->l, __src, rg->addr);
}

void emit_exit(insp __ins) {
	op_exit(__ins->op, *(ff_addr_t*)__ins->l->p);
}

void emit_asb(insp __ins) {
	op_asb(__ins->op, *(ff_addr_t*)__ins->l->p, *(ff_u8_t*)__ins->r->p);
}

void emit_asw(insp __ins) {
	op_asw(__ins->op, *(ff_addr_t*)__ins->l->p, *(ff_u16_t*)__ins->r->p);
}

void emit_asd(insp __ins) {
	op_asd(__ins->op, *(ff_addr_t*)__ins->l->p, *(ff_u32_t*)__ins->r->p);
}

void emit_asq(insp __ins) {
	op_asq(__ins->op, *(ff_addr_t*)__ins->l->p, *(ff_u64_t*)__ins->r->p);
}

void op_ld(ff_u8_t __op, ff_u8_t __l, ff_addr_t __lt, ff_addr_t __rt) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__lt);
	oust_addr(__rt);
}

void op_st(ff_u8_t __op, ff_u8_t __l, ff_addr_t __lt, ff_addr_t __rt) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__lt);
	oust_addr(__rt);
}

void emit_ldb(insp __ins) {
	op_ld(__ins->op, 1, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_ldw(insp __ins) {
	op_ld(__ins->op, 2, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_ldd(insp __ins) {
	op_ld(__ins->op, 4, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_ldq(insp __ins) {
	op_ld(__ins->op, 8, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_stb(insp __ins) {
	op_st(__ins->op, 1, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_stw(insp __ins) {
	op_st(__ins->op, 2, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_std(insp __ins) {
	op_st(__ins->op, 4, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void emit_stq(insp __ins) {
	op_st(__ins->op, 8, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void op_out(ff_u8_t __op, ff_u8_t __l, ff_addr_t __adr) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__adr);
}

void emit_outq(insp __ins) {
	op_out(__ins->op, 8, *(ff_addr_t*)__ins->l->p);
}

void emit_outd(insp __ins) {
	op_out(__ins->op, 4, *(ff_addr_t*)__ins->l->p);
}

void emit_outw(insp __ins) {
	op_out(__ins->op, 2, *(ff_addr_t*)__ins->l->p);
}

void emit_outb(insp __ins) {
	op_out(__ins->op, 1, *(ff_addr_t*)__ins->l->p);
}

void static
emit_jmp(insp __ins) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = __ins->l;
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
		is_flag(la->flags, LA_LOOSE)?hook:reloc;

	p(offset-sizeof(ff_addr_t), 2, &la->sy, ll);
	oustbyte(__ins->op);
	oust_addr(getreg(rgname)->addr);
}

void static
emit_rin(insp __ins) {
	oustbyte(__ins->op);
	oustbyte(*(ff_u8_t*)__ins->l->p);
}

void static
op_arm(ff_u8_t __op, ff_u8_t __l, ff_addr_t __lt, ff_addr_t __rt, ff_addr_t __dst) {
	oustbyte(__op);
	oustbyte(__l);
	oust_addr(__lt);
	oust_addr(__rt);
	oust_addr(__dst);
}

void static
op_call(ff_u8_t __op, ff_addr_t __adr) {
	oustbyte(__op);
	oust_addr(__adr);
}

void static
op_ret(ff_u8_t __op) {
	oustbyte(__op);
}

void static
emit_armb(insp __ins) {
	op_arm(__ins->op, 1, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_armw(insp __ins) {
	op_arm(__ins->op, 2, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_armd(insp __ins) {
	op_arm(__ins->op, 4, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_armq(insp __ins) {
	op_arm(__ins->op, 8, *(ff_addr_t*)__ins->l->p,
		*(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_incb(insp __ins) {
	op_inc(__ins->op, 1, *(ff_addr_t*)__ins->l->p);
}

void static
emit_incw(insp __ins) {
	op_inc(__ins->op, 2, *(ff_addr_t*)__ins->l->p);
}

void static
emit_incd(insp __ins) {
	op_inc(__ins->op, 4, *(ff_addr_t*)__ins->l->p);
}

void static
emit_incq(insp __ins) {
	op_inc(__ins->op, 8, *(ff_addr_t*)__ins->l->p);
}

void static 
emit_decb(insp __ins) {
	op_dec(__ins->op, 1, *(ff_addr_t*)__ins->l->p);
}

void static
emit_decw(insp __ins) {
	op_dec(__ins->op, 2, *(ff_addr_t*)__ins->l->p);
}

void static
emit_decd(insp __ins) {
	op_dec(__ins->op, 4, *(ff_addr_t*)__ins->l->p);
}

void static
emit_decq(insp __ins) {
	op_dec(__ins->op, 8, *(ff_addr_t*)__ins->l->p);
}

void static
emit_cmpb(insp __ins) {
	op_cmp(__ins->op, 1, *(ff_addr_t*)__ins->l->p,
		*(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_cmpw(insp __ins) {
	op_cmp(__ins->op, 2, *(ff_addr_t*)__ins->l->p,
		*(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_cmpd(insp __ins) {
	op_cmp(__ins->op, 4, *(ff_addr_t*)__ins->l->p,
		*(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_cmpq(insp __ins) {
	op_cmp(__ins->op, 8, *(ff_addr_t*)__ins->l->p,
		*(ff_addr_t*)__ins->r->p, *(ff_addr_t*)__ins->r->next->p);
}

void static
emit_cjmp(insp __ins) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = __ins->l;
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
		is_flag(la->flags, LA_LOOSE)?hook:reloc;
	
	p(offset-sizeof(ff_addr_t), 2, &la->sy, ll);
	oustbyte(__ins->op);
	oust_addr(getreg(rgname)->addr);
	oust_addr(*(ff_addr_t*)__ins->r->p);
}

void static
emit_movb(insp __ins) {
	op_mov(__ins->op, 1, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void static
emit_movw(insp __ins) {
	op_mov(__ins->op, 2, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void static
emit_movd(insp __ins) {
	op_mov(__ins->op, 4, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void static
emit_movq(insp __ins) {
	op_mov(__ins->op, 8, *(ff_addr_t*)__ins->l->p, *(ff_addr_t*)__ins->r->p);
}

void static
emit_call(insp __ins) {
	char const *rgname = rbl(sizeof(ff_addr_t));
	symbolp l = __ins->l;
	labelp la = (labelp)l->p;

	rgasw(rgname, 0);

	void(*p)(ff_u64_t, ff_u8_t, symbolp*, local_labelp) = 
		is_flag(la->flags, LA_LOOSE)?hook:reloc;

	p(offset-sizeof(ff_addr_t), 2, &la->sy, NULL);
	op_call(__ins->op, getreg(rgname)->addr);
}

void static
emit_ret(insp __ins) {
	op_ret(__ins->op);
}

struct ins *bc[] = {
	&(struct ins){"exit", NULL, emit_exit, NULL, NULL, _op_exit},
	&(struct ins){"asb", NULL, emit_asb, NULL, NULL, _op_as},
	&(struct ins){"asw", NULL, emit_asw, NULL, NULL, _op_as},
	&(struct ins){"asd", NULL, emit_asd, NULL, NULL, _op_as},
	&(struct ins){"asq", NULL, emit_asq, NULL, NULL, _op_as},
	&(struct ins){"ldb", NULL, emit_ldb, NULL, NULL, _op_ld},
	&(struct ins){"ldw", NULL, emit_ldw, NULL, NULL, _op_ld},
	&(struct ins){"ldd", NULL, emit_ldd, NULL, NULL, _op_ld},
	&(struct ins){"ldq", NULL, emit_ldq, NULL, NULL, _op_ld},
	&(struct ins){"stb", NULL, emit_stb, NULL, NULL, _op_st},
	&(struct ins){"stw", NULL, emit_stb, NULL, NULL, _op_st},
	&(struct ins){"std", NULL, emit_stb, NULL, NULL, _op_st},	
	&(struct ins){"stq", NULL, emit_stq, NULL, NULL, _op_st},
	&(struct ins){"outb", NULL, emit_outb, NULL, NULL, _op_out},
	&(struct ins){"outw", NULL, emit_outw, NULL, NULL, _op_out},
	&(struct ins){"outd", NULL, emit_outd, NULL, NULL, _op_out},
	&(struct ins){"outq", NULL, emit_outq, NULL, NULL, _op_out},
	&(struct ins){"jmp", NULL, emit_jmp, NULL, NULL, _op_jmp},
	&(struct ins){"rin", NULL, emit_rin, NULL, NULL, _op_rin},
	&(struct ins){"divb", NULL, emit_armb, NULL, NULL, _op_div},
	&(struct ins){"mulb", NULL, emit_armb, NULL, NULL, _op_mul},
	&(struct ins){"subb", NULL, emit_armb, NULL, NULL, _op_sub},
	&(struct ins){"subw", NULL, emit_armw, NULL, NULL, _op_sub},
	&(struct ins){"subd", NULL, emit_armd, NULL, NULL, _op_sub},
	&(struct ins){"subq", NULL, emit_armq, NULL, NULL, _op_sub},
	&(struct ins){"addb", NULL, emit_armb, NULL, NULL, _op_add},
	&(struct ins){"addw", NULL, emit_armw, NULL, NULL, _op_add},
	&(struct ins){"addd", NULL, emit_armd, NULL, NULL, _op_add},
	&(struct ins){"addq", NULL, emit_armq, NULL, NULL, _op_add},
	&(struct ins){"incb", NULL, emit_incb, NULL, NULL, _op_inc},
	&(struct ins){"decb", NULL, emit_decb, NULL, NULL, _op_dec},
	&(struct ins){"cmpb", NULL, emit_cmpb, NULL, NULL, _op_cmp},
	&(struct ins){"cmpw", NULL, emit_cmpw, NULL, NULL, _op_cmp},
	&(struct ins){"cmpd", NULL, emit_cmpd, NULL, NULL, _op_cmp},
	&(struct ins){"cmpq", NULL, emit_cmpq, NULL, NULL, _op_cmp},
	&(struct ins){"je", NULL, emit_cjmp, NULL, NULL, _op_je},
	&(struct ins){"jne", NULL, emit_cjmp, NULL, NULL, _op_jne},
	&(struct ins){"jg", NULL, emit_cjmp, NULL, NULL, _op_jg},
	&(struct ins){"jl", NULL, emit_cjmp, NULL, NULL, _op_jl},
	&(struct ins){"movb", NULL, emit_movb, NULL, NULL, _op_mov},
	&(struct ins){"movw", NULL, emit_movw, NULL, NULL, _op_mov},
	&(struct ins){"movd", NULL, emit_movd, NULL, NULL, _op_mov},
	&(struct ins){"movq", NULL, emit_movq, NULL, NULL, _op_mov},
	&(struct ins){"call", NULL, emit_call, NULL, NULL, _op_call},
	&(struct ins){"ret", NULL, emit_ret, NULL, NULL, _op_ret},
	NULL
};
