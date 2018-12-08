# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "resin.h"
# include "rdm.h"
# include "string.h"
#define MAX 0x38

/*
	redisign < move __p to static
*/
void static
op_exit(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_asb(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *p);
}

void static
op_asw(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u16_t*)p);
}

void static
op_asd(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u32_t*)p);
}

void static
op_asq(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u64_t*)p);
}

void static
op_jmp(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_st(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("src{%x}\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
}

void static
op_ld(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("dst{%x}\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("src{%x}\n", *(ff_addr_t*)p);
}

void static
op_out(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_mov(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("src{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
}

void static
op_rin(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("no{%x},\t\t", *(p++));
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_div(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x}\n", *(ff_addr_t*)p);
}

void static
op_mul(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x}\n", *(ff_addr_t*)p);
}

void static
op_sub(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x}\n", *(ff_addr_t*)p);
}

void static
op_add(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x}\n", *(ff_addr_t*)p);
}

void static
op_inc(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_dec(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_cmp(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x}\n", *(ff_addr_t*)p);
}

void static
op_cjmp(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x}\n", *(ff_addr_t*)p);
}

void static
op_call(ff_u8_t *__p, char const *__ident) {
	ff_u8_t *p = __p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)p);
}

void static
op_ret(ff_u8_t *__p, char const *__ident) {
	ffly_printf("%s\n", __ident);
}

char const *ident[] = {
	"exit",
	"asb",
	"asw",
	"asd",
	"asq",
	"jmp",
	"stb",
	"stw",
	"std",
	"stq",
	"ldb",
	"ldw",
	"ldd",
	"ldq",
	"outb",
	"outw",
	"outd",
	"outq",
	"movb",
	"movw",
	"movd",
	"movq",
	"rin",
	"divb",
	"divw",
	"divd",
	"divq",
	"mulb",
	"mulw",
	"muld",
	"mulq",
	"subb",
	"subw",
	"subd",
	"subq",
	"addb",
	"addw",
	"addd",
	"addq",
	"incb",
	"incw",
	"incd",
	"incq",
	"decb",
	"decw",
	"decd",
	"decq",
	"cmpb",
	"cmpw",
	"cmpd",
	"cmpq",
	"je",
	"jne",
	"jg",
	"jl",
	"call",
	"ret"
};

static void(*out[])(ff_u8_t*, char const*) = {
	op_exit,
	// assign
	op_asb,
	op_asw,
	op_asd,
	op_asq,
	op_jmp,
	// store
	op_st,
	op_st,
	op_st,
	op_st,
	// load
	op_ld,
	op_ld,
	op_ld,
	op_ld,
	// out
	op_out,
	op_out,
	op_out,
	op_out,
	// mov
	op_mov,
	op_mov,
	op_mov,
	op_mov,
	op_rin,
	// div
	op_div,
	op_div,
	op_div,
	op_div,
	// mul
	op_mul,
	op_mul,
	op_mul,
	op_mul,
	// sub
	op_sub,
	op_sub,
	op_sub,
	op_sub,
	// add
	op_add,
	op_add,
	op_add,
	op_add,
	// inc
	op_inc,
	op_inc,
	op_inc,
	op_inc,
	// dec
	op_dec,
	op_dec,
	op_dec,
	op_dec,
	// compare
	op_cmp,
	op_cmp,
	op_cmp,
	op_cmp,
	op_cjmp,
	op_cjmp,
	op_cjmp,
	op_cjmp,
	op_call,
	op_ret
};

/*
	TODO:
		__get() by chunks
*/
// used by rdm with file
void ffly_rdm(void(*__get)(ff_uint_t, ff_uint_t, void*), ff_u32_t __offset, ff_u32_t __end) {
	ff_u8_t buf[64];

	ff_u32_t cur = __offset;
	ff_u8_t s;
	ff_uint_t i = 0;
	while(cur-__offset < __end) {
		ff_u8_t op;
		__get(cur, 1, &op);	
		if (op>MAX) {
			ffly_printf("error malformed opno, got{%u}\n", op);
			break;
		}
		cur++;

		__get(cur, s = ff_resin_ops(op), buf);
		ffly_printf("%u-%u:(%x)\t\t", i++, cur-__offset, op);
		out[op](buf, ident[op]);
		cur+=s;
	}
}

// rdm with memory pointer
void ffly_rdmp(ff_u8_t *__p, ff_u32_t __end) {
	ff_u8_t *p = __p;
	ff_u8_t *end = p+__end;
	ff_uint_t i = 0;
	while(p != end) {
		ff_u8_t op;
		if ((op = *(p++)) > MAX) {
			ffly_printf("error malformed opno, got{%u}\n", op);
			break;
		}
		ffly_printf("%u-%u:(%x)\t\t", i++, (p-__p)-1, op);
		out[op](p, ident[op]);
		p+=ff_resin_ops(op);
	}
}

# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
int static fd;
void static
get(ff_uint_t __from, ff_uint_t __size, void *__buf) {
	pread(fd, __buf, __size, __from);
}

void ffly_rdmf(char const *__file, ff_u32_t __offset, ff_uint_t __size) {
	if ((fd = open(__file, O_RDONLY, 0)) < 0) {
		ffly_fprintf(ffly_out, "failed to open file\n");
		return;
	}

	ffly_rdm(get, __offset, __size);
	close(fd);
}
