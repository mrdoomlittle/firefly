# include "brew.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../ffly_def.h"
# include "../types.h"
# include "../system/err.h"
ffly_err_t parser_label(bucketp *__node) {
	bucketp name = nexttok();
	if (!expect_token(_chr, _colon)) {
		fprintf(stderr, "expect error.\n");
		reterr;
	}

	printf("label: %s\n", (char*)name->p);	
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
	retok;
}

void parse(bucketp *__p) {
	bucketp end = NULL, p = NULL;
	while(!at_eof() && !tokbuf_size()) {
		bucketp tok = nexttok();
		if (!tok) return;
		if (tok->sort == _keywd) {
			ulex(tok);
			switch(tok->val) {
				case _keywd_cp:
					if (_err(parser_cp(&p))) return;
				break;
			}
		} else if (tok->sort == _ident) {
			bucketp brief = peektok();
			ulex(tok);

			if (brief->sort == _chr && brief->val == _colon) {
				if (_err(parser_label(&p))) return;
			}
		} else {
			printf("unknown.\n");
		}

		if (p != NULL) {
			if (!*__p)
				*__p = p;
			if (end != NULL)
				end->fd = p;
			end = p;
		}
	}
}
