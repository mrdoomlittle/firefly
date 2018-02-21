# include "brew.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../ffly_def.h"
# include "../types.h"
# include "../system/err.h"
# define alloc_node (bucketp)malloc(sizeof(struct bucket))

ffly_err_t parser_jump(bucketp *__node) {
	bucketp label = nexttok();
	if (label->sort != _ident) {
		fprintf(stderr, "wrong sort of token.\n");
		reterr;
	}

	bucketp p = (*__node = alloc_node);
	p->p = label->p;
	retok;
}

ffly_err_t parser_label(bucketp *__node) {
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
		bucketp bk = *__node;
		parser_jump(__node);
		(*__node)->fd = bk;
	}
	retok;
}

ffly_err_t parser_cp(bucketp *__node) {
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

ffly_err_t parser_end(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_exit)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	printf("end.\n");
	bucketp p = (*__node = alloc_node);
	p->sort = _end;
	retok;
}

ffly_err_t parser_exit(bucketp *__node) {
	if (!expect_token(_keywd, _keywd_exit)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	printf("exit.\n");
	bucketp p = (*__node = alloc_node);
	p->sort = _exit;
	retok;
}

void parse(bucketp *__p) {
	bucketp end = NULL, p = NULL;
	while(!at_eof() && !tokbuf_size()) {
		bucketp tok = nexttok();
		if (!tok) return;
		if (tok->sort == _keywd) {
			ulex(tok);
			if (tok->val == _circumflex) {
				p = alloc_node;
				p->sort = _shell;

			} else {
				switch(tok->val) {
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

			if (brief->sort == _keywd && brief->val == _comma) {
				if (_err(parser_label(&p))) return;
			} else {
				if (_err(parser_jump(&p))) return;
			}
		} else {
			printf("unknown.\n");
		}

		if (p != NULL) {
			to_free(p);
			if (!*__p)
				*__p = p;
			if (end != NULL)
				end->fd = p;
			end = p;
		}
	}
	end->fd = NULL;
}
