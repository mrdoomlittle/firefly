# include "bci.h"
# include "system/io.h"
# include "system/err.h"
# include "data/mem_set.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_err_t static
stack_put(ffly_bcip __bci, mdl_u8_t *__src, mdl_uint_t __bc, ffly_addr_t __addr) {
	mdl_u8_t *p = __src;
	mdl_u8_t *end = p+__bc;
	ffly_addr_t dst;

	while(p != end) {
		if ((dst = __addr+(p-__src)) >= __bci->stack_size) {
			// err
			reterr;
		}

		*(__bci->stack+dst) = *(p++);
	}

	retok;
}

ffly_err_t static
stack_get(ffly_bcip __bci, mdl_u8_t *__dst, mdl_uint_t __bc, ffly_addr_t __addr) {
	mdl_u8_t *p = __dst;
	mdl_u8_t *end = p+__bc;
	ffly_addr_t src;

	while(p != end) {
		if ((src = __addr+(p-__dst)) >= __bci->stack_size) {
			// err
			reterr;
		}

		*(p++) = *(__bci->stack+src);	
	}
	retok;
}

mdl_u8_t static
next_byte(void *__arg_p) {
	ffly_bcip p = (ffly_bcip)__arg_p;
	return p->fetch_byte(p->ip_off++);
}

# define get_4l(__bci) \
	ffly_get_bit(&__bci->bit, 4)

mdl_u8_t static
get_8l(ffly_bcip __bci, ffly_err_t *__err) {
	return ffly_get_bit(&__bci->bit, 8);
}

mdl_u16_t static
get_16l(ffly_bcip __bci, ffly_err_t *__err) {
	return ((mdl_u16_t)get_8l(__bci, __err))|((mdl_u16_t)get_8l(__bci, __err))<<8;
}

mdl_u32_t static
get_32l(ffly_bcip __bci, ffly_err_t *__err) {
	return ((mdl_u32_t)get_16l(__bci, __err))|((mdl_u32_t)get_16l(__bci, __err))<<16;
}

mdl_u64_t static
get_64l(ffly_bcip __bci, ffly_err_t *__err) {
	return ((mdl_u64_t)get_32l(__bci, __err))|((mdl_u64_t)get_32l(__bci, __err))<<32;
}

ffly_err_t ffly_bci_init(ffly_bcip __bci) {
	ffly_init_get_bit(&__bci->bit, next_byte, (void*)__bci);
	__bci->stack = (mdl_u8_t*)__ffly_mem_alloc(__bci->stack_size);
	ffly_mem_set(__bci->stack, 0xff, __bci->stack_size);

	retok;
}

ffly_err_t ffly_bci_de_init(ffly_bcip __bci) {
	__ffly_mem_free(__bci->stack);

	retok;
}

void static
get(ffly_bcip __bci, mdl_u8_t *__dst, mdl_u8_t __n, ffly_err_t *__err) {
	mdl_u8_t *p = __dst;
	mdl_u8_t *end = p+__n;
	while(p != end) {
		*(p++) = get_8l(__bci, __err);
	}
}

void _exit();
void _as();
void _jmp();
static void(*op[])() = {
	_exit,
	_as,
	_jmp
};

# define get_addr(__bci, __err) \
	get_16l(__bci, __err)

# define fi __asm__("jmp _fi");
# define next __asm__("jmp _next")
# define end __asm__("jmp _end")
# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
# define errjmp if (_err(err)) jmpend
# include "system/io.h"
ffly_err_t ffly_bci_exec(ffly_bcip __bci, ffly_err_t *__exit_code) {
	ffly_err_t err;

	__asm__("_next:\n\t");
	{
	__bci->ip_off = 0;
	mdl_u8_t opcode = get_8l(__bci, &err);
	jmpto(op[opcode]);

	__asm__("_as:\n\t");
	{
		mdl_u8_t l = get_8l(__bci, &err);
		ffly_addr_t to = get_addr(__bci, &err);	
		mdl_u64_t val;
		get(__bci, (mdl_u8_t*)&val, l, &err);
		stack_put(__bci, (mdl_u8_t*)&val, l, to);
	}
	fi;

	__asm__("_jmp:\n\t");
	{
		ffly_addr_t addr = get_addr(__bci, &err);
		ffly_addr_t dst;
		stack_get(__bci, (mdl_u8_t*)&dst, sizeof(ffly_addr_t), addr);
		ffly_printf("... %u\n", addr);
		__bci->set_ip(dst);
	}
	fi;

	__asm__("_exit:\n\t");
	{
		ffly_addr_t addr = get_addr(__bci, &err);
		ffly_err_t exit_code;
		if (__exit_code != NULL) {
			stack_get(__bci, (mdl_u8_t*)&exit_code, sizeof(ffly_err_t), addr);
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