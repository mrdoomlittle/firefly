# include "mul.h"
# include "../linux/stat.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../malloc.h"
# include "../string.h"
# include "../stdio.h"
void *tf[100];
void **fresh = tf;
void to_free(void *__p) {
	*(fresh++) = __p;
}

brickp get_brick(mulp __mul, char const *__id) {
	return (brickp)hash_get(&__mul->bricks, __id, strlen(__id));
}

ff_uint_t mul_bricklen(brickp __brc) {
	return (__brc->end-__brc->p);
}

void mul_brickw(void *__buf, ff_uint_t __off, ff_uint_t __n, brickp __brc) {
	memcpy(__brc->p+__off, __buf, __n);
}

void mul_brickr(void *__buf, ff_uint_t __off, ff_uint_t __n, brickp __brc) {
	memcpy(__buf, __brc->p+__off, __n);
}

ff_u8_t at_eof(mulp __mul) {
	return (__mul->cur>=__mul->end) || *__mul->cur == '\0';
}

void mul_prepare(mulp __mul) {
	hash_init(&__mul->bricks);
	__mul->top = NULL;
}

ff_u8_t expect(mulp __mul, ff_u8_t __sort, ff_u8_t __val) {
	bucketp tok;
	lex(__mul, &tok);
	return (tok->sort == __sort && tok->val == __val);
}

void build_brick(brickp *__brick, ff_u8_t *__beg, ff_u8_t *__end) {
	ff_uint_t l = __end-__beg;
	ff_u8_t *p = (ff_u8_t*)malloc(l);	
	ff_u8_t *cur = __beg;
	ff_uint_t off;

	while(cur != __end) {
		ff_uint_t left = __end-cur;
		off = cur-__beg;
		if (left>>3 > 0) {
			*((ff_u64_t*)(p+off)) = *(ff_u64_t*)cur;
			cur+=sizeof(ff_u64_t);
		} else
			*(p+off) = *(cur++); 
	}

	*(*__brick = (brickp)malloc(sizeof(struct brick))) =
		(struct brick){.p=p, .end=p+l, .next=NULL};
}

brickp static end = NULL;
void leak_brick(mulp __mul, char const *__id, ff_u8_t *__beg, ff_u8_t *__end) {
	brickp brc;
	build_brick(&brc, __beg, __end);
	if (!__mul->top)
		__mul->top = brc;
	if (end != NULL)
		end->next = brc;
	end = brc;

	hash_put(&__mul->bricks, __id, strlen(__id), brc);	
}

void mul_process(mulp __mul) {
	bucketp name;

	_again:
	lex(__mul, &name);
	if (name->sort != _ident) {
		fprintf(stderr, "fatal error, got: %u\n", name->sort);
		return;
	}

	printf("brick: %s\n", (char*)name->p);

	bucketp brc;
	lex(__mul, &brc);
	leak_brick(__mul, (char const*)name->p, brc->beg+1, brc->end-1);

	if (!at_eof(__mul)) {
		bucketp tok;
		lex(__mul, &tok);
		if (tok->sort == _chr && tok->val == _comma)
			goto _again;
	}
}

void mul_ld(mulp __mul, char const *__file) {
	int fd;
	if ((fd = open(__file, O_RDONLY, 0)) == -1) {
		// err
		return;
	}

	struct stat st;
	fstat(fd, &st);
	__mul->p = (ff_u8_t*)malloc(st.st_size);
	__mul->end = __mul->p+st.st_size;
	__mul->cur = __mul->p;
	read(fd, __mul->p, st.st_size);
	close(fd);
}

void mul_oust(mulp __mul, char const *__dst) {
	int fd;
	if ((fd = open(__dst, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))) {
		// err
	}

	brickp cur = __mul->top;
	while(cur != NULL) {
		write(fd, cur->p, cur->end-cur->p);		
		cur = cur->next;
	}

	close(fd);
}

void mul_cleanup(mulp __mul) {
	brickp cur = __mul->top;
	while(cur != NULL) {
		brickp bk = cur;
		cur = cur->next;
		free(bk->p);
		free(bk);
	}

	hash_free(&__mul->bricks);	
	
	free(__mul->p);

	void **p = tf;
	while(p != fresh)
		free(*(p++));
	lexer_cleanup();
}
