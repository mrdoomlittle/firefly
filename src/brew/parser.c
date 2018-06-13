# include "brew.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../ffly_def.h"
# include "../types.h"
# include "../system/err.h"
# include "../string.h"
# define alloc_node (bucketp)malloc(sizeof(struct bucket))

struct frag* format_str(char const*);
ff_err_t parser_jump(bucketp *__node) {
	bucketp label = nexttok();
	if (label->sort != _ident) {
		fprintf(stderr, "wrong sort of token.\n");
		reterr;
	}

	bucketp p = (*__node = alloc_node);
	p->sort = _jump;
	p->p = label->p;
	retok;
}

ff_err_t parser_label(bucketp *__node) {
	bucketp name = nexttok();
	if (!expect_token(_keywd, _comma)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	printf("label: %s\n", (char*)name->p);	
	bucketp p = (*__node = alloc_node);
	p->sort = _label;
	p->p = name->p;

	bucketp tok;
	while(!is_keywd(tok = nexttok(), _comma)) {
		if (tok->sort != _ident) break;
		ulex(tok);
		bucketp bk = p;
		parser_jump(&p);
		bk->fd = p;
	}
	p->fd = NULL;
	retok;
}

ff_err_t parser_decl(bucketp *__node) {
	bucketp name = nexttok();	
	bucketp val;
	struct var *v = (struct var*)malloc(sizeof(struct var));
	to_free(v);
	hash_put(&env, name->p, strlen(name->p), v);
	if (!expect_token(_keywd, _eq)) {
	}
	val = nexttok();
	v->p = val->p;
	v->l = val->len;
	printf("var value: %s\n", val->p);
}

ff_err_t parser_cp(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_cp)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	bucketp dst, src;
	dst = nexttok();
	src = nexttok();
	printf("cp %s %s\n", (char*)dst->p, (char*)src->p);
	bucketp p = (*__node = alloc_node);
	p->sort = _cp;
	p->src = src->p;
	p->dst = dst->p;
	retok;
}

ff_err_t parser_end(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_end)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	bucketp p = (*__node = alloc_node);
	p->sort = _end;
	retok;
}

ff_err_t parser_exit(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_exit)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	bucketp p = (*__node = alloc_node);
	p->sort = _exit;
	retok;
}

ff_err_t parser_echo(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_echo)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	bucketp val = nexttok();

	bucketp p = (*__node = alloc_node);
	p->sort = _echo;
	p->p = format_str(val->p);
	retok;
}

struct frag*
format_str(char const *__s) {
	char *p = __s;
	char *bed = p;
	char buf[1024];
	char *bufp;
	struct frag *top = (struct frag*)malloc(sizeof(struct frag));
	struct frag *cur = top, *f;
	top->next = NULL;

_again:
	to_free(cur);
	bed = p;
	if (*p == '$') {
		p++;
		bufp = buf;
		while(*p != ' ' && *p != '\0')
			*(bufp++) = *(p++);
		struct var *v;

		v = (struct var*)hash_get(&env, buf, bufp-buf);
		if (!v) {
			printf("error.\n");
			return NULL;
		}

		printf("var value: %s\n", v->p);
		cur->len = v->l;
		cur->p = v->p;
	} else {
		while(*p != '$' && *p != '\0')
			p++;
		ff_uint_t l = p-bed;
		cur->p = malloc(l+1);
		to_free(cur->p);
		memcpy(cur->p, bed, l);
		cur->len = l;
		*((char*)cur->p+l) = '\0';
	}

	if (*p != '\0') {
		f = (struct frag*)malloc(sizeof(struct frag));
		cur->next = f;
		cur = f;
		f->next = NULL;
		goto _again;
	}

	return top;
}

ff_err_t parser_shell(bucketp *__node) {
//	if (!expect_token(_keywd, _circumflex)) {
//		reterr;
//	}

	struct shell *s = (struct shell*)malloc(sizeof(struct shell));
	to_free(s);
	bucketp b = (*__node = alloc_node);
	b->sort = _shell;
	char *p = (char*)nexttok()->p;
 	printf("shell.\n");
 	char **arg = s->args;
	char buf[1024];
	char *bufp = buf;
	while(*p != ' ' && *p != '\0')
		*(bufp++) = *(p++);
	if (*buf == '$') {
		struct var *v;
		v = (struct var*)hash_get(&env, buf+1, (bufp-buf)-1);
		s->base = v->p;
	} else {
		*bufp = '\0';
		memdup(&s->base, buf, (bufp-buf)+1);
		to_free(s->base);
	}
	if (*p == '\0')
		goto _sk;
_again:
	p++;
	bufp = buf;
	while(*p != ' ' && *p != '\0')
		*(bufp++) = *(p++);
	if (*buf == '$') {
		struct var *v;
		v = (struct var*)hash_get(&env, buf+1, (bufp-buf)-1);
		if (*(char*)v->p != '\0')
			*(arg++) = v->p;
	} else {
		*bufp = '\0';
		memdup(arg++, buf, (bufp-buf)+1);
		to_free(*(arg-1));
	}

	if (*p != '\0')
		goto _again;
_sk:
	*arg = NULL;
	b->p = s;
	retok;
}

// entry point designation
ff_i8_t static epdeg = -1;
void parse(bucketp *__p) {
	bucketp end = NULL, p;
	while(!at_eof() && !tokbuf_size()) {
		p = NULL;
		bucketp tok = nexttok();
		if (!tok) return;

		if (tok->sort == _keywd) {
			if (tok->val == _circumflex) {
				parser_shell(&p);
			} else if (tok->val == _period) {
				bucketp dir;
				if ((dir = nexttok())->sort != _keywd) {
					// err
					printf("expected keyword.\n");
				}

				switch(dir->val) {
					case _keywd_entry:
						if (!epdeg) {
							printf("entry point has already been designated.\n");
							nexttok(); // sk
							break;
						} else
							epdeg = 0;
						p = alloc_node;
						p->sort = _jump;
						p->p = nexttok()->p;
						p->fd = NULL;
						if (*__p != NULL)
							p->fd = *__p;
						*__p = p;
						printf("entry point: %s\n", p->p);
						end = p;
						p = NULL;
					break;
				}
			} else {
				ulex(tok);
				switch(tok->val) {
					case _keywd_echo:
						if (_err(parser_echo(&p))) return;
					break;
					case _keywd_cp:
						if (_err(parser_cp(&p))) return;
					break;
					case _keywd_exit:
						if (_err(parser_exit(&p))) return;
					break;
					case _keywd_end:
						if (_err(parser_end(&p))) return;
					break;
				}
			}
		} else if (tok->sort == _ident) {
			bucketp brief = peektok();
			ulex(tok);

			if (brief->sort == _keywd && brief->val == _eq) {
				parser_decl(&p);
			} else {
				if (brief->sort == _keywd && brief->val == _comma) {
					if (_err(parser_label(&p))) return;
					if (!*__p)
						*__p = p;
					if (end != NULL)
						end->fd = p;
					to_free(p);
					while(p->fd != NULL)
						to_free(p = p->fd);
					end = p;
					continue;
				} else {
					if (_err(parser_jump(&p))) return;
				}
			}
		} else {
			printf("unknown.\n");
		}

		if (p != NULL) {
			to_free(p);
			p->fd = NULL;
			if (!*__p)
				*__p = p;
			if (end != NULL)
				end->fd = p;
			end = p;
		}
	}
}
