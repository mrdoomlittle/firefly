# include "mat.h"
# include "system/string.h"
# include "string.h"
# include "malloc.h"
# include "stdio.h"
void static *tf[100];
void static **fresh = tf;
void static
to_free(void *__p) {
	if (fresh-tf >= 100) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

char static fetchc(ffly_matp __mat){return *__mat->p;}

ff_u8_t static 
is_space(ffly_matp __mat) {
	char c = *__mat->p;
	return (c == ' ' || c == '\t'); 
}

ff_u8_t static
at_eof(ffly_matp __mat) {
	return __mat->p>=__mat->end;
}

enum {
	_colour,
	_pos,
	_po
};

# define _padl 1
# define _padt 2
# define _r 4
# define _g 8
# define _b 16

# define is_bit(__attr, __bit) \
	(((__attr)->bits&__bit)==__bit)

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

struct frag {
	char *p;
	ff_uint_t l;
	ff_uint_t off;
};

struct line {
	struct frag *frags[20];
	struct frag **p;
	ff_uint_t off;
};

typedef struct parameter {
	char const *name;
	char const *val;
} *parameterp;

typedef struct attr {
	ff_u8_t type;
	ff_u64_t bits;

	ff_uint_t pad_left;
	ff_uint_t pad_top;
	ff_u8_t r, g, b;
} *attrp;

typedef struct bucket {
	ff_u8_t sort;
	ff_u8_t val;
	void *p;
} *bucketp;

char const static*
read_ident(ffly_matp __mat, ff_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = fetchc(__mat);
	while(c>='a'&&c<='z') {
		*(bufp++) = *(__mat->p++);
		c = fetchc(__mat);
	}
	*bufp = '\0';
	*__l = bufp-buf;
	char const *ret = strdup(buf);
	to_free(ret);
	return ret;
}

char const static*
read_no(ffly_matp __mat, ff_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = fetchc(__mat);
	while(c>='0'&&c<='9') {
		*(bufp++) = *(__mat->p++);
		c = fetchc(__mat);
	}
	*bufp = '\0';
	*__l = bufp-buf;
	char const *ret = strdup(buf);
	to_free(ret);
	return ret;
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
			ff_uint_t l;
			ret->p = (void*)read_ident(__mat, &l);
		} else if (c>='0'&&c<='9') {
			ret->sort = _no;
			ff_uint_t l;
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

ff_u8_t static
expect(ffly_matp __mat, ff_u8_t __sort, ff_u8_t __val) {
	bucketp tok = lex(__mat);
	if (!tok) return 0;
	return (tok->sort == __sort && tok->val == __val);
}

ff_i8_t static
next_tokis(ffly_matp __mat, ff_u8_t __sort, ff_u8_t __val) {
	bucketp tok = lex(__mat);
	if (!tok) return -1;

	ff_u8_t res = (tok->sort == __sort && tok->val == __val);
	if (res)
		return 0;

	ulex(tok);

	return -1;
}

void static act(ffly_matp, attrp);
void static 
label(ffly_matp __mat, ff_i8_t *__exit) {
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
		printf("no params, %s\n", name);
	if (!expect(__mat, _keychr, _gt)) {
		printf("expect error.\n");
	}

	struct attr p;
	p.bits = 0;
	if (!strcmp(name, "colour")) {
		p.type = _colour;
	} else if (!strcmp(name, "pos")) {
		p.type = _pos;
	} else if (!strcmp(name, "po")) {
		p.type = _po;
	} else
		printf("error.\n");

	parameterp cur;
	while((cur = *(--param)) != NULL) {
		if (*(cur->name+1) == '\0') {
			ff_u8_t *val;
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

# define new_frag \
{						\
	struct line *ln;	\
	ln = *(frame+cur);  \
	f = (*(ln->p++) = (struct frag*)malloc(sizeof(struct frag)));	\
}

# define line_save \
{                       \
	struct line *ln;    \
	ln = *(frame+cur);  \
	new_frag            \
	if ((f->l = (p-buf)) > 0) {				\
		memdup((void*)&f->p, buf, f->l); 	\
		f->off = ln->off+dis;				\
		ln->off+=f->l;						\
	}										\
}

# define HEIGHT 60
static struct line *frame[HEIGHT];
static char buf[1024];
static char *p = buf;
static char *base;
static struct frag *f;
static ff_uint_t cur = 0;
# define clr memset(p, '.', 1024-(p-buf));
static ff_uint_t dis;
static ff_i8_t pomode = -1;
void
act(ffly_matp __mat, attrp __attr) {
	ff_u8_t rc = 0;
	if (__attr != NULL) {
		if (__attr->type == _po) {
			base = p;
			pomode = 0;
		}
		if (is_bit(__attr, _padt)) {
			cur = __attr->pad_top;
		}

		if (is_bit(__attr, _padl)) {
			p+=__attr->pad_left;
		}

		if ((__attr->bits&(_r|_g|_b))>0) {
			rc = 1;

			p+=ffly_str_cpy(p, "\e[38;2;");
			p+=ffly_nots(__attr->r, p);
			*(p++) = ';';
			p+=ffly_nots(__attr->g, p);
			*(p++) = ';';
			p+=ffly_nots(__attr->b, p);
			*(p++) = 'm';
		}
	}

	bucketp tok;
	while(!at_eof(__mat)) {
		ff_i8_t exit;
		if (*__mat->p != '<')
			goto _no;
		if (!(tok = lex(__mat)))
			break;
		label(__mat, &exit);
		if (!exit) {
			break;
		}
		_no:
		{
			char c;
			while((c = *__mat->p) != '<' && !at_eof(__mat)) {
				if (c == '\t' || c == '\n')
					__mat->p++;
				else if (c == ';') {
					__mat->p++;
					line_save;
					cur++;
					clr
					p = buf;
					if (__attr != NULL) {
						if (is_bit(__attr, _padl)) {
							p+=__attr->pad_left;
						}
					}
				} else {
					*(p++) = c;
					__mat->p++;
				}
			}
		}
	}

	if (rc) {
		memcpy(p, "\e[0m", 4);
		p+=4;
	}

	if (!pomode) {
		ffly_printf("--> %u\n", p-base);
		(*(frame+cur))->off-=(dis = p-base);
		pomode = -1;
	}
	line_save;
	p = buf;
	clr
	dis = 0;
}

void static
cleanup() {
	struct line **p = frame;
	struct line **e = p+HEIGHT;
	struct line *ln;
	while(p != e) { 
		ln = *(p++);
		struct frag **cur = ln->frags;
		while(*cur != NULL) {
			struct frag *f = *cur;
			if (f->l>0)
				free(f->p);
			free(f);
			cur++;
		}
		free(ln);
	}

	if (bk != NULL)
		free(bk);

	{
		void **p = tf;
		while(p != fresh)
			free(*(p++));
	}
}

void static 
show(void) {
	struct line **p = frame;
	struct line **e = p+HEIGHT;
	ff_uint_t off;
	struct line *ln;
	while(p != e) {
		ln = *(p++);
		off = 0;
		if (*ln->frags != NULL) {
			struct frag **cur = ln->frags;
			while(*cur != NULL) {
				struct frag *f = *cur;
				if (f->l>0) {
					ff_uint_t bias;
					bias = off-f->off;
					ff_uint_t l = f->l-bias;
					char *p = f->p+bias;
					if (l>1024) {
						ffly_printf("error.\n");
					} else
						ffly_fwrite(ffly_out, p, l);
					off+=f->l;
				}
				cur++;
			}
		}
		ffly_printf("\n");
	}
}

void ffly_matact(ffly_matp __mat) {
	clr
	struct line **p = frame;
	struct line **e = p+HEIGHT;
	struct line *ln;
	while(p != e) {
		ln = (*(p++) = (struct line*)malloc(sizeof(struct line)));
		memset(ln->frags, 0, sizeof(ln->frags));
		ln->p = ln->frags;
		ln->off = 0;
	}
	act(__mat, NULL);

	show();
	cleanup();
}



