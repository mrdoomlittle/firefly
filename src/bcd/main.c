# include "../types/err_t.h"
# include "../stdio.h"
# include "../system/err.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../bci.h"
# include <mdlint.h>

# define MAX 7
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
	printf("to{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t)+l;
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
	printf("src{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
	printf("dst{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void(*out[])(mdl_u8_t**) = {
	op_exit,
	op_as,
	op_jmp,
	op_st,
	op_ld,
	op_out,
	op_mov
};

# include "../ffef.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("please provide binfile.\n");
		reterr;
	}

	char const *bin = __argv[1];

	int fd;
	if ((fd = open(bin, O_RDONLY, 0)) < 0) {
		fprintf(stderr, "failed to open file\n");
		reterr;
	}

	struct stat st;
	stat(bin, &st);

	mdl_u8_t *bed = (mdl_u8_t*)malloc(st.st_size);
	lseek(fd, ffef_hdr_size, SEEK_SET);
	read(fd, bed, st.st_size-ffef_hdr_size);
	close(fd);

	mdl_u8_t *p = bed;
	mdl_u8_t *end = p+(st.st_size-ffef_hdr_size);
	while(p != end) {
		mdl_u8_t op;	
		if ((op = *(p++)) > MAX-1) {
			printf("error malformed opcode, got{%u}\n", op);
			break;
		}

		out[op](&p);
	}

	free(bed);
	retok;
}
