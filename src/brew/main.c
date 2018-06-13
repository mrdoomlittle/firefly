# include "brew.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/types.h"
# include "../linux/stat.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../types.h"
# include "../string.h"
ff_u8_t *p = NULL;
ff_u8_t static *eof;
ff_u8_t at_eof() {
	return p >= eof;
}

// to free list
void *tf[100];
void **fresh = tf;
void to_free(void *__p) {
	if (fresh-tf >= 100) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

struct hash env;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const *file;
	if (__argc < 2) {
		fprintf(stderr, "please provide file.\n");
		return -1;
	}

	file = __argv[1];
	ff_s32_t fd;
	ff_u8_t *bed;
	if ((fd = open(file, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "failed to open file.\n");
		return -1;
	}

	struct stat st;
	if (fstat(fd, &st) == -1) {
		fprintf(stderr, "failed to stat file.\n");
		goto _fault;	
	}

	if ((p = (ff_u8_t*)malloc(st.st_size)) == NULL) {
		fprintf(stderr, "failed to allocate memory.\n");
		goto _fault;
	}	

	bed = p;
	read(fd, p, st.st_size);
	
	eof = p+st.st_size;
	bucketp top = NULL;
	printf("filesize: %u\n", st.st_size);
	hash_init(&env);
	char const **arg = __argv+1;
	while(arg != __argv+__argc) {
		if (!strcmp(*arg, "-D")) {
			char const *s = *(++arg);
			char const *p = s;
			char buf[128];
			char *bufp;
			struct var *v = (struct var*)malloc(sizeof(struct var));
			to_free(v);

			bufp = buf;
			while(*p != '=')
				*(bufp++) = *(p++);
			hash_put(&env, buf, bufp-buf, v);
			p++;
			bufp = buf;
			while(*p != '\0')
				*(bufp++) = *(p++);
			*bufp = '\0';
			memdup(&v->p, buf, (bufp-buf)+1);
			v->l = bufp-buf;
		}
		arg++;
	}

	parse(&top);
	objp ep;
	gen(top, &ep);
	hash_destroy(&env);
	brew_exec(ep);

	void **cur = tf;
	while(cur != fresh)
		free(*(cur++));
	lexer_cleanup();

_fault:
	close(fd);
	if (bed != NULL)
		free(bed);
	return 0;
}
