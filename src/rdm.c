# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "resin.h"
# include "rdm.h"
# include "string.h"
# define MAX 0x38
void static
op_exit(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_asb(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u8_t*)(p++));
	*__p = p;
}

void static
op_asw(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u16_t*)p);
	p+=sizeof(ff_u16_t);
	*__p = p;
}

void static
op_asd(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u32_t*)p);
	p+=sizeof(ff_u32_t);
	*__p = p;
}

void static
op_asq(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("val{%u}\n", *(ff_u64_t*)p);
	p+=sizeof(ff_u64_t);
	*__p = p;
}

void static
op_jmp(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_st(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("src{%x}\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_ld(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("dst{%x}\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("src{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_out(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_mov(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("src{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_rin(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("no{%x},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_div(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_mul(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_sub(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_add(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_inc(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_dec(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_cmp(ff_u8_t **__p, char const *__ident) {
	ff_u8_t *p = *__p;
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;

}

void static
op_cjmp(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ffly_printf("flags{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_call(ff_u8_t **__p, char const *__ident) {
	ffly_printf("%s,\t\t", __ident);
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void op_ret(ff_u8_t **__p, char const *__ident) {
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

void static(*out[])(ff_u8_t**, char const*) = {
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

void ffly_rdm(ff_u8_t *__p, ff_u8_t *__end) {
	ff_u8_t *p = __p;
	ff_u8_t *end = __end;
	ff_uint_t i = 0;
	while(p != end) {
		ff_u8_t op;
		if ((op = *(p++)) > MAX) {
			ffly_printf("error malformed opno, got{%u}\n", op);
			break;
		}
		ffly_printf("%u-%u:(%x)\t\t", i++, p-__p, op);
		out[op](&p, ident[op]);
	}
}

# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
void ffly_rdmf(char const *__file) {
	int fd;
	if ((fd = open(__file, O_RDONLY, 0)) < 0) {
		ffly_fprintf(ffly_out, "failed to open file\n");
		return;
	}

	struct stat st;
	fstat(fd, &st);

	ff_u8_t *p = (ff_u8_t*)__ffly_mem_alloc(st.st_size);
	read(fd, p, st.st_size);
	close(fd);
	ffly_rdm(p, p+st.st_size);
	__ffly_mem_free(p);
}
