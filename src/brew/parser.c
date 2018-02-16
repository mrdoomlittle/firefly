# include "brew.h"
# include "../malloc.h"
# include "../stdio.h"
void parser_label(bucketp *__p) {
	bucketp name = lex();
	if (!expect_token(_chr, _colon)) {
		fprintf(stderr, "expect error.\n");
		return;
	}

	printf("label: %s\n", (char*)name->p);	

}

void parse(bucketp *__p) {
	while(!at_eof() && !tokbuf_size()) {
		bucketp tok = lex();
		if (tok->sort == _ident) {
			bucketp brief = peek_token();
			ulex(tok);

			if (brief->sort == _chr && brief->val == _colon) {
				parser_label(__p);
			}
		}
	}
}
