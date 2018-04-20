# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "bci.h"
# include "bcd.h"
# include "string.h"
# define MAX 21
void static
op_exit(ff_u8_t **__p) {
	ffly_printf("exit,\t\t");
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_as(ff_u8_t **__p) {
	ffly_printf("as,\t\t");
	ff_u8_t l;
	ffly_printf("l{%u},\t\t", l = *(ff_u8_t*)((*__p)++));
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ff_u64_t val = 0;
	memcpy(&val, *__p, l);
	ffly_printf("val{%u}\n", val);
	(*__p)+=l;
}

void static
op_jmp(ff_u8_t **__p) {
	ffly_printf("jmp,\t\t");
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_st(ff_u8_t **__p) {
	ffly_printf("st,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("src{%x}\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_ld(ff_u8_t **__p) {
	ffly_printf("ld,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("dst{%x}\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ffly_printf("src{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_out(ff_u8_t **__p) {
	ffly_printf("out,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_mov(ff_u8_t **__p) {
	ffly_printf("mov,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("src{%x},\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_rin(ff_u8_t **__p) {
	ffly_printf("rin,\t\t");
	ffly_printf("no{%x}\n", *(ff_u8_t*)((*__p)++));
}

void static
op_arm(ff_u8_t **__p) {
	ff_u8_t *p = *__p;
	ffly_printf("arm,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)(p++));
    ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;
}

void static
op_inc(ff_u8_t **__p) {
	ffly_printf("inc,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_dec(ff_u8_t **__p) {
	ffly_printf("dec,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)((*__p)++));
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_cmp(ff_u8_t **__p) {
	ff_u8_t *p = *__p;
	ffly_printf("cmp,\t\t");
	ffly_printf("l{%u},\t\t", *(ff_u8_t*)(p++));
	ffly_printf("lhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("rhs{%x},\t\t", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	ffly_printf("dst{%x}\n", *(ff_addr_t*)p);
	p+=sizeof(ff_addr_t);
	*__p = p;

}

void static
op_cjmp(ff_u8_t **__p) {
	ffly_printf("cjmp,\t\t");
	ffly_printf("to{%x},\t\t", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
	ffly_printf("flags{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void static
op_call(ff_u8_t **__p) {
	ffly_printf("call,\t\t");
	ffly_printf("adr{%x}\n", *(ff_addr_t*)*__p);
	(*__p)+=sizeof(ff_addr_t);
}

void op_ret(ff_u8_t **__p) {
	ffly_printf("ret\n");
}

void static(*out[])(ff_u8_t**) = {
	op_exit,
	op_as,
	op_jmp,
	op_st,
	op_ld,
	op_out,
	op_mov,
	op_rin,
	op_arm,
	op_arm,
	op_arm,
	op_arm,
	op_inc,
	op_dec,
	op_cmp,
	op_cjmp,
	op_cjmp,
	op_cjmp,
	op_cjmp,
	op_call,
	op_ret
};

void ffly_bcd(ff_u8_t *__p, ff_u8_t *__end) {
	ff_u8_t *p = __p;
	ff_u8_t *end = __end;
	while(p != end) {
		ff_u8_t op;
		if ((op = *(p++)) > MAX-1) {
			ffly_printf("error malformed opno, got{%u}\n", op);
			break;
		}
		out[op](&p);
	}
}

# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
void ffly_bcdf(char const *__file) {
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
	ffly_bcd(p, p+st.st_size);
	__ffly_mem_free(p);
}
