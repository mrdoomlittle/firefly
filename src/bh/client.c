# include "../bh.h"
# include "../stdio.h"
# include "../string.h"
# include "../brick.h"
# include "../system/util/ff5.h"
# include "../dep/mem_cmp.h"
# include "../dep/mem_cpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
//static struct ff_bh bh;

/*
	TODO:
		add "load" command, load contents of file into bricks 
*/
struct wedge {
	void *p, *pp;
	struct wedge *next, **bk;
};

struct wedge *mem_top = NULL;

struct arg {
	ff_u8_t *data;
	ff_uint_t len;
};

struct cmd {
	ff_u8_t *id;
	ff_uint_t idlen;
	struct arg *args[20];
	ff_uint_t argc;
};

void static* mem_alloc(ff_uint_t __n) {
	ff_u8_t *p;
	struct wedge *w;
	w = (struct wedge*)__ffly_mem_alloc(sizeof(struct wedge));
	p = (ff_u8_t*)__ffly_mem_alloc(__n+sizeof(void*));
	void **wp;
	wp = (void**)p;
	p+=sizeof(void*);
	*wp = w;

	if (mem_top != NULL)
		mem_top->bk = &w->next;
	w->next = mem_top;
	mem_top = w;
	w->bk = &mem_top;

	return  w->pp = (void*)p;
}


void static mem_free(void *__p) {
	struct wedge *w;

	ff_u8_t *p;
	p = ((ff_u8_t*)__p)-sizeof(struct wedge*);
	w = *(void**)p;


	*w->bk = w->next;
	if (w->next != NULL)
		w->next->bk = w->bk;

	__ffly_mem_free(w);
	__ffly_mem_free(p);
}

void static* _memdup(void *__p, ff_uint_t __n) {
	void *p;
	p = mem_alloc(__n);
	ffly_mem_cpy(p, __p, __n);
	return p;
}

struct cmd cur_command;

void static read_comd(char const *__line, ff_uint_t __n) {
	char buf[2048];

	char *le;
	le = __line+__n+1; // add one to use only need to use '<' and not '<='
	char *lp, *bfp;
	ff_uint_t ac;
	ac = 0;
	lp = __line;

	struct arg **ap, *a;

	ap = cur_command.args;
	bfp = buf;
	char c;
	c = *lp;
	while(c != ' ' && lp<le) {
		*(bfp++) = c;
		c = *(++lp);
	}

	ff_uint_t idlen;
	idlen = bfp-buf;
	cur_command.id = (ff_u8_t*)_memdup(buf, idlen);
	cur_command.idlen = idlen;
_again:
	if (lp<le) {
		bfp = buf;
		c = *(++lp);
		while(c != ' ' && lp<le) {
			*(bfp++) = c;
			c = *(++lp);
		}

		a = *(ap++) = mem_alloc(sizeof(struct arg));
		ff_uint_t len;
		len = bfp-buf;
		a->data = (ff_u8_t*)_memdup(buf, len);
		a->len = len;
		ac++;
		goto _again;
	}

	cur_command.argc = ac;
}

void static freeall(void) {
	struct wedge *cur, *bk;
	cur = mem_top;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		mem_free(bk->pp);
	}
}

ff_err_t ffmain(int __argc, char const *__argv[]) {

	ff_bh_open(&bh);
	ff_bh_connect(&bh, "192.168.0.111", 40960);	
	ff_err_t err;

/*	
	ff_uint_t const n = 8;
	ff_u32_t b[n];

	ff_uint_t i;

	i = 0;
	while(i != n) {
		b[i++] = ff_bh_bnew(&bh, _ff_brick_256, &err);
	}

	i = 0;
	while(i != n) {
		ff_bh_brid(&bh, b[i++]);
	}
*/
//	ff_bh_bnewm(&bh, _ff_brick_256, b, n);
//	char buf[1024];

//	*(buf+ffly_ff5_enc(b, buf, n*sizeof(ff_u32_t))) = '\0';
//	printf("brick key: %s\n", buf);
//	ff_bh_bridm(&bh, b, n);

	char line[8192];

	char buf[2048];
	ff_u32_t b;
	ff_uint_t n;
	ff_uint_t i;
	struct arg *a0, *a1;
_again:
	ffly_printf("~: ");
	ffly_fdrain(ffly_out);
	n = ffly_rdline(line, 200, ffly_in);
	read_comd(line, n-1);

	ffly_printf("line: %s : %u\n", line, n);
	ffly_mem_cpy(buf, cur_command.id, cur_command.idlen);
	*(buf+cur_command.idlen) = '\0';
	ffly_printf("command : %s : %u\n", buf, cur_command.idlen);
	i = 0;
	while(i != cur_command.argc) {
		a0 = *(cur_command.args+i);
		ffly_mem_cpy(buf, a0->data, a0->len);
		*(buf+a0->len) = '\0';
		ffly_printf("%u arg : %s\n", i, buf);
		i++;
	}

	switch(cur_command.idlen) {
		case 3:
			if (!ffly_mem_cmp(cur_command.id, "new", 3)) {
				a0 = *cur_command.args;
				ff_uint_t n;
				n = ffly_sntno(a0->data, a0->len);
				ff_u32_t *b;
				b = (ff_u32_t*)__ffly_mem_alloc(n*sizeof(ff_u32_t));
				ff_bh_bnewm(&bh, _ff_brick_256, b, n);
				char bkbuf[2048];
				*(bkbuf+ffly_ff5_enc(b, bkbuf, n*sizeof(ff_u32_t))) = '\0';
				__ffly_mem_free(b);
				ffly_printf("%s\n", bkbuf);
			} else if (!ffly_mem_cmp(cur_command.id, "rid", 3)) {
				a0 = *cur_command.args;
				a1 = *(cur_command.args+1);
				ff_uint_t n;
				n = ffly_sntno(a0->data, a0->len);
				ff_u32_t *b;
				b = (ff_u32_t*)__ffly_mem_alloc(n*sizeof(ff_u32_t));
				ffly_ff5_dec(a1->data, b, a1->len);
				ff_bh_bridm(&bh, b, n);
				__ffly_mem_free(b);
			}
		break;
		case 4:
			if (!ffly_mem_cmp(cur_command.id, "exit", 4)) {
				goto _esc;
			}
		break;
	}

	freeall();
	goto _again;
_esc:
	freeall();
/*
	if (*__argv[1] == '#') {
		ffly_ff5_dec(__argv[2], &b, strlen(__argv[2]));
		if (!ff_bh_bexist(&bh, b, &err)) {
			ff_bh_bopen(&bh, b);
			ff_bh_bread(&bh, b, buf0, 256, 0);
			ff_bh_bclose(&bh, b);
			printf("%s\n", buf0);
		} else
			printf("brick/s do not exist.\n");
	} else {
		strcpy(buf0, __argv[2]);
		b = ff_bh_bnew(&bh, _ff_brick_256, &err);
		char buf[1024];
		*(buf+ffly_ff5_enc(&b, buf, sizeof(ff_u32_t))) = '\0';
		printf("brick key: %s\n", buf);
		ff_bh_bopen(&bh, b);
		ff_bh_bwrite(&bh, b, buf0, 100, 0);
		ff_bh_bclose(&bh, b);
	}
*/
	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
}
