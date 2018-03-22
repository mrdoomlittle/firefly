# include "../as.h"
# include "../../ffly_def.h"
# include "../../stdio.h"

// bed of stack
mdl_uint_t static bed = 0;

typedef struct {
	char const *name;
	mdl_u8_t l;
	mdl_uint_t addr;
} reginfo;

# define rax_rg 0x0
# define eax_rg 0x1
# define ax_rg 0x2
# define al_rg 0x3

# define rlx_rg 0x4
# define elx_rg 0x5
# define lx_rg 0x6
# define ll_rg 0x7

void oust_addr(ffly_addr_t __addr) {
	oust((mdl_u8_t*)&__addr, sizeof(ffly_addr_t));
}


reginfo reg[] = {
	{"rax", 8, 0}, {"eax", 4, 0}, {"ax", 2, 0}, {"al", 1, 0},
	{"rlx", 8, 8}, {"elx", 4, 8}, {"lx", 2, 8}, {"ll", 1, 8}
};

/*
	1, 2, 4, 8, 16
	r, a, x, e, l
*/

/*
	rax	=	00011	= 24
	eax	=	10001	= 17
	ax	=	10011	= 25
	al	=	10110	= 13
	rlx	=			= 10	
	elx	=			= 3
	lx	=			= 11
	ll	=			= 31
*/

mdl_u8_t c[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//0
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//1
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//2
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//3
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//4
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//5
	0,1,0,0,0,3,0,0,0,0,0,0,4,0,0,0,	//6
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,	//7
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//8
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//9
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//10
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//11
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//12
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//13
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//14
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 	//15
};

char const* rbl(mdl_u8_t __l) {
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
	mdl_u8_t no = 0x1f;
	while(*p != '\0') {
		no ^= 1<<c[(*p)];
		p++;
	}

	printf("reg: %u\n", no);
	switch(no) {
		case 24: return reg+rax_rg;
		case 17: return reg+eax_rg;
		case 25: return reg+ax_rg;
		case 13: return reg+al_rg;
		case 10: return reg+rlx_rg;
		case 3: return reg+elx_rg;
		case 11: return reg+lx_rg;
		case 31: return reg+ll_rg;
	}
	return NULL;
}

ffly_addr_t rgadr(char const *__reg) {
	return getreg(__reg)->addr;
}

# define POSTST 16//registers
void prepstack(void) {
	bed+=POSTST;		
}

mdl_uint_t stackadr() {
	return bed;
}

void isa(mdl_uint_t __by) {
	bed+=__by;
}

void op_exit(mdl_u8_t __opcode, ffly_addr_t __exit) {
	oustbyte(__opcode);
	oust_addr(__exit);
}

void op_asb(mdl_u8_t __opcode, ffly_addr_t __dst, mdl_u8_t __val) {
	oustbyte(__opcode);
	oustbyte(1);
	oust_addr(__dst);
	oustbyte(__val);
}

void op_asw(mdl_u8_t __opcode, ffly_addr_t __dst, mdl_u16_t __val) {
	oustbyte(__opcode);
	oustbyte(2);
	oust_addr(__dst);
	oust_16l(__val);
}

void op_asd(mdl_u8_t __opcode, ffly_addr_t __dst, mdl_u32_t __val) {
	oustbyte(__opcode);
	oustbyte(4);
	oust_addr(__dst);
	oust_32l(__val);
}

void op_asq(mdl_u8_t __opcode, ffly_addr_t __dst, mdl_u64_t __val) {
	oustbyte(__opcode);
	oustbyte(8);
	oust_addr(__dst);
	oust_64l(__val);
}

void op_movb(mdl_u8_t __opcode, ffly_addr_t __src, ffly_addr_t __dst) {
	oustbyte(__opcode);
	oustbyte(1);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_movw(mdl_u8_t __opcode, ffly_addr_t __src, ffly_addr_t __dst) {
	oustbyte(__opcode);
	oustbyte(2);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_movd(mdl_u8_t __opcode, ffly_addr_t __src, ffly_addr_t __dst) {
	oustbyte(__opcode);
	oustbyte(4);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_movq(mdl_u8_t __opcode, ffly_addr_t __src, ffly_addr_t __dst) {
	oustbyte(__opcode);
	oustbyte(8);
	oust_addr(__src);
	oust_addr(__dst);
}

void op_mov(mdl_u8_t __l, ffly_addr_t __src, ffly_addr_t __dst) {
	switch(__l) {
		case 1:
			op_movb(_op_mov, __src, __dst);
		break;
		case 2:
			op_movw(_op_mov, __src, __dst);
		break;
		case 4:
			op_movd(_op_mov, __src, __dst);
		break;
		case 8:
			op_movq(_op_mov, __src, __dst);
		break;
	}
}

void rgasb(char const *__reg, mdl_u8_t __to) {
	op_asb(_op_as, getreg(__reg)->addr, __to);
}

void rgasw(char const *__reg, mdl_u16_t __to) {
	op_asw(_op_as, getreg(__reg)->addr, __to);
}

void rgasd(char const *__reg, mdl_u32_t __to) {
	op_asd(_op_as, getreg(__reg)->addr, __to);
}

void rgasq(char const *__reg, mdl_u64_t __to) {
	op_asq(_op_as, getreg(__reg)->addr, __to);
}

void rgst(char const *__reg, ffly_addr_t __dst) {
	reginfo *rg = getreg(__reg);
	op_mov(rg->l, rg->addr, __dst);
}

void rgld(char const *__reg, ffly_addr_t __src) {
	reginfo *rg = getreg(__reg);
	op_mov(rg->l, __src, rg->addr);
}

void emit_exit(insp __ins) {
	op_exit(*__ins->opcode, *(ffly_addr_t*)__ins->l->p);
}

void emit_asb(insp __ins) {
	op_asb(*__ins->opcode, *(ffly_addr_t*)__ins->l->p, *(mdl_u8_t*)__ins->r->p);
}

void emit_asw(insp __ins) {
	op_asw(*__ins->opcode, *(ffly_addr_t*)__ins->l->p, *(mdl_u16_t*)__ins->r->p);
}

void emit_asd(insp __ins) {
	op_asd(*__ins->opcode, *(ffly_addr_t*)__ins->l->p, *(mdl_u32_t*)__ins->r->p);
}

void emit_asq(insp __ins) {
	op_asq(*__ins->opcode, *(ffly_addr_t*)__ins->l->p, *(mdl_u64_t*)__ins->r->p);
}

void emit_ldb(insp __ins) {
	oustbyte(*__ins->opcode);
	oustbyte(1);
	oust_addr(*(ffly_addr_t*)__ins->r->p);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
}

void emit_stb(insp __ins) {
	oustbyte(*__ins->opcode);
	oustbyte(1);
	oust_addr(*(ffly_addr_t*)__ins->r->p);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
}

void emit_outb(insp __ins) {
	oustbyte(*__ins->opcode);
	oustbyte(1);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
}

void static
emit_jmp(insp __ins) {
	char const *rgname = rbl(sizeof(ffly_addr_t));
	rgasw(rgname, *(mdl_u64_t*)__ins->l->p);
	reloc(offset-4, 4);
	oustbyte(*__ins->opcode);
	oust_addr(getreg(rgname)->addr);
}

void static
emit_rin(insp __ins) {
	oustbyte(*__ins->opcode);
	oust_addr(*(ffly_addr_t*)__ins->l->p);
}

struct ins *bc[] = {
	&(struct ins){"exit", NULL, emit_exit, NULL, NULL, {_op_exit}},
	&(struct ins){"asb", NULL, emit_asb, NULL, NULL, {_op_as}},
	&(struct ins){"asw", NULL, emit_asw, NULL, NULL, {_op_as}},
	&(struct ins){"asd", NULL, emit_asd, NULL, NULL, {_op_as}},
	&(struct ins){"asq", NULL, emit_asq, NULL, NULL, {_op_as}},
	&(struct ins){"ldb", NULL, emit_ldb, NULL, NULL, {_op_ld}},
	&(struct ins){"stb", NULL, emit_stb, NULL, NULL, {_op_st}},
	&(struct ins){"outb", NULL, emit_outb, NULL, NULL, {_op_out}},
	&(struct ins){"jmp", NULL, emit_jmp, NULL, NULL, {_op_jmp}},
	&(struct ins){"rin", NULL, emit_rin, NULL, NULL, {_op_rin}},
	NULL
};
