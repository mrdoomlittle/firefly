# include "resin.h"
# include "system/errno.h"
# include "system/io.h"
# include "dep/mem_set.h"
# include "dep/mem_cpy.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
#define copymem(__src, __dst, __size) \
	ffly_mem_cpy(__dst, __src, __size)
#define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
/*
	make stack put and get better somehow
	also more error detection

	TODO: COULDDO
		add lock memory - a range from X to X within stack where memory can be locked
		and can only be unlocked by an external source
*/
/*
	TODO:
	stack_put and stack_get to asm not c
*/

/*
	TODO:
		use function pointer for get functions e.g. get_8l
		as if flag not set we will use a direct method to get a byte else will go thru getbits
		as to allow for compact code
*/
ff_err_t static
stack_put(ffly_resinp __resin, ff_u8_t *__src, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p = __src;
	ff_u8_t *end = p+__bc;
	ff_addr_t dst;

	while(p != end) {
		if ((dst = __addr+(p-__src)) >= __resin->stack_size) {
			ffly_printf("put error.\n");
			return FFLY_FAILURE;
		}

		*(__resin->stack+dst) = *(p++);
	}
	return FFLY_SUCCESS;
}

ff_err_t static
stack_get(ffly_resinp __resin, ff_u8_t *__dst, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p = __dst;
	ff_u8_t *end = p+__bc;
	ff_addr_t src;

	while(p != end) {
		if ((src = __addr+(p-__dst)) >= __resin->stack_size) {
			ffly_printf("get error.\n");
			return FFLY_FAILURE;
		}

		*(p++) = *(__resin->stack+src);	
	}
	return FFLY_SUCCESS;
}

void ff_resin_sst(ffly_resinp __resin, void *__p, ff_addr_t __adr, ff_uint_t __n) {
	ffly_mem_cpy(__resin->stack+__adr, __p, __n);
}

void ff_resin_sld(ffly_resinp __resin, ff_addr_t __adr, void *__p, ff_uint_t __n) {
	ffly_mem_cpy(__p, __resin->stack+__adr, __n);
}

void ff_resin_rset(ffly_resinp __resin, void *__p, ff_u8_t __r, ff_uint_t __n) {
	ffly_mem_cpy(__resin->r[__r], __p, __n);
}

void ff_resin_rget(ffly_resinp __resin, ff_u8_t __r, void *__p, ff_uint_t __n) {
	ffly_mem_cpy(__p, __resin->r[__r], __n);
}

ff_u8_t static
next_byte(void *__arg_p) {
	ffly_resinp p = (ffly_resinp)__arg_p;
	return p->fetch_byte(p->ip_off++);
}

# define get_4l(__resin) \
	ffly_get_bit(&__resin->bit, 4)

ff_u8_t static
get_8l(ffly_resinp __resin, ff_err_t *__err) {
	return ffly_get_bit(&__resin->bit, 8);
}

ff_u16_t static
get_16l(ffly_resinp __resin, ff_err_t *__err) {
	return ((ff_u16_t)get_8l(__resin, __err))|((ff_u16_t)get_8l(__resin, __err))<<8;
}

ff_u32_t static
get_32l(ffly_resinp __resin, ff_err_t *__err) {
	return ((ff_u32_t)get_16l(__resin, __err))|((ff_u32_t)get_16l(__resin, __err))<<16;
}

ff_u64_t static
get_64l(ffly_resinp __resin, ff_err_t *__err) {
	return ((ff_u64_t)get_32l(__resin, __err))|((ff_u64_t)get_32l(__resin, __err))<<32;
}

ff_err_t ff_resin_init(ffly_resinp __resin) {
	ffly_init_get_bit(&__resin->bit, next_byte, (void*)__resin);
	__resin->stack = (ff_u8_t*)__ffly_mem_alloc(__resin->stack_size);
	ffly_mem_set(__resin->stack, 0xff, __resin->stack_size);
	__resin->retto = __resin->rtbuf;

	ff_u64_t **r = __resin->r;
	*r = &__resin->r0;
	*(r+1) = &__resin->r1;
	*(r+2) = &__resin->r2;
	*(r+3) = &__resin->r3;
	return FFLY_SUCCESS;
}

ff_err_t ff_resin_de_init(ffly_resinp __resin) {
	__ffly_mem_free(__resin->stack);
	return FFLY_SUCCESS;
}

void static
get(ffly_resinp __resin, ff_u8_t *__dst, ff_u8_t __n, ff_err_t *__err) {
	ff_u8_t *p = __dst;
	ff_u8_t *end = p+__n;
	while(p != end) {
		*(p++) = get_8l(__resin, __err);
	}
}

/*
	TODO:
		prestore instruction ^
		so we dont need to read byte per byte thru instruction
		also could do this a few steps ahead
*/

#define __ADDR_TS sizeof(ff_addr_t)
ff_u8_t static ops[] = {
	__ADDR_TS,		//exit

	__ADDR_TS+1,	//asb
	__ADDR_TS+2,	//asw
	__ADDR_TS+4,	//asd
	__ADDR_TS+8,	//asq

	__ADDR_TS,		//jmp

	__ADDR_TS*2,	//stb
	__ADDR_TS*2,	//stw
	__ADDR_TS*2,	//std
	__ADDR_TS*2,	//stq

	__ADDR_TS*2,    //ldb
	__ADDR_TS*2,    //ldw
	__ADDR_TS*2,    //ldd
	__ADDR_TS*2,    //ldq

	__ADDR_TS,		//outb
	__ADDR_TS,		//outw
	__ADDR_TS,		//outd
	__ADDR_TS,		//outq

	__ADDR_TS*2,	//movb
	__ADDR_TS*2,	//movw
	__ADDR_TS*2,	//movd
	__ADDR_TS*2,	//movq

	__ADDR_TS+1,	//rin

	__ADDR_TS*2,	//divb
	__ADDR_TS*2,	//divw
	__ADDR_TS*2,	//divd
	__ADDR_TS*2,	//divq

	__ADDR_TS*2,	//mulb
	__ADDR_TS*2,	//mulw
	__ADDR_TS*2,	//muld
	__ADDR_TS*2,	//mulq

	__ADDR_TS*2,	//subb
	__ADDR_TS*2,	//subw
	__ADDR_TS*2,	//subd
	__ADDR_TS*2,	//subq

	__ADDR_TS*2,	//addb
	__ADDR_TS*2,	//addw
	__ADDR_TS*2,	//addd
	__ADDR_TS*2,	//addq

	__ADDR_TS,		//incb
	__ADDR_TS,		//incw
	__ADDR_TS,		//incd
	__ADDR_TS,		//incq

	__ADDR_TS,		//decb
	__ADDR_TS,		//decw
	__ADDR_TS,		//decd
	__ADDR_TS,		//decq

	__ADDR_TS*2,	//cmpb
	__ADDR_TS*2,	//cmpw
	__ADDR_TS*2,	//cmpd
	__ADDR_TS*2,	//cmpq

	__ADDR_TS,	//cjmp
	__ADDR_TS,	//cjmp
	__ADDR_TS,	//cjmp
	__ADDR_TS,	//cjmp

	__ADDR_TS,	//call
	0			//ret
};

ff_u8_t ff_resin_ops(ff_u8_t __op) {
	return ops[__op];
}

void*
ff_resin_resolv_adr(ffly_resinp __resin, ff_addr_t __adr) {
	if (__adr>=__resin->stack_size) {
		ffly_printf("address out of rage, got: %u\n", __adr);
		return NULL;
	}
	return (void*)(__resin->stack+__adr);
}

void _res_exit0();

void _res_as0b();
void _res_as0w();
void _res_as0d();
void _res_as0q();

void _res_jmp0();

void _res_st0b();
void _res_st0w();
void _res_st0d();
void _res_st0q();

void _res_ld0b();
void _res_ld0w();
void _res_ld0d();
void _res_ld0q();

void _res_out0b();
void _res_out0w();
void _res_out0d();
void _res_out0q();

void _res_mov0b();
void _res_mov0w();
void _res_mov0d();
void _res_mov0q();

void _res_rin0();

void _res_div0b();
void _res_div0w();
void _res_div0d();
void _res_div0q();

void _res_mul0b();
void _res_mul0w();
void _res_mul0d();
void _res_mul0q();

void _res_sub0b();
void _res_sub0w();
void _res_sub0d();
void _res_sub0q();

void _res_add0b();
void _res_add0w();
void _res_add0d();
void _res_add0q();

void _res_inc0b();
void _res_inc0w();
void _res_inc0d();
void _res_inc0q();

void _res_dec0b();
void _res_dec0w();
void _res_dec0d();
void _res_dec0q();

void _res_cmp0b();
void _res_cmp0w();
void _res_cmp0d();
void _res_cmp0q();

void _res_cjmp0();
void _res_call0();
void _res_ret();

void _res_as1br0();
void _res_as1wr0();
void _res_as1dr0();
void _res_as1qr0();

void _res_as1br1();
void _res_as1wr1();
void _res_as1dr1();
void _res_as1qr1();

void _res_mov1br0();
void _res_mov1wr0();
void _res_mov1dr0();
void _res_mov1qr0();

void _res_mov1br1();
void _res_mov1wr1();
void _res_mov1dr1();
void _res_mov1qr1();

void _res_mov2br0();
void _res_mov2wr0();
void _res_mov2dr0();
void _res_mov2qr0();

void _res_mov2br1();
void _res_mov2wr1();
void _res_mov2dr1();
void _res_mov2qr1();

void _res_mov3br0r1();
void _res_mov3wr0r1();
void _res_mov3dr0r1();
void _res_mov3qr0r1();

void _res_mov3br1r0();
void _res_mov3wr1r0();
void _res_mov3dr1r0();
void _res_mov3qr1r0();

void _res_out1br0();
void _res_out1wr0();
void _res_out1dr0();
void _res_out1qr0();

void _res_out1br1();
void _res_out1wr1();
void _res_out1dr1();
void _res_out1qr1();

void _res_inc1br0();
void _res_inc1wr0();
void _res_inc1dr0();
void _res_inc1qr0();

void _res_inc1br1();
void _res_inc1wr1();
void _res_inc1dr1();
void _res_inc1qr1();

void _res_inc1br2();
void _res_inc1wr2();
void _res_inc1dr2();
void _res_inc1qr2();

void _res_inc1br3();
void _res_inc1wr3();
void _res_inc1dr3();
void _res_inc1qr3();

void _res_dec1br0();
void _res_dec1wr0();
void _res_dec1dr0();
void _res_dec1qr0();

void _res_dec1br1();
void _res_dec1wr1();
void _res_dec1dr1();
void _res_dec1qr1();

void _res_dec1br2();
void _res_dec1wr2();
void _res_dec1dr2();
void _res_dec1qr2();

void _res_dec1br3();
void _res_dec1wr3();
void _res_dec1dr3();
void _res_dec1qr3();

void _res_jmp1r0();
void _res_jmp1r1();

void _res_add1br0r1();
void _res_add1wr0r1();
void _res_add1dr0r1();
void _res_add1qr0r1();

void _res_add1br1r0();
void _res_add1wr1r0();
void _res_add1dr1r0();
void _res_add1qr1r0();

void _res_r0r0();
void _res_r1r0();
void _res_r2r0();
void _res_r3r0();

void _res_r0r1();
void _res_r1r1();
void _res_r2r1();
void _res_r3r1();

void _res_push0b();
void _res_push0w();
void _res_push0d();
void _res_push0q();

void _res_pop0b();
void _res_pop0w();
void _res_pop0d();
void _res_pop0q();

void _res_push1br0();
void _res_push1wr0();
void _res_push1dr0();
void _res_push1qr0();

void _res_push1br1();
void _res_push1wr1();
void _res_push1dr1();
void _res_push1qr1();

void _res_pop1br0();
void _res_pop1wr0();
void _res_pop1dr0();
void _res_pop1qr0();

void _res_pop1br1();
void _res_pop1wr1();
void _res_pop1dr1();
void _res_pop1qr1();

void _res_lbpr0();
void _res_lbpr1();
void _res_lspr0();
void _res_lspr1();

void _res_exit1();
void _res_exit2r0();
void _res_exit2r1();

static void(*op[])() = {
	_res_exit0,
	// assign
	_res_as0b,
	_res_as0w,
	_res_as0d,
	_res_as0q,	
	// jump
	_res_jmp0,
	// store
	_res_st0b,
	_res_st0w,
	_res_st0d,
	_res_st0q,
	// load
	_res_ld0b,
	_res_ld0w,
	_res_ld0d,
	_res_ld0q,
	// out
	_res_out0b,
	_res_out0w,
	_res_out0d,
	_res_out0q,
	// move
	_res_mov0b,
	_res_mov0w,
	_res_mov0d,
	_res_mov0q,
	// ring
	_res_rin0,
	// dev
	_res_div0b,
	_res_div0w,
	_res_div0d,
	_res_div0q,
	// mul
	_res_mul0b,
	_res_mul0w,
	_res_mul0d,
	_res_mul0q,
	// sub
	_res_sub0b,
	_res_sub0w,
	_res_sub0d,
	_res_sub0q,
	// add
	_res_add0b,
	_res_add0w,
	_res_add0d,
	_res_add0q,
	// incr
	_res_inc0b,
	_res_inc0w,
	_res_inc0d,
	_res_inc0q,
	// decr
	_res_dec0b,
	_res_dec0w,
	_res_dec0d,
	_res_dec0q,
	// compare
	_res_cmp0b,
	_res_cmp0w,
	_res_cmp0d,
	_res_cmp0q,
	// je
	_res_cjmp0,
	// jne
	_res_cjmp0,
	// jg
	_res_cjmp0,
	// jl
	_res_cjmp0,

	_res_call0,
	_res_ret,

	_res_as1br0,
	_res_as1wr0,
	_res_as1dr0,
	_res_as1qr0,

	_res_as1br1,
	_res_as1wr1,
	_res_as1dr1,
	_res_as1qr1,

	_res_mov1br0,
	_res_mov1wr0,
	_res_mov1dr0,
	_res_mov1qr0,

	_res_mov1br1,
	_res_mov1wr1,
	_res_mov1dr1,
	_res_mov1qr1,

	_res_mov2br0,
	_res_mov2wr0,
	_res_mov2dr0,
	_res_mov2qr0,
	
	_res_mov2br1,
	_res_mov2wr1,
	_res_mov2dr1,
	_res_mov2qr1,

	_res_mov3br0r1,
	_res_mov3wr0r1,
	_res_mov3dr0r1,
	_res_mov3qr0r1,

	_res_mov3br1r0,
	_res_mov3wr1r0,
	_res_mov3dr1r0,
	_res_mov3qr1r0,

	_res_out1br0,
	_res_out1wr0,
	_res_out1dr0,
	_res_out1qr0,

	_res_out1br1,
	_res_out1wr1,
	_res_out1dr1,
	_res_out1qr1,

	_res_inc1br0,
	_res_inc1wr0,
	_res_inc1dr0,
	_res_inc1qr0,

	_res_inc1br1,
	_res_inc1wr1,
	_res_inc1dr1,
	_res_inc1qr1,

	_res_dec1br0,
	_res_dec1wr0,
	_res_dec1dr0,
	_res_dec1qr0,

	_res_dec1br1,
	_res_dec1wr1,
	_res_dec1dr1,
	_res_dec1qr1,

	_res_jmp1r0,
	_res_jmp1r1,

	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,

	_res_add1br0r1,
	_res_add1wr0r1,
	_res_add1dr0r1,
	_res_add1qr0r1,

 	_res_add1br1r0,
 	_res_add1wr1r0,
 	_res_add1dr1r0,
	_res_add1qr1r0,
	
	_res_lbpr0,
	_res_lbpr1,
	_res_lspr0,
	_res_lspr1,

	_res_r0r0,
	_res_r1r0,
	_res_r2r0,
	_res_r3r0,

	_res_r0r1,
	_res_r1r1,
	_res_r2r1,
	_res_r3r1,

	_res_push0b,
	_res_push0w,
	_res_push0d,
	_res_push0q,
	_res_pop0b,
	_res_pop0w,
	_res_pop0d,
	_res_pop0q,

	_res_push1br0,
	_res_push1wr0,
	_res_push1dr0,
	_res_push1qr0,

	_res_push1br1,
	_res_push1wr1,
	_res_push1dr1,
	_res_push1qr1,

	_res_pop1br0,
	_res_pop1wr0,
	_res_pop1dr0,
	_res_pop1qr0,

	_res_pop1br1,
	_res_pop1wr1,
	_res_pop1dr1,
	_res_pop1qr1,

	_res_exit1,
	_res_exit2r0,
	_res_exit2r1
};
/*
	as im using asm labels it may cause issues
	ive checked the assembly output and it allocates
	memory for variables on function entry so i dont think
	it should cause any problems if it does re will have to 
	we think how we deal with variables.

	also registers might cause issues with all the jmps 
	i dont know how gcc would react so could break.
	
	using registers r8, r9, ... im hoping that gcc does not
	use them, it seems to be okay for now...
	its so we dont need to use push and pop to make sure the contents
	of the register stay the same and wont cause issues
*/

/*
	a lot of repeating of same code, why? 
	to reduce number of machine instructions executed.
	i dont like it but thats how it is.

	if we did it diffrently it would be slower and speed
	is the most important thing for this.

	TODO:
		error detection
*/

// for debug
#define MAX 0xff
#define get_addr(__resin, __err) \
	get_16l(__resin, __err)

#define fi			__asm__("jmp _res_fi"); // finished
#define next		__asm__("jmp _res_next")
#define end			__asm__("jmp _res_end")
#define jmpto(__p)	__asm__("jmp *%0" : : "r"(__p))
#define errjmp if (_err(err)) jmpend
# include "system/io.h"
# include "system/nanosleep.h"
__asm__("\t.globl _res_fi\n"
		"\t.globl _res_end");
/*
	register pointers 
	4 registers 2 pointers
*/
ff_u64_t *res_r0, *res_r1;
ff_u64_t *res_bp, *res_sp;
ff_u8_t *res_stack;
/*
	rename opno to just on for opn or whatever fits

	TODO:
		"allow" for copy of all code so we can append finish at end
*/
ff_u8_t static ob[64];
ff_err_t ff_resin_exec(ffly_resinp __resin, ff_err_t *__exit_code) {
	ff_err_t err;
	ff_u16_t opno;
	ff_u8_t l;
	ff_err_t code;

	// not used directly
	// gets put in res_r0 or res_r1
	ff_u64_t *r0 = &__resin->r0;
	ff_u64_t *r1 = &__resin->r1;
	ff_u64_t *r2 = &__resin->r2;
	ff_u64_t *r3 = &__resin->r3;
	ff_u64_t *bp = &__resin->bp;
	ff_u64_t *sp = &__resin->sp;
	ff_u8_t *cflags = &__resin->cflags;
	res_stack = __resin->stack;
	res_bp = bp;
	res_sp = sp;
	*sp = 100;//ignore only for testing

	ff_u64_t *r;

	__asm__("_res_next:\n\t");
	__resin->ip_off = 0;
	/*
		TODO:
			put if statment within define so we dont need to compile with it,
			only for debugging to remove unneed shit
	*/
	if ((opno = get_8l(__resin, &err)) > MAX) {
		ffly_printf("opno invalid, got: %u\n", opno);
		return FFLY_FAILURE;
	}

	get(__resin, ob, ops[opno], &err);
/*
	{
		ff_uint_t i;
		i = 0;
		for(;i != ops[opno];i++) {
			ffly_printf("%u\n", ob[i]);
		}
	}
*/
	jmpto(op[opno]);

	__asm__("_res_push0b:			\n\t"
			"movb $1, %0			\n\t"
			"jmp _res_push0			\n"
			"_res_push0w:			\n\t"
			"movb $2, %0            \n\t"
			"jmp _res_push0         \n"
			"_res_push0d:			\n\t"
			"movb $4, %0            \n\t"
			"jmp _res_push0         \n"
			"_res_push0q:			\n\t"
			"movb $8, %0			\n"
			"_res_push0:			\n\t" : "=m"(l));
	{
		ff_addr_t dst, src;
		dst = (*sp = (*sp-l));
		src = *(ff_addr_t*)ob;
		ff_u8_t tmp[8];
		stack_get(__resin, (ff_u8_t*)tmp, l, src);
		stack_put(__resin, (ff_u8_t*)tmp, l, dst);		
	}
	fi;

	__asm__("_res_pop0b:			\n\t"
			"movb $1, %0			\n\t"
			"jmp _res_pop0 			\n"
			"_res_pop0w:			\n\t"
			"movb $2, %0			\n\t"
			"jmp _res_pop0			\n"
			"_res_pop0d:			\n\t"
			"movb $4, %0			\n\t"
			"jmp _res_pop0			\n"
			"_res_pop0q:			\n\t"
			"movb $8, %0			\n"
			"_res_pop0:				\n\t" : "=m"(l));
	{
		ff_addr_t dst, src;
		dst = *(ff_addr_t*)ob;
		src = *sp;
		*sp+=l;
		ff_u8_t tmp[8];
		stack_get(__resin, (ff_u8_t*)tmp, l, src);
		stack_put(__resin, (ff_u8_t*)tmp, l, dst);
	}
	fi;

	__asm__("_res_div0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_div0		\n"
			"_res_div0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_div0		\n"
			"_res_div0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_div0		\n"
			"_res_div0q:		\n\t"
			"movb $8, %0		\n"
			"_res_div0:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr;
		lt_adr = *(ff_addr_t*)ob;
		rt_adr = *(ff_addr_t*)(ob+__ADDR_TS);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = rt/lt;
		stack_put(__resin, (ff_u8_t*)&dst, l, rt_adr);
	}
	fi;

	__asm__("_res_mul0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mul0		\n"
			"_res_mul0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mul0		\n"
			"_res_mul0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mul0		\n"
			"_res_mul0q:		\n\t"
			"movb $8, %0		\n"
			"_res_mul0:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr;
		lt_adr = *(ff_addr_t*)ob;
		rt_adr = *(ff_addr_t*)(ob+__ADDR_TS);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = rt*lt;
		stack_put(__resin, (ff_u8_t*)&dst, l, rt_adr);
	}
	fi;

	__asm__("_res_sub0b:		\n\t"
			"movb $1, %0		\n\t"		
			"jmp _res_sub0		\n"
			"_res_sub0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_sub0		\n"
			"_res_sub0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_sub0		\n"
			"_res_sub0q:		\n\t"
			"movb $8, %0		\n"
			"_res_sub0:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr;
		lt_adr = *(ff_addr_t*)ob;
		rt_adr = *(ff_addr_t*)(ob+__ADDR_TS);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = rt-lt;
		stack_put(__resin, (ff_u8_t*)&dst, l, rt_adr);
	}
	fi;

	__asm__("_res_add0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_add0		\n"
			"_res_add0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_add0		\n"
			"_res_add0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_add0		\n"
			"_res_add0q:		\n\t"
			"movb $8, %0		\n"
			"_res_add0:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr;
		lt_adr = *(ff_addr_t*)ob;
		rt_adr = *(ff_addr_t*)(ob+__ADDR_TS);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = rt+lt;
		stack_put(__resin, (ff_u8_t*)&dst, l, rt_adr);
//		ffly_printf("%u, %u, %u\n", lt, rt, dst);
	}
	fi;

	__asm__("_res_inc0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_inc0		\n"
			"_res_inc0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_inc0		\n"
			"_res_inc0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_inc0		\n"
			"_res_inc0q:		\n\t"
			"movb $8, %0		\n"
			"_res_inc0:			\n\t" : "=m"(l));
	{
		ff_addr_t adr = *(ff_addr_t*)ob;
		ff_u64_t tmp = 0;
		stack_get(__resin, (ff_u8_t*)&tmp, l, adr);
		tmp++;
		stack_put(__resin, (ff_u8_t*)&tmp, l, adr);
	}
	fi;

	__asm__("_res_dec0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_dec0		\n"
			"_res_dec0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_dec0		\n"
			"_res_dec0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_dec0		\n"
			"_res_dec0q:		\n\t"
			"movb $8, %0		\n"
			"_res_dec0:			\n\t" : "=m"(l));
	{
		ff_addr_t adr = *(ff_addr_t*)ob;
		ff_u64_t tmp = 0;
		stack_get(__resin, (ff_u8_t*)&tmp, l, adr);
		tmp--;
		stack_put(__resin, (ff_u8_t*)&tmp, l, adr);
	}
	fi;

	__asm__("_res_cmp0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_cmp0		\n"
			"_res_cmp0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_cmp0		\n"
			"_res_cmp0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_cmp0		\n"
			"_res_cmp0q:		\n\t"
			"movb $8, %0		\n"
			"_res_cmp0:			\n\t" : "=m"(l));
	{
		ff_addr_t la, ra;
		la = *(ff_addr_t*)ob;
		ra = *(ff_addr_t*)(ob+__ADDR_TS);

		ff_u64_t lv = 0, rv = 0;
		stack_get(__resin, (ff_u8_t*)&lv, l, la);
		stack_get(__resin, (ff_u8_t*)&rv, l, ra);

		ff_u8_t flags = 0x0;
		if (lv>rv) flags |= _gt;
		if (lv<rv) flags |= _lt;
		if (lv==rv) flags |= _eq;
		*cflags = flags;
	}
	fi;

	__asm__("_res_as1br1:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_as1r1		\n"
			"_res_as1wr1:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_as1r1		\n"
			"_res_as1dr1:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_as1r1		\n"
			"_res_as1qr1:		\n\t"
			"movb $8, %0		\n"
			"_res_as1r1:		\n\t" : "=m"(l));
	copymem(ob, res_r1, l);
	fi;

	__asm__("_res_as1br0:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_as1r0		\n"
			"_res_as1wr0:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_as1r0		\n"
			"_res_as1dr0:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_as1r0		\n"
			"_res_as1qr0:		\n\t"
			"movb $8, %0		\n"
			"_res_as1r0:		\n\t" : "=m"(l));
	copymem(ob, res_r0, l);
	fi;

	__asm__("_res_as0b:			\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_as0		\n"
			"_res_as0w:			\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_as0		\n"
			"_res_as0d:			\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_as0		\n"
			"_res_as0q:			\n\t"
			"movb $8, %0		\n"
			"_res_as0:			\n\t" : "=m"(l));
	{
		ff_addr_t to;
		to = *(ff_addr_t*)ob;	

		ff_u64_t val;
		copymem(ob+__ADDR_TS, &val, l);
		stack_put(__resin, (ff_u8_t*)&val, l, to);
	}
	fi;

	__asm__("_res_cjmp0:\n\t"); {
		ff_addr_t adr = *(ff_addr_t*)ob;
		ff_s16_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_s16_t), adr);

		ff_u8_t flags;
		
		flags = *cflags;
		if (is_flag(flags, _gt)) {
			if (opno == _op_jg)
				goto _end;
		}

		if (is_flag(flags, _lt)) {
			if (opno == _op_jl)
				goto _end;
		}

		if (is_flag(flags, _eq)) {
			if (opno == _op_je)
				goto _end;
		} else {
			if (opno == _op_jne)
				goto _end;
		}

		fi;
	_end:
		__resin->set_ip(__resin->get_ip()+dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_res_jmp2:		\n\t");
	__resin->set_ip(__resin->get_ip()+*(ff_s16_t*)ob);
	__resin->ip_off = 0;
	fi;

	__asm__("_res_jmp1r1:	\n\t");
	__resin->set_ip(__resin->get_ip()+*(ff_s16_t*)res_r1);
	__resin->ip_off = 0;
	fi;
	__asm__("_res_jmp1r0:	\n\t");
	__resin->set_ip(__resin->get_ip()+*(ff_s16_t*)res_r0);
	__resin->ip_off = 0;
	fi;

	__asm__("_res_jmp8:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _res_jmp0	\n\t"
			"_res_jmp16:	\n\t"
			"movb $2, %0	" : "=m"(l));
	__asm__("_res_jmp0:	\n\t");
	{
		ff_addr_t adr = *(ff_addr_t*)ob;
		ff_s16_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_s16_t), adr);
//		ffly_printf("jump dis: %d, dst: %u\n", dst, __resin->get_ip()+dst);
		__resin->set_ip(__resin->get_ip()+dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_res_ld0b:			\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_ld0		\n"
			"_res_ld0w:			\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_ld0		\n"
			"_res_ld0d:			\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_ld0		\n"
			"_res_ld0q:			\n\t"
			"movb $8, %0		\n"
			"_res_ld0:			\n\t" : "=m"(l));
	{
		ff_addr_t dst = *(ff_addr_t*)ob;
		ff_addr_t src = *(ff_addr_t*)(ob+__ADDR_TS);
		ff_addr_t adr;
		stack_get(__resin, (ff_u8_t*)&adr, sizeof(ff_addr_t), dst);

		void *p = ff_resin_resolv_adr(__resin, adr);
		if (p != NULL)
			stack_get(__resin, (ff_u8_t*)p, l, src);
	}
	fi;

	__asm__("_res_st0b:			\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_st0		\n"
			"_res_st0w:			\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_st0		\n"
			"_res_st0d:			\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_st0		\n"
			"_res_st0q:			\n\t"
			"movb $8, %0		\n"
			"_res_st0:			\n\t" : "=m"(l));
	{
		ff_addr_t src = *(ff_addr_t*)ob;
		ff_addr_t dst = *(ff_addr_t*)(ob+__ADDR_TS);
		ff_addr_t adr;
		stack_get(__resin, (ff_u8_t*)&adr, sizeof(ff_addr_t), src);

		void *p = ff_resin_resolv_adr(__resin, adr);
		if (p != NULL)
			stack_put(__resin, (ff_u8_t*)p, l, dst);
	}
	fi;	

	/*
		memory to reg
	*/
	__asm__("_res_mov1br1:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mov1r1	\n"
			"_res_mov1wr1:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mov1r1	\n"
			"_res_mov1dr1:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mov1r1	\n"
			"_res_mov1qr1:		\n\t"
			"movb $8, %0		\n"
			"_res_mov1r1:		\n\t"
			"movq %3, %%r8		\n\t"
			"movq %%r8, %1		\n\t"
			"jmp _res_mov1		\n"

			"_res_mov1br0:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mov1r0	\n"
			"_res_mov1wr0:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mov1r0	\n"
			"_res_mov1dr0:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mov1r0	\n"
			"_res_mov1qr0:		\n\t"
			"movb $8, %0		\n"
			"_res_mov1r0:		\n\t"
			"movq %2, %%r8		\n\t"
			"movq %%r8, %1		\n"
			"_res_mov1:			\n\t" : "=m"(l), "=m"(r) : "m"(res_r0), "m"(res_r1));
	{
		ff_addr_t s = *(ff_addr_t*)ob;
		stack_get(__resin, (ff_u8_t*)r, l, s);
	}
	fi;

	/*
		reg to memory
	*/
	__asm__("_res_mov2br1:      \n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mov2r1    \n"
			"_res_mov2wr1:      \n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mov2r1    \n"
			"_res_mov2dr1:      \n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mov2r1    \n"
			"_res_mov2qr1:      \n\t"
			"movb $8, %0		\n"
			"_res_mov2r1:       \n\t"
			"movq %3, %%r8		\n\t"
			"movq %%r8, %1		\n\t"
			"jmp _res_mov2		\n"

			"_res_mov2br0:      \n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mov2r0	\n"
			"_res_mov2wr0:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mov2r0	\n"
			"_res_mov2dr0:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mov2r0	\n"
			"_res_mov2qr0:		\n\t"
			"movb $8, %0		\n"
			"_res_mov2r0:		\n\t"
			"movq %2, %%r8		\n\t"
			"movq %%r8, %1		\n"
			"_res_mov2:				\n\t" : "=m"(l), "=m"(r) : "m"(res_r0), "m"(res_r1));
	{
		ff_addr_t d = *(ff_addr_t*)ob;
		stack_put(__resin, (ff_u8_t*)r, l, d);
	}
	fi;

	__asm__("_res_mov0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_mov0		\n"
			"_res_mov0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_mov0		\n"
			"_res_mov0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_mov0		\n"
			"_res_mov0q:		\n\t"
			"movb $8, %0		\n"
			"_res_mov0:			\n\t" : "=m"(l));
	{
		ff_addr_t src = *(ff_addr_t*)ob;
		ff_addr_t dst = *(ff_addr_t*)(ob+__ADDR_TS);

		ff_u8_t tmp[8];
		stack_get(__resin, tmp, l, src);
		stack_put(__resin, tmp, l, dst);
	}
	fi;
	
	__asm__("_res_rin0:\n\t");
	{
		ff_u8_t no = *(ff_u8_t*)ob;
		ff_addr_t adr = *(ff_addr_t*)(ob+1);
		__resin->rin(no, ff_resin_resolv_adr(__resin, adr));
	}
	fi;

	__asm__("_res_out1br1:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_out1r1	\n"
			"_res_out1wr1:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_out1r1	\n"
			"_res_out1dr1:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_out1r1	\n"
			"_res_out1qr1:		\n\t"
			"movb $8, %0		\n"
			"_res_out1r1:		\n\t"
			"movq %3, %%r8		\n\t"
			"movq %%r8, %1		\n\t"
			"jmp _res_out1		\n"

			"_res_out1br0:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_out1r0	\n"
			"_res_out1wr0:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_out1r0	\n"
			"_res_out1dr0:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_out1r0	\n"
			"_res_out1qr0:      \n\t"
			"movb $8, %0		\n"
			"_res_out1r0:		\n\t"
			"movq %2, %%r8		\n\t"
			"movq %%r8, %1		\n\t"
			"_res_out1:			\n\t": "=m"(l), "=m"(r) : "m"(res_r0), "m"(res_r1));
	ffly_printf("out: %u\n", *r&(0xffffffffffffffff>>(64-(l*8))));
	fi;

	__asm__("_res_out0b:		\n\t"
			"movb $1, %0		\n\t"
			"jmp _res_out0		\n"
			"_res_out0w:		\n\t"
			"movb $2, %0		\n\t"
			"jmp _res_out0		\n"
			"_res_out0d:		\n\t"
			"movb $4, %0		\n\t"
			"jmp _res_out0		\n"
			"_res_out0q:		\n\t"
			"movb $8, %0		\n"
			"_res_out0:			\n\t" : "=m"(l));
	{
		ff_addr_t addr = *(ff_addr_t*)ob;
		ff_u64_t val = 0;
		stack_get(__resin, (ff_u8_t*)&val, l, addr);
		ffly_printf("out: %u\n", val);
	}
	fi;

	__asm__("_res_r3r1:		\n\t"
			"movq %5, %%r8	\n\t"
			"movq %%r8, %1	\n\t"
			"jmp _res_fi	\n"

			"_res_r2r1:		\n\t"
			"movq %4, %%r8	\n\t"
			"movq %%r8, %1	\n\t"
			"jmp _res_fi	\n"

			"_res_r1r1:		\n\t"
			"movq %3, %%r8	\n\t"
			"movq %%r8, %1	\n\t"
			"jmp _res_fi	\n"

			"_res_r0r1:		\n\t"
			"movq %2, %%r8	\n\t"
			"movq %%r8, %1	\n\t"
			"jmp _res_fi	\n"

			"_res_r3r0:	\n\t"
			"movq %5, %%r8	\n\t"
			"movq %%r8, %0	\n\t"
			"jmp _res_fi	\n"

			"_res_r2r0:	\n\t"
			"movq %4, %%r8	\n\t"
			"movq %%r8, %0	\n\t"
			"jmp _res_fi	\n"

			"_res_r1r0:	\n\t"
			"movq %3, %%r8	\n\t"
			"movq %%r8, %0	\n\t"
			"jmp _res_fi	\n"

			"_res_r0r0:		\n\t"
			"movq %2, %%r8	\n\t"
			"movq %%r8, %0	\n\t"
			"jmp _res_fi	\n" : "=m"(res_r0), "=m"(res_r1) : "m"(r0), "m"(r1), "m"(r2), "m"(r3));

	__asm__("_res_call0:\n\t");
	{
		ff_addr_t adr = *(ff_addr_t*)ob;
		ff_s16_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);
		*(__resin->retto++) = __resin->get_ip()+__resin->ip_off;
		__resin->set_ip(__resin->get_ip()+dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_res_ret:\n\t");
	{
		__resin->set_ip(*(--__resin->retto));
		__resin->ip_off = 0;
	}
	next;

	/*
		exit and use rp?? as the code
	*/
	__asm__("_res_exit2r1:		\n\t"
			"movq %1, %%r8		\n\t"
			"movb (%%r8), %%r8b	\n\t"
			"movb %%r8b, %0		\n\t"
			"jmp _res_end" : "=m"(code) : "m"(res_r1));
	__asm__("_res_exit2r0:		\n\t"
			"movq %1, %%r8		\n\t"
			"movb (%%r8), %%r8b	\n\t" 
			"movb %%r8b, %0		\n\t"
			"jmp _res_end" : "=m"(code) : "m"(res_r0));

	__asm__("_res_exit1:\n\t");
	copymem(ob, &code, 1);
	end;

	__asm__("_res_exit0:\n\t");
	{
		ff_addr_t adr = *(ff_addr_t*)ob;
		stack_get(__resin, (ff_u8_t*)&code, sizeof(ff_err_t), adr);
	}
	end;
	__asm__("_res_fi:\n\t");
	__resin->ip_incr(__resin->ip_off);
	next;

	__asm__("_res_end:\n\t");
	ffly_printf("goodbye.\n");
	if (__exit_code != NULL)
		*__exit_code = code;
}
