# include "brew.h"
# include "../data/str_cmp.h"
# include "../ffly_def.h"
void static
to_keyword(bucketp __tok, mdl_u8_t __val) {
	__tok->sort = _keywd;
	__tok->val = __val;
}
// next token
bucketp nexttok() {
	bucketp tok;
	if (!(tok = lex())) return NULL;
	maybe_keyword(tok);
	return tok;
}

bucketp peektok() {
	bucketp p;
	ulex(p = nexttok());
	return p;
}

void
maybe_keyword(bucketp __tok) {
	if (!ffly_str_cmp((char*)__tok->p, "cp"))
		to_keyword(__tok, _keywd_cp);
	else if (!ffly_str_cmp((char*)__tok->p, "exit"))
		to_keyword(__tok, _keywd_exit);
}
