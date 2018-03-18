# include "../types/err_t.h"
# include "../stdio.h"
# include "../system/err.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../bci.h"
# include <mdlint.h>

# define MAX 8
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

void static
op_rin(mdl_u8_t **__p) {
	printf("rin,\t\t");
	printf("adr{%x}\n", *(ffly_addr_t*)*__p);
	(*__p)+=sizeof(ffly_addr_t);
}

void(*out[])(mdl_u8_t**) = {
	op_exit,
	op_as,
	op_jmp,
	op_st,
	op_ld,
	op_out,
	op_mov,
	op_rin
};

# include "../ffef.h"
# include "../opt.h"
# include "../system/string.h"
# include "../ffly_def.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("please provide binfile.\n");
		reterr;
	}

	mdl_u64_t beg = 0, size = 0;
	if (__argc>2) {
		struct ffpcll pcl;
		ffoe_prep();
		pcl.cur = __argv+2;
		pcl.end = __argv+__argc;
		ffoe(ffoe_pcll, (void*)&pcl);
		beg = ffly_stno(ffoptval(ffoe_get("b")));
		size = ffly_stno(ffoptval(ffoe_get("s")));
		ffoe_end();
	}

	char const *bin = __argv[1];
	int fd;
	if ((fd = open(bin, O_RDONLY, 0)) < 0) {
		fprintf(stderr, "failed to open file\n");
		reterr;
	}

	struct stat st;
	stat(bin, &st);
	if (!size)
		size = st.st_size;

	mdl_u8_t *bed = (mdl_u8_t*)malloc(size);
	if (beg>0)
		lseek(fd, beg, SEEK_SET);
	read(fd, bed, size);
	close(fd);

	printf("beg, %u, size, %u\n", beg, size);
	mdl_u8_t *p = bed;
	mdl_u8_t *end = p+size;
	while(p != end) {
		mdl_u8_t op;	
		if ((op = *(p++)) > MAX-1) {
			printf("error malformed opno, got{%u}\n", op);
			break;
		}

		out[op](&p);
	}

	free(bed);
	retok;
}
