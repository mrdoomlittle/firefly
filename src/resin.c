# include "resin.h"
# include "system/errno.h"
# include "system/io.h"
# include "dep/mem_set.h"
# include "dep/mem_cpy.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
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
	*(r++) = &__resin->r0;
	*(r++) = &__resin->r1;
	*(r++) = &__resin->r2;
	*(r++) = &__resin->r3;
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

*/
ff_u8_t static ops[] = {
	sizeof(ff_addr_t),		// exit

	sizeof(ff_addr_t)+1,	// asb
	sizeof(ff_addr_t)+2,	// asw
	sizeof(ff_addr_t)+4,	// asd
	sizeof(ff_addr_t)+8,	// asq

	sizeof(ff_addr_t),		// jmp

	sizeof(ff_addr_t)*2,	// stb
	sizeof(ff_addr_t)*2,	// stw
	sizeof(ff_addr_t)*2,	// std
	sizeof(ff_addr_t)*2,	// stq

	sizeof(ff_addr_t)*2,    // ldb
	sizeof(ff_addr_t)*2,    // ldw
	sizeof(ff_addr_t)*2,    // ldd
	sizeof(ff_addr_t)*2,    // ldq

	sizeof(ff_addr_t),		// outb
	sizeof(ff_addr_t),		// outw
	sizeof(ff_addr_t),		// outd
	sizeof(ff_addr_t),		// outq

	sizeof(ff_addr_t)*2,	// movb
	sizeof(ff_addr_t)*2,	// movw
	sizeof(ff_addr_t)*2,	// movd
	sizeof(ff_addr_t)*2,	// movq

	sizeof(ff_addr_t)+1,	// rin

	sizeof(ff_addr_t)*3,	// divb
	sizeof(ff_addr_t)*3,	// divw
	sizeof(ff_addr_t)*3,	// divd
	sizeof(ff_addr_t)*3,	// divq

	sizeof(ff_addr_t)*3,	// mulb
	sizeof(ff_addr_t)*3,	// mulw
	sizeof(ff_addr_t)*3,	// muld
	sizeof(ff_addr_t)*3,	// mulq

	sizeof(ff_addr_t)*3,	// subb
	sizeof(ff_addr_t)*3,	// subw
	sizeof(ff_addr_t)*3,	// subd
	sizeof(ff_addr_t)*3,	// subq

	sizeof(ff_addr_t)*3,	// addb
	sizeof(ff_addr_t)*3,	// addw
	sizeof(ff_addr_t)*3,	// addd
	sizeof(ff_addr_t)*3,	// addq

	sizeof(ff_addr_t),		// incb
	sizeof(ff_addr_t),		// incw
	sizeof(ff_addr_t),		// incd
	sizeof(ff_addr_t),		// incq

	sizeof(ff_addr_t),		// decb
	sizeof(ff_addr_t),		// decw
	sizeof(ff_addr_t),		// decd
	sizeof(ff_addr_t),		// decq

	sizeof(ff_addr_t)*3,	// cmpb
	sizeof(ff_addr_t)*3,	// cmpw
	sizeof(ff_addr_t)*3,	// cmpd
	sizeof(ff_addr_t)*3,	// cmpq

	sizeof(ff_addr_t)*2,	// cjmp
	sizeof(ff_addr_t)*2,	// cjmp
	sizeof(ff_addr_t)*2,	// cjmp
	sizeof(ff_addr_t)*2,	// cjmp

	sizeof(ff_addr_t),		// call
	0						// ret
};

ff_u8_t ff_resin_ops(ff_u8_t __op) {
	return ops[__op];
}

void*
ff_resin_resolv_adr(ffly_resinp __resin, ff_addr_t __adr) {
	return (void*)(__resin->stack+__adr);
}

void _exit0();

void _as0b();
void _as0w();
void _as0d();
void _as0q();

void _jmp();

void _stb();
void _stw();
void _std();
void _stq();

void _ldb();
void _ldw();
void _ldd();
void _ldq();

void _out0b();
void _out0w();
void _out0d();
void _out0q();

void _mov0b();
void _mov0w();
void _mov0d();
void _mov0q();

void _rin();

void _divb();
void _divw();
void _divd();
void _divq();

void _mulb();
void _mulw();
void _muld();
void _mulq();

void _subb();
void _subw();
void _subd();
void _subq();

void _addb();
void _addw();
void _addd();
void _addq();

void _incb();
void _incw();
void _incd();
void _incq();

void _decb();
void _decw();
void _decd();
void _decq();

void _cmpb();
void _cmpw();
void _cmpd();
void _cmpq();

void _cjmp();
void _call();
void _ret();

void _as1br0();
void _as1wr0();
void _as1dr0();
void _as1qr0();

void _mov1br0();
void _mov1wr0();
void _mov1dr0();
void _mov1qr0();

void _mov2br0();
void _mov2wr0();
void _mov2dr0();
void _mov2qr0();

void _out1br0();
void _out1wr0();
void _out1dr0();
void _out1qr0();

static void(*op[])() = {
	_exit0,
	// assign
	_as0b,
	_as0w,
	_as0d,
	_as0q,	
	// jump
	_jmp,
	// store
	_stb,
	_stw,
	_std,
	_stq,
	// load
	_ldb,
	_ldw,
	_ldd,
	_ldq,
	// out
	_out0b,
	_out0w,
	_out0d,
	_out0q,
	// move
	_mov0b,
	_mov0w,
	_mov0d,
	_mov0q,
	// ring
	_rin,
	// dev
	_divb,
	_divw,
	_divd,
	_divq,
	// mul
	_mulb,
	_mulw,
	_muld,
	_mulq,
	// sub
	_subb,
	_subw,
	_subd,
	_subq,
	// add
	_addb,
	_addw,
	_addd,
	_addq,
	// incr
	_incb,
	_incw,
	_incd,
	_incq,
	// decr
	_decb,
	_decw,
	_decd,
	_decq,
	// compare
	_cmpb,
	_cmpw,
	_cmpd,
	_cmpq,
	// je
	_cjmp,
	// jne
	_cjmp,
	// jg
	_cjmp,
	// jl
	_cjmp,

	_call,
	_ret,

	_as1br0,
	_as1wr0,
	_as1dr0,
	_as1qr0,

	_mov1br0,
	_mov1wr0,
	_mov1dr0,
	_mov1qr0,

	_mov2br0,
	_mov2wr0,
	_mov2dr0,
	_mov2qr0,
	
	_out1br0,
	_out1wr0,
	_out1dr0,
	_out1qr0
};
/*
	as im using asm labels it may cause issues
	ive checked the assembly output and it allocates
	memory for variables on function entry so i dont think
	it should cause any problems if it does re will have to 
	we think how we deal with variables.
*/

/*
	a lot of repeating of same code, why? 
	to reduce number of machine instructions executed.

	if we did it diffrently it would be slower and speed
	is the most important thing for this.
*/
# define MAX _op_out1qr0
# define get_addr(__resin, __err) \
	get_16l(__resin, __err)

# define fi __asm__("jmp _fi"); // finished
# define next __asm__("jmp _next")
# define end __asm__("jmp _end")
# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
# define errjmp if (_err(err)) jmpend
# include "system/io.h"
# include "system/nanosleep.h"
ff_err_t ff_resin_exec(ffly_resinp __resin, ff_err_t *__exit_code) {
	ff_err_t err;
	ff_u8_t opno, l;
	ff_err_t code;

	ff_u64_t *r0, *r1, *r2, *r3;

	r0 = &__resin->r0;
	r1 = &__resin->r1;
	r2 = &__resin->r2;
	r3 = &__resin->r3;

	__asm__("_next:\n\t");
	__resin->ip_off = 0;	
	if ((opno = get_8l(__resin, &err)) > MAX) {
		ffly_printf("opno invalid, got: %u\n", opno);
		return FFLY_FAILURE;
	}

	jmpto(op[opno]);

	__asm__("_divb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _div		\n"
			"_divw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _div		\n"
			"_divd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _div		\n"
			"_divq:			\n\t"
			"movb $8, %0	\n"
			"_div:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr, dst_adr;
		lt_adr = get_addr(__resin, &err);
		rt_adr = get_addr(__resin, &err);
		dst_adr = get_addr(__resin, &err);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = lt/rt;
		stack_put(__resin, (ff_u8_t*)&dst, l, dst_adr);
	}
	fi;

	__asm__("_mulb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _mul		\n"
			"_mulw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _mul		\n"
			"_muld:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _mul		\n"
			"_mulq:			\n\t"
			"movb $8, %0	\n"
			"_mul:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr, dst_adr;
		lt_adr = get_addr(__resin, &err);
		rt_adr = get_addr(__resin, &err);
		dst_adr = get_addr(__resin, &err);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(l*8));
		lt &= mask;
		rt &= mask;

		dst = lt*rt;
		stack_put(__resin, (ff_u8_t*)&dst, l, dst_adr);
	}
	fi;

	__asm__("_subb:			\n\t"
			"movb $1, %0	\n\t"		
			"jmp _sub		\n"
			"_subw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _sub		\n"
			"_subd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _sub		\n"
			"_subq:			\n\t"
			"movb $8, %0	\n"
			"_sub:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr, dst_adr;
		lt_adr = get_addr(__resin, &err);
		rt_adr = get_addr(__resin, &err);
		dst_adr = get_addr(__resin, &err);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(8<<l));
		lt &= mask;
		rt &= mask;

		dst = lt-rt;
		stack_put(__resin, (ff_u8_t*)&dst, l, dst_adr);
	}
	fi;


	__asm__("_addb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _add		\n"
			"_addw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _add		\n"
			"_addd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _add		\n"
			"_addq:			\n\t"
			"movb $8, %0	\n"
			"_add:			\n\t" : "=m"(l));
	{
		ff_u64_t lt, rt;
		ff_addr_t lt_adr, rt_adr, dst_adr;
		lt_adr = get_addr(__resin, &err);
		rt_adr = get_addr(__resin, &err);
		dst_adr = get_addr(__resin, &err);

		stack_get(__resin, (ff_u8_t*)&lt, l, lt_adr);
		stack_get(__resin, (ff_u8_t*)&rt, l, rt_adr);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(4<<l));
		lt &= mask;
		rt &= mask;

		dst = lt+rt;
		stack_put(__resin, (ff_u8_t*)&dst, l, dst_adr);
	}
	fi;
/*
	__asm__("_inc1br0:		\n\t"
			"push %%rax		\n\t"
			"push %%rdi		\n\t"
			"movq %0, %%rdi	\n\t"
			"movb (%%rdi), %%al	\n\t"
			"decb %%al		\n\t"
			"movb %%al, (%%rdi)	\n\t"
			"pop %%rdi		\n\t"
			"pop %%rax		\n\t"
			"jmp _fi" : : "m"(r0));

	__asm__("_inc1wr0:      \n\t"
			"push %%rax     \n\t"
			"push %%rdi		\n\t"
			"movq %0, %%rdi \n\t"
			"movw (%%rdi), %%ax	\n\t"
			"decw %%ax      \n\t"
			"movw %%ax, (%%rdi)	\n\t"
			"pop %%rdi		\n\t"
			"pop %%rax      \n\t"
			"jmp _fi" : : "m"(r0));

	__asm__("_inc1dr0:      \n\t"
			"push %%rax     \n\t"
			"push %%rdi		\n\t"
			"movq %0, %%rdi \n\t"
			"movl (%%rdi), %%eax	\n\t"
			"decl %%eax      \n\t"
			"movl %%eax, (%%rdi)	\n\t"
			"pop %%rdi		\n\t"
			"pop %%rax      \n\t"
			"jmp _fi" : : "m"(r0));

	__asm__("_inc1qr0:      \n\t"
			"push %%rax     \n\t"
			"push %%rdi		\n\t"
			"movq %0, %%rdi \n\t"
			"movq (%%rdi), %%rax	\n\t"
			"decq %%rax      \n\t"
			"movq %%rax, (%%rdi)	\n\t"
			"pop %%rdi		\n\t"
			"pop %%rax      \n\t"
			"jmp _fi" : : "m"(r0));
*/
	__asm__("_incb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _inc		\n"
			"_incw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _inc		\n"
			"_incd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _inc		\n"
			"_incq:			\n\t"
			"movb $8, %0	\n"
			"_inc:			\n\t" : "=m"(l));
	{
		ff_addr_t adr = get_addr(__resin, &err);
		ff_u64_t tmp = 0;
		stack_get(__resin, (ff_u8_t*)&tmp, l, adr);
		tmp++;
		stack_put(__resin, (ff_u8_t*)&tmp, l, adr);
	}
	fi;

	__asm__("_decb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _dec		\n"
			"_decw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _dec		\n"
			"_decd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _dec		\n"
			"_decq:			\n\t"
			"movb $8, %0	\n"
			"_dec:			\n\t" : "=m"(l));
	{
		ff_addr_t adr = get_addr(__resin, &err);
		ff_u64_t tmp = 0;
		stack_get(__resin, (ff_u8_t*)&tmp, l, adr);
		tmp--;
		stack_put(__resin, (ff_u8_t*)&tmp, l, adr);
	}
	fi;

	__asm__("_cmpb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _cmp		\n"
			"_cmpw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _cmp		\n"
			"_cmpd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _cmp		\n"
			"_cmpq:			\n\t"
			"movb $8, %0	\n"
			"_cmp:			\n\t" : "=m"(l));
	{
		ff_addr_t la, ra, dst;
		
		la = get_addr(__resin, &err);
		ra = get_addr(__resin, &err);
		dst = get_addr(__resin, &err);

		ff_u64_t lv = 0, rv = 0;
		stack_get(__resin, (ff_u8_t*)&lv, l, la);
		stack_get(__resin, (ff_u8_t*)&rv, l, ra);

		ff_u8_t flags = 0x0;
		if (lv>rv) flags |= _gt;
		if (lv<rv) flags |= _lt;
		if (lv==rv) flags |= _eq;
		stack_put(__resin, (ff_u8_t*)&flags, 1, dst);
	}
	fi;

	__asm__("_as1br0:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _as1r0		\n"
			"_as1wr0:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _as1r0		\n"
			"_as1dr0:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _as1r0		\n"
			"_as1qr0:		\n\t"
			"movb $8, %0	\n"
			"_as1r0:		\n\t" : "=m"(l));
	get(__resin, (ff_u8_t*)&__resin->r0, l, &err);
	fi;

	__asm__("_as0b:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _as0		\n"
			"_as0w:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _as0		\n"
			"_as0d:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _as0		\n"
			"_as0q:			\n\t"
			"movb $8, %0	\n"
			"_as0:			\n\t" : "=m"(l));
	{
		ff_addr_t to;
		to = get_addr(__resin, &err);	

		ff_u64_t val;
		get(__resin, (ff_u8_t*)&val, l, &err);
		stack_put(__resin, (ff_u8_t*)&val, l, to);
	}
	fi;

	__asm__("_cjmp:\n\t"); {
		ff_addr_t adr = get_addr(__resin, &err);
		ff_addr_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);

		ff_addr_t fa = get_addr(__resin, &err);
		ff_u8_t flags;
		stack_get(__resin, (ff_u8_t*)&flags, 1, fa);
	
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
		__resin->set_ip(dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_jmp:\n\t");
	{
		ff_addr_t adr = get_addr(__resin, &err);
		ff_addr_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);
		__resin->set_ip(dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_ldb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _ld		\n"
			"_ldw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _ld		\n"
			"_ldd:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _ld		\n"
			"_ldq:			\n\t"
			"movb $8, %0	\n"
			"_ld:			\n\t" : "=m"(l));
	{
		ff_addr_t dst = get_addr(__resin, &err);
		ff_addr_t src = get_addr(__resin, &err);
		ff_addr_t adr;
		stack_get(__resin, (ff_u8_t*)&adr, sizeof(ff_addr_t), dst);

		void *p = ff_resin_resolv_adr(__resin, adr);
		stack_get(__resin, (ff_u8_t*)p, l, src);
	}
	fi;

	__asm__("_stb:			\n\t"
			"movb $1, %0	\n\t"
			"jmp _st		\n"
			"_stw:			\n\t"
			"movb $2, %0	\n\t"
			"jmp _st		\n"
			"_std:			\n\t"
			"movb $4, %0	\n\t"
			"jmp _st		\n"
			"_stq:			\n\t"
			"movb $8, %0	\n"
			"_st:			\n\t" : "=m"(l));
	{
		ff_addr_t src = get_addr(__resin, &err);
		ff_addr_t dst = get_addr(__resin, &err);
		ff_addr_t adr;
		stack_get(__resin, (ff_u8_t*)&adr, sizeof(ff_addr_t), src);

		void *p = ff_resin_resolv_adr(__resin, adr);
		stack_put(__resin, (ff_u8_t*)p, l, dst);
	}
	fi;	

	/*
		memory to reg0
	*/
	__asm__("_mov1br0:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _mov1r0	\n"
			"_mov1wr0:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _mov1r0	\n"
			"_mov1dr0:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _mov1r0	\n"
			"_mov1qr0:		\n\t"
			"movb $8, %0	\n"
			"_mov1r0:		\n\t" : "=m"(l));
	{
		ff_addr_t s = get_addr(__resin, &err);
		stack_get(__resin, (ff_u8_t*)&__resin->r0, l, s);
	}
	fi;

	/*
		reg0 to memory
	*/
	__asm__("_mov2br0:      \n\t"
			"movb $1, %0    \n\t"
			"jmp _mov2r0    \n"
			"_mov2wr0:      \n\t"
			"movb $2, %0    \n\t"
			"jmp _mov2r0    \n"
			"_mov2dr0:      \n\t"
			"movb $4, %0    \n\t"
			"jmp _mov2r0    \n"
			"_mov2qr0:      \n\t"
			"movb $8, %0    \n"
			"_mov2r0:       \n\t" : "=m"(l));
	{
		ff_addr_t d = get_addr(__resin, &err);
		stack_put(__resin, (ff_u8_t*)&__resin->r0, l, d);
	}
	fi;

	__asm__("_mov0b:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _mov0		\n"
			"_mov0w:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _mov0		\n"
			"_mov0d:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _mov0		\n"
			"_mov0q:		\n\t"
			"movb $8, %0	\n"
			"_mov0:			\n\t" : "=m"(l));
	{
		ff_addr_t src = get_addr(__resin, &err);
		ff_addr_t dst = get_addr(__resin, &err);

		ff_u8_t tmp[8];
		stack_get(__resin, tmp, l, src);
		stack_put(__resin, tmp, l, dst);
	}
	fi;
	
	__asm__("_rin:\n\t");
	{
		ff_u8_t no = get_8l(__resin, &err);
		ff_addr_t adr = get_addr(__resin, &err);
		__resin->rin(no, ff_resin_resolv_adr(__resin, adr));
	}
	fi;

	__asm__("_out1br0:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _out1r0	\n"
			"_out1wr0:		\n\t"
			"movb $2, %0    \n\t"
			"jmp _out1r0   \n"
			"_out1dr0:      \n\t"
			"movb $4, %0    \n\t"
			"jmp _out1r0   \n"
			"_out1qr0:      \n\t"
			"movb $8, %0	\n"
			"_out1r0:		\n\t" : "=m"(l));
	ffly_printf("out: %u\n", __resin->r0&(0xffffffffffffffff>>(64-(l*8))));
	fi;

	__asm__("_out0b:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _out0		\n"
			"_out0w:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _out0		\n"
			"_out0d:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _out0		\n"
			"_out0q:		\n\t"
			"movb $8, %0	\n"
			"_out0:			\n\t" : "=m"(l));
	{
		ff_addr_t addr = get_addr(__resin, &err);
		ff_u64_t val = 0;
		stack_get(__resin, (ff_u8_t*)&val, l, addr);
		ffly_printf("out: %u\n", val);
	}
	fi;

	__asm__("_call:\n\t");
	{
		ff_addr_t adr = get_addr(__resin, &err);
		ff_addr_t dst;
		stack_get(__resin, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);
		*(__resin->retto++) = __resin->get_ip()+__resin->ip_off;
		__resin->set_ip(dst);
		__resin->ip_off = 0;
	}
	next;

	__asm__("_ret:\n\t");
	{
		__resin->set_ip(*(--__resin->retto));
		__resin->ip_off = 0;
	}
	next;
	__asm__("_exit2r3:		\n\t"
			"push %%rax		\n\t"
			"movb %1, %%al	\n\t" 
			"movb %%al, %0	\n\t"
			"pop %%rax		\n\t"
			"jmp _end" : "=m"(code) : "m"(__resin->r3));
	__asm__("_exit2r2:		\n\t"
			"push %%rax		\n\t"
			"movb %1, %%al	\n\t" 
			"movb %%al, %0	\n\t"
			"pop %%rax		\n\t"
			"jmp _end" : "=m"(code) : "m"(__resin->r2));
	__asm__("_exit2r1:		\n\t"
			"push %%rax		\n\t"
			"movb %1, %%al	\n\t" 
			"movb %%al, %0	\n\t"
			"pop %%rax		\n\t"
			"jmp _end" : "=m"(code) : "m"(__resin->r1));
	__asm__("_exit2r0:		\n\t"
			"push %%rax		\n\t"
			"movb %1, %%al	\n\t" 
			"movb %%al, %0	\n\t"
			"pop %%rax		\n\t"
			"jmp _end" : "=m"(code) : "m"(__resin->r0));
	__asm__("_exit1:\n\t");
	get(__resin, (ff_u8_t*)&code, 1, &err);
	end;
	__asm__("_exit0:\n\t");
	{
		ff_addr_t adr = get_addr(__resin, &err);
		stack_get(__resin, (ff_u8_t*)&code, sizeof(ff_err_t), adr);
	}
	end;
	
	__asm__("_fi:\n\t");
	__resin->ip_incr(__resin->ip_off);
	next;

	__asm__("_end:\n\t");
	ffly_printf("goodbye.\n");
	if (__exit_code != NULL)
		*__exit_code = code;
}
