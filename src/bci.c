# include "bci.h"
# include "system/errno.h"
# include "system/io.h"
# include "dep/mem_set.h"
# include "dep/mem_cpy.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
ff_err_t static
stack_put(ffly_bcip __bci, ff_u8_t *__src, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p = __src;
	ff_u8_t *end = p+__bc;
	ff_addr_t dst;

	while(p != end) {
		if ((dst = __addr+(p-__src)) >= __bci->stack_size) {
			// err
			return FFLY_FAILURE;
		}

		*(__bci->stack+dst) = *(p++);
	}
	return FFLY_SUCCESS;
}

ff_err_t static
stack_get(ffly_bcip __bci, ff_u8_t *__dst, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p = __dst;
	ff_u8_t *end = p+__bc;
	ff_addr_t src;

	while(p != end) {
		if ((src = __addr+(p-__dst)) >= __bci->stack_size) {
			// err
			return FFLY_FAILURE;
		}

		*(p++) = *(__bci->stack+src);	
	}
	return FFLY_SUCCESS;
}

void ffly_bci_sst(ffly_bcip __bci, void *__p, ff_addr_t __adr, ff_uint_t __n) {
	ffly_mem_cpy(__bci->stack+__adr, __p, __n);
}

void ffly_bci_sld(ffly_bcip __bci, ff_addr_t __adr, void *__p, ff_uint_t __n) {
	ffly_mem_cpy(__p, __bci->stack+__adr, __n);
}

ff_u8_t static
next_byte(void *__arg_p) {
	ffly_bcip p = (ffly_bcip)__arg_p;
	return p->fetch_byte(p->ip_off++);
}

# define get_4l(__bci) \
	ffly_get_bit(&__bci->bit, 4)

ff_u8_t static
get_8l(ffly_bcip __bci, ff_err_t *__err) {
	return ffly_get_bit(&__bci->bit, 8);
}

ff_u16_t static
get_16l(ffly_bcip __bci, ff_err_t *__err) {
	return ((ff_u16_t)get_8l(__bci, __err))|((ff_u16_t)get_8l(__bci, __err))<<8;
}

ff_u32_t static
get_32l(ffly_bcip __bci, ff_err_t *__err) {
	return ((ff_u32_t)get_16l(__bci, __err))|((ff_u32_t)get_16l(__bci, __err))<<16;
}

ff_u64_t static
get_64l(ffly_bcip __bci, ff_err_t *__err) {
	return ((ff_u64_t)get_32l(__bci, __err))|((ff_u64_t)get_32l(__bci, __err))<<32;
}

ff_err_t ffly_bci_init(ffly_bcip __bci) {
	ffly_init_get_bit(&__bci->bit, next_byte, (void*)__bci);
	__bci->stack = (ff_u8_t*)__ffly_mem_alloc(__bci->stack_size);
	ffly_mem_set(__bci->stack, 0xff, __bci->stack_size);
	__bci->retto = __bci->rtbuf;
	return FFLY_SUCCESS;
}

ff_err_t ffly_bci_de_init(ffly_bcip __bci) {
	__ffly_mem_free(__bci->stack);

	return FFLY_SUCCESS;
}

void static
get(ffly_bcip __bci, ff_u8_t *__dst, ff_u8_t __n, ff_err_t *__err) {
	ff_u8_t *p = __dst;
	ff_u8_t *end = p+__n;
	while(p != end) {
		*(p++) = get_8l(__bci, __err);
	}
}

void*
ffly_bci_resolv_adr(ffly_bcip __bci, ff_addr_t __addr) {
	return (void*)(__bci->stack+__addr);
}

void _exit();
void _as();
void _jmp();
void _st();
void _ld();
void _out();
void _mov();
void _rin();
void _arm(); // rename
void _inc_or_dec();
void _cmp();
void _cjmp();
void _call();
void _ret();
static void(*op[])() = {
	_exit,
	_as,
	_jmp,
	_st,
	_ld,
	_out,
	_mov,
	_rin,
	_arm,
	_arm,
	_arm,
	_arm,
	_inc_or_dec,
	_inc_or_dec,
	_cmp,
	_cjmp,
	_cjmp,
	_cjmp,
	_cjmp,
	_call,
	_ret
};

# define NOOP 21
# define get_addr(__bci, __err) \
	get_16l(__bci, __err)

# define fi __asm__("jmp _fi"); // finished
# define next __asm__("jmp _next")
# define end __asm__("jmp _end")
# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
# define errjmp if (_err(err)) jmpend
# include "system/io.h"
# include "system/nanosleep.h"
ff_err_t ffly_bci_exec(ffly_bcip __bci, ff_err_t *__exit_code) {
	ff_err_t err;

	__asm__("_next:\n\t");
	ffly_printf("next op.\n");
	{
	__bci->ip_off = 0;
	ff_u8_t opno;	
	if ((opno = get_8l(__bci, &err)) >= NOOP) {
		//ffly_errmsg("opno invalid, got: %u\n", opno);
		return FFLY_FAILURE;
	}
	ffly_nanosleep(0, 100000000); // <- debug
	jmpto(op[opno]);

	__asm__("_arm:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_u64_t lt, rt;
		ff_addr_t la, ra, dst_adr;

		la = get_addr(__bci, &err);
		ra = get_addr(__bci, &err);
		dst_adr = get_addr(__bci, &err);

		stack_get(__bci, (ff_u8_t*)&lt, l, la);
		stack_get(__bci, (ff_u8_t*)&rt, l, ra);
		ff_u64_t dst;

		ff_u64_t mask = 0xffffffffffffffff>>(64-(8<<l));
		lt &= mask;
		rt &= mask;
		switch(opno) {
			case _op_div:
				dst = lt/rt;
			break;
			case _op_mul:
				dst = lt-rt;
			break;
			case _op_sub:
				dst = lt-rt;
			break;
			case _op_add:
				dst = lt+rt;
			break;
		}

		stack_put(__bci, (ff_u8_t*)&dst, l, dst_adr);
	}
	fi;

	__asm__("_inc_or_dec:\n\t"); {
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t adr = get_addr(__bci, &err);
		ff_u64_t tmp = 0;
		stack_get(__bci, (ff_u8_t*)&tmp, l, adr);
		if (opno == _op_inc)
			tmp++;
		else if (opno == _op_dec)
			tmp--;
		stack_put(__bci, (ff_u8_t*)&tmp, l, adr);
	}
	fi;

	__asm__("_cmp:\n\t"); {
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t la, ra, dst;
		
		la = get_addr(__bci, &err);
		ra = get_addr(__bci, &err);
		dst = get_addr(__bci, &err);

		ff_u64_t lv = 0, rv = 0;
		stack_get(__bci, (ff_u8_t*)&lv, l, la);
		stack_get(__bci, (ff_u8_t*)&rv, l, ra);

		ff_u8_t flags = 0;
		if (lv>rv) flags |= _gt;
		if (lv<rv) flags |= _lt;
		if (lv==rv) flags |= _eq;
		stack_put(__bci, (ff_u8_t*)&flags, 1, dst);
	}
	fi;

	__asm__("_as:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t to = get_addr(__bci, &err);	
		ff_u64_t val;
		get(__bci, (ff_u8_t*)&val, l, &err);
		stack_put(__bci, (ff_u8_t*)&val, l, to);
	}
	fi;

	__asm__("_cjmp:\n\t"); {
		ff_addr_t adr = get_addr(__bci, &err);
		ff_addr_t dst;
		stack_get(__bci, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);

		ff_addr_t fa = get_addr(__bci, &err);
		ff_u8_t flags;
		stack_get(__bci, (ff_u8_t*)&flags, 1, fa);
	
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
		__bci->set_ip(dst);
		__bci->ip_off = 0;
	}
	next;

	__asm__("_jmp:\n\t");
	{
		ff_addr_t adr = get_addr(__bci, &err);
		ff_addr_t dst;
		stack_get(__bci, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);
		__bci->set_ip(dst);
		__bci->ip_off = 0;
	}
	next;

	__asm__("_ld:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t dst = get_addr(__bci, &err);
		ff_addr_t src = get_addr(__bci, &err);
		void *p = ffly_bci_resolv_adr(__bci, dst);
		stack_get(__bci, (ff_u8_t*)p, l, src);
	}
	fi;

	__asm__("_st:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t src = get_addr(__bci, &err);
		ff_addr_t dst = get_addr(__bci, &err);
		void *p = ffly_bci_resolv_adr(__bci, src);
		stack_put(__bci, (ff_u8_t*)p, l, dst);
	}
	fi;

	__asm__("_mov:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t src = get_addr(__bci, &err);
		ff_addr_t dst = get_addr(__bci, &err);
		ff_u8_t tmp[8];
		stack_get(__bci, tmp, l, src);
		stack_put(__bci, tmp, l, dst);
	}
	fi;
	
	__asm__("_rin:\n\t");
	{
		ff_u8_t no = get_8l(__bci, &err);
		__bci->rin(no, NULL);
	}
	fi;

	__asm__("_out:\n\t");
	{
		ff_u8_t l = get_8l(__bci, &err);
		ff_addr_t addr = get_addr(__bci, &err);
		ff_u64_t val = 0;
		stack_get(__bci, (ff_u8_t*)&val, l, addr);
		ffly_printf("%u\n", val);
	}
	fi;

	__asm__("_call:\n\t");
	{
		ff_addr_t adr = get_addr(__bci, &err);
		ff_addr_t dst;
		stack_get(__bci, (ff_u8_t*)&dst, sizeof(ff_addr_t), adr);
		*(__bci->retto++) = __bci->get_ip()+__bci->ip_off;
		__bci->set_ip(dst);
		__bci->ip_off = 0;
	}
	next;

	__asm__("_ret:\n\t");
	{
		__bci->set_ip(*(--__bci->retto));
		__bci->ip_off = 0;
	}
	next;

	__asm__("_exit:\n\t");
	{
		ff_addr_t addr = get_addr(__bci, &err);
		ff_err_t exit_code;
		if (__exit_code != NULL) {
			stack_get(__bci, (ff_u8_t*)&exit_code, sizeof(ff_err_t), addr);
			*__exit_code = exit_code;
		}
		
	}
	end;
	}

	__asm__("_fi:\n\t");
	__bci->ip_incr(__bci->ip_off);
	next;

	__asm__("_end:\n\t");
}
