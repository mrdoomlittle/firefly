# include "brew.h"
# include "../dep/str_cmp.h"
# include "../ffly_def.h"
mdl_u8_t 
is_keywd(bucketp __tok, mdl_u8_t __val) {
	return (__tok->sort == _keywd && __tok->val == __val);
}

void static
to_keyword(bucketp __tok, mdl_u8_t __val) {
	__tok->sort = _keywd;
	__tok->val = __val;
}
// next token
bucketp nexttok() {
	bucketp tok;
	if (!(tok = lex())) return NULL;
	if (tok->sort == _ident)
		maybe_keyword(tok);
	return tok;
}

bucketp peektok() {
	bucketp p;
	ulex(p = nexttok());
	return p;
}
# include "../stdio.h"
void
maybe_keyword(bucketp __tok) {
	if (__tok->p == NULL) {
		printf("error: lexer.\n");
		return;
	}

	if (!ffly_str_cmp((char*)__tok->p, "cp"))
		to_keyword(__tok, _keywd_cp);
	else if (!ffly_str_cmp((char*)__tok->p, "exit"))
		to_keyword(__tok, _keywd_exit);
	else if (!ffly_str_cmp((char*)__tok->p, "end"))
		to_keyword(__tok, _keywd_end);
	else if (!ffly_str_cmp((char*)__tok->p, "entry"))
		to_keyword(__tok, _keywd_entry);
}
