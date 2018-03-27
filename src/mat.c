# include "mat.h"
# include "system/string.h"
# include "string.h"
# include "malloc.h"
# include "stdio.h"
char static fetchc(ffly_matp __mat){return *__mat->p;}

mdl_u8_t static 
is_space(ffly_matp __mat) {
	char c = *__mat->p;
	return (c == ' ' || c == '\t'); 
}

mdl_u8_t static
at_eof(ffly_matp __mat) {
	return __mat->p>=__mat->end;
}

enum {
	_colour,
	_pos
};

# define _padl 1
# define _padt 2
# define _r 4
# define _g 8
# define _b 16

# define is_bit(__pill, __bit) \
	(((__pill)->bits&__bit)==__bit)

enum {
	_keychr,
	_ident,
	_no
};

enum {
	_gt,
	_lt,
	_colon,
	_slash,
	_comma
};

typedef struct parameter {
	char const *name;
	char const *val;
} *parameterp;

typedef struct pill {
	mdl_u8_t type;
	mdl_u64_t bits;

	mdl_uint_t pad_left;
	mdl_uint_t pad_top;
	mdl_u8_t r, g, b;
} *pillp;

typedef struct bucket {
	mdl_u8_t sort;
	mdl_u8_t val;
	void *p;
} *bucketp;

char const static*
read_ident(ffly_matp __mat, mdl_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = fetchc(__mat);
	while(c>='a'&&c<='z') {
		*(bufp++) = *(__mat->p++);
		c = fetchc(__mat);
	}
	*bufp = '\0';
	*__l = bufp-buf;
	return (char const*)strdup(buf);
}

char const static*
read_no(ffly_matp __mat, mdl_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = fetchc(__mat);
	while(c>='0'&&c<='9') {
		*(bufp++) = *(__mat->p++);
		c = fetchc(__mat);
	}
	*bufp = '\0';
	*__l = bufp-buf;
	return (char const*)strdup(buf);
}

bucketp static tokbuf[8];
bucketp static *next = tokbuf;

bucketp static bk = NULL;
bucketp static
lex(ffly_matp __mat) {
	if (next > tokbuf)
		return *(--next);
	if (at_eof(__mat)) return NULL;
	bucketp ret = (bucketp)malloc(sizeof(struct bucket));

	while(is_space(__mat)) __mat->p++;

	char c = *__mat->p;
	switch(c) {
		case '<':
			ret->sort = _keychr;
			ret->val = _lt;
			__mat->p++;
		break;
		case '>':
			ret->sort = _keychr;
			ret->val = _gt;
			__mat->p++;
		break;
		case ':':
			ret->sort = _keychr;
			ret->val = _colon;
			__mat->p++;
		break;
		case '/':
			ret->sort = _keychr;
			ret->val = _slash;
			__mat->p++;
		break;
		case ',':
			ret->sort = _keychr;
			ret->val = _comma;
			__mat->p++;
		break;
		default:
		if (c>='a'&&c<='z') {
			ret->sort = _ident;
			mdl_uint_t l;
			ret->p = (void*)read_ident(__mat, &l);
		} else if (c>='0'&&c<='9') {
			ret->sort = _no;
			mdl_uint_t l;
			ret->p = (void*)read_no(__mat, &l);
		}
	}

	if (bk != NULL)
		free(bk);
	bk = ret;
	return ret;
}

void static
ulex(bucketp __tok) {
	*(next++) = __tok;
}

mdl_u8_t static
expect(ffly_matp __mat, mdl_u8_t __sort, mdl_u8_t __val) {
	bucketp tok = lex(__mat);
	if (!tok) return 0;
	return (tok->sort == __sort && tok->val == __val);
}

mdl_i8_t static
next_tokis(ffly_matp __mat, mdl_u8_t __sort, mdl_u8_t __val) {
	bucketp tok = lex(__mat);
	if (!tok) return -1;

	mdl_u8_t res = (tok->sort == __sort && tok->val == __val);
	if (res)
		return 0;

	ulex(tok);

	return -1;
}

void static act(ffly_matp, pillp);
void static 
label(ffly_matp __mat, mdl_i8_t *__exit) {
	if (!next_tokis(__mat, _keychr, _slash)) {
		*__exit = 0;
		expect(__mat, _keychr, _gt);
		return;
	} else
		*__exit = -1;

	char const *name = lex(__mat)->p;

	parameterp *param = (parameterp*)malloc(20*sizeof(parameterp));
	*(param++) = NULL;
	if (!next_tokis(__mat, _keychr, _colon)) {
		char const *name, *val;
		_again:
		name = (char const*)lex(__mat)->p;
		if (!expect(__mat, _keychr, _colon)) {
			printf("expect error.\n");
		}
		val = (char const*)lex(__mat)->p;
	
		printf("param: %s, %s\n", name, val);

		parameterp p = (parameterp)malloc(sizeof(struct parameter));

		p->name = name;
		p->val = val;
		*(param++) = p;
		if (!next_tokis(__mat, _keychr, _comma)) {
			goto _again;
		}
	} else
		printf("no params.\n");
	if (!expect(__mat, _keychr, _gt)) {
		printf("expect error.\n");
	}

	struct pill p;
	p.bits = 0;
	if (!strcmp(name, "colour")) {
		p.type = _colour;
	} else if (!strcmp(name, "pos")) {
		p.type = _pos;
	} else
		printf("error.\n");

	parameterp cur;
	while((cur = *(--param)) != NULL) {
		if (*(cur->name+1) == '\0') {
			mdl_u8_t *val;
			p.r = 0;
			p.g = 0;
			p.b = 0;
			switch(*cur->name) {
				case 'r':
					p.bits |= _r;
					val = &p.r;
				break;
				case 'g':
					p.bits |= _g;
					val = &p.g;
				break;
				case 'b':
					p.bits |= _b;
					val = &p.b;
				break;
			}
			*val = ffly_stno(cur->val);
		} else if (!strcmp(cur->name, "padl")) {
			p.bits |= _padl;
			p.pad_left = ffly_stno(cur->val);
		} else if (!strcmp(cur->name, "padt")) {
			p.bits |= _padt;
			p.pad_top = ffly_stno(cur->val);
		}
		free(*param);
	}

	free(param);
	act(__mat, &p);
}

# define WD_SH 7

# define WIDTH (1<<WD_SH)
# define HEIGHT 40

char static frame[WIDTH*HEIGHT];
char static buf[WIDTH*HEIGHT];
char static **bkbuf;
void
act(ffly_matp __mat, pillp __pill) {
	if (!*(bkbuf-1))
		*bkbuf = frame;
	else
		*bkbuf = *(bkbuf-1);
	char *p = *(bkbuf++);

	mdl_u8_t rc = 0;
	if (__pill != NULL) {
		if (is_bit(__pill, _padl))
			p+=__pill->pad_left;
		if (is_bit(__pill, _padt))
			p+=__pill->pad_top*WIDTH;
	
		if (is_bit(__pill, _r) || is_bit(__pill, _g) || is_bit(__pill, _b)) {
			char r[24];
			char g[24];
			char b[24];
			rc = 1;


			ffly_nots(__pill->r, r);
			ffly_nots(__pill->g, g);
			ffly_nots(__pill->b, b);
			p+=ffly_sprintf(p, "\e[38;2;%s;%s;%sm", r, g, b);
			*p = '.';
		}
	}
	
	bucketp tok;
	while(!at_eof(__mat)) {
		while(*__mat->p == '\n')
			__mat->p++;
		if (*__mat->p != '<')
			goto _no;
		if (!(tok = lex(__mat)))
			break;
		mdl_i8_t exit;
		if (tok->sort == _keychr && tok->val == _lt) {
			*(bkbuf-1) = p;
			if (!next_tokis(__mat, _keychr, _gt))
				act(__mat, NULL);
			else
				label(__mat, &exit);
			p = *(bkbuf-1);
			if (!exit) break;
		}
		_no:
		{
			char c;
			while((c = *__mat->p) != '<' && !at_eof(__mat)) {
				if (*p == '\n') p++;
				if (c == '\n' || c == '\t')
					__mat->p++;
				else
					*(p++) = *(__mat->p++);
			}
		}
	}

	if (rc) {
		memcpy(p, "\e[0m", 4);
		p+=4;
	}

	if (!*(bkbuf-2))
		bkbuf--;
	else {
		if (*(bkbuf-3) != NULL)
			*((--bkbuf)-1) = p;
		else
			bkbuf--;
	}
}

void ffly_matact(ffly_matp __mat) {
	bkbuf = (char**)malloc(12*sizeof(char*));
	*(bkbuf++) = NULL;
	char *p = frame;
	while(p != frame+(WIDTH*HEIGHT)) {
		memset(p, '.', WIDTH);
		*p = '@';
		*((p+WIDTH)-2) = '@';
		*((p+=WIDTH)-1) = '\n';
	}

	act(__mat, NULL);
	*p = '\0';
	char buf[2048];
	char *bufp;
	p = frame;
	_again:
	bufp = buf;
	while(*p != '\n') {
		if (p >= frame+(WIDTH*HEIGHT))
			goto _end;

		*(bufp++) = *(p++);
	}
	p++;
	*bufp = '\0';
	printf("%s\n", buf);
	goto _again;
	_end:
	free(bkbuf-1);
}



