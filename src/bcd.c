# include "types/err_t.h"
# include "stdio.h"
# include "malloc.h"
# include "bci.h"
# include "bcd.h"
# include "string.h"
# define MAX 19
void static
op_exit(mdl_u8_t **__p) {
	printf("exit,\t\t");
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_as(mdl_u8_t **__p) {
	printf("as,\t\t");
	mdl_u8_t l;
	printf("l{%u},\t\t", l = *(mdl_u8_t*)((*__p)++));
	printf("to{%x},\t\t", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	mdl_u64_t val = 0;
	memcpy(&val, *__p, l);
	printf("val{%u}\n", val);
	(*__p)+=l;
}

void static
op_jmp(mdl_u8_t **__p) {
	printf("jmp,\t\t");
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_st(mdl_u8_t **__p) {
	printf("st,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("src{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	printf("dst{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_ld(mdl_u8_t **__p) {
	printf("ld,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("dst{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	printf("src{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_out(mdl_u8_t **__p) {
	printf("out,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_mov(mdl_u8_t **__p) {
	printf("mov,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("src{%x},\t\t", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	printf("dst{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_rin(mdl_u8_t **__p) {
	printf("rin,\t\t");
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_arm(mdl_u8_t **__p) {
	mdl_u8_t *p = *__p;
	printf("arm,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)(p++));
    printf("lhs{%x},\t\t", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	printf("rhs{%x},\t\t", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	printf("dst{%x}\n", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	*__p = p;
}

void static
op_inc(mdl_u8_t **__p) {
	printf("inc,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_dec(mdl_u8_t **__p) {
	printf("dec,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)((*__p)++));
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static
op_cmp(mdl_u8_t **__p) {
	mdl_u8_t *p = *__p;
	printf("cmp,\t\t");
	printf("l{%u},\t\t", *(mdl_u8_t*)(p++));
	printf("lhs{%x},\t\t", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	printf("rhs{%x},\t\t", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	printf("dst{%x}\n", *(ffly_addr_t*)p);
	p+=sizeof(ffly_addr_t);
	*__p = p;

}

void static
op_cjmp(mdl_u8_t **__p) {
	printf("cjmp,\t\t");
	printf("to{%x},\t\t", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	printf("flags{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void static(*out[])(mdl_u8_t**) = {
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
	op_cjmp
};

void ffly_bcd(mdl_u8_t *__p, mdl_u8_t *__end) {
	mdl_u8_t *p = __p;
	mdl_u8_t *end = __end;
	while(p != end) {
		mdl_u8_t op;
		if ((op = *(p++)) > MAX-1) {
			printf("error malformed opno, got{%u}\n", op);
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
		fprintf(stderr, "failed to open file\n");
		return;
	}

	struct stat st;
	fstat(fd, &st);

	mdl_u8_t *p = (mdl_u8_t*)malloc(st.st_size);
	read(fd, p, st.st_size);
	close(fd);
	ffly_bcd(p, p+st.st_size);
	free(p);
}
