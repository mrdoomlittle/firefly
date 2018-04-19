# define __ffly_compiler_internal
# define __ffly_parser
# include "../compiler.h"

ffly_mapp ffc_get_env(struct ffly_compiler*);
void ffc_build_node(struct ffly_compiler*, struct node**, struct node*);
void ffc_build_type(struct ffly_compiler*, struct type**, struct type*);

ff_bool_t static is_func_call(ff_compilerp);

struct type static *uint_t = &(struct type){.kind=_uint_t, .size=sizeof(ff_uint_t)};
struct type static *int_t = &(struct type){.kind=_int_t, .size=sizeof(ff_int_t)};

struct type static *u64_t = &(struct type){.kind=_u64_t, .size=8};
struct type static *i64_t = &(struct type){.kind=_i64_t, .size=8};

struct type static *u32_t = &(struct type){.kind=_u32_t, .size=4};
struct type static *i32_t = &(struct type){.kind=_i32_t, .size=4};

struct type static *u16_t = &(struct type){.kind=_u16_t, .size=2};
struct type static *i16_t = &(struct type){.kind=_i16_t, .size=2};

struct type static *u8_t = &(struct type){.kind=_u8_t, .size=1};
struct type static *i8_t = &(struct type){.kind=_i8_t, .size=1};

void static
ast_func(ff_compilerp __compiler, struct node **__node, char const *__name, ffly_vecp __block) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func, .block=*__block});
	(*__node)->p = (void*)__name;
}

void static
ast_func_call(ff_compilerp __compiler, struct node **__node, char const *__to) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func_call});
	(*__node)->p = (void*)__to;
}

void static
ast_as(ff_compilerp __compiler, struct node **__node, char const *__code) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_as});
	(*__node)->p = (void*)__code;
}

void static
ast_ret(ff_compilerp __compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_ret});
}

ff_err_t
parser_primary_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);

	switch(tok->kind) {
		case _tok_str:
			
		break;
		default:
			ffly_ulex(__compiler, tok);
	}
	retok;
}

ff_err_t
parser_expr(ff_compilerp __compiler, struct node **__node) {
	parser_primary_expr(__compiler, __node);
	retok;
}

ff_err_t
parser_decl_spec(ff_compilerp __compiler, struct token *__tok, struct type **__type) {
	if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
	
	} else if (__tok->id == _k_void) {
		*__type = NULL;
	}
	retok;
}

ff_err_t
parser_as_stmt(ff_compilerp __compiler, struct node **__node) {
	sktok(__compiler);

	ff_err_t err;
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	struct token *as;
	as = next_token(__compiler);
	ast_as(__compiler, __node, (char const*)as->p);
	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	retok;
}

ff_err_t
parser_ret_stmt(ff_compilerp __compiler, struct node **__node) {
	sktok(__compiler);
	ast_ret(__compiler, __node);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}
	retok;
}

ff_err_t parser_func_call(ff_compilerp, struct node**);

ff_err_t
parser_compound_stmt(ff_compilerp __compiler, struct ffly_vec *__vec) {
	if (!expect_token(__compiler, _tok_keywd, _l_brace)) {

	}

	struct token *tok;
	struct node *nod;
	struct node **p;
_again:
	tok = peek_token(__compiler);
	if (tok->id == _r_brace) {
		sktok(__compiler);
		goto _end;
	}

	if (tok->id == _k_as)
		parser_as_stmt(__compiler, &nod);
	else if (tok->id == _k_ret)
		parser_ret_stmt(__compiler, &nod);
	else if (is_func_call(__compiler))
		parser_func_call(__compiler, &nod);
	ffly_vec_push_back(__vec, (void**)&p);
	*p = nod;
	goto _again;
_end:
	retok;
}

ff_err_t
parser_func(ff_compilerp __compiler, struct node **__node) {
	struct type *ret_type;
	parser_decl_spec(__compiler, next_token(__compiler), &ret_type);

	struct token *name;
	name = next_token(__compiler);

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

	}

	struct ffly_vec block;
	if (!next_token_is(__compiler, _tok_keywd, _semicolon)) {
		ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
		ffly_vec_init(&block, sizeof(struct node*));
		parser_compound_stmt(__compiler, &block);

		vec_cleanup(__compiler, &block);

		struct node *nod;
		ff_err_t err;
		if ((nod = (struct node*)ffly_map_get(&__compiler->env, name->p, ffly_str_len((char*)name->p), &err)) != NULL) {
			nod->block = block;
			goto _end;
		}
	}

	ast_func(__compiler, __node, name->p, &block);
	ffly_map_put(&__compiler->env, name->p, ffly_str_len((char*)name->p), *__node);
_end:
	retok;
}

ff_err_t
parser_func_call(ff_compilerp __compiler, struct node **__node) {
	struct token *name;
	name = next_token(__compiler);

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
	}

	ff_err_t err;
	ffly_map_get(&__compiler->env, name->p, ffly_str_len((char*)name->p), &err);
	if (_err(err)) {
		errmsg("function not hear.\n");
		reterr;
	}

	ast_func_call(__compiler, __node, name->p);
	retok;
}

ff_err_t static
parser_stmt(ff_compilerp __compiler, struct node **__node) {
	struct token *tok;
	tok = peek_token(__compiler);

	if (tok->id == _k_as)
		parser_as_stmt(__compiler, __node);
}

ff_bool_t static
is_func(ff_compilerp __compiler) {
	struct token *type, *name, *l_paren;
	ff_u8_t res = 0;
	if(!(type = next_token(__compiler)))
		goto _r2;
	if (!(name = next_token(__compiler)))
		goto _r1;
	if (!(l_paren = next_token(__compiler)))
		goto _r0;
	res = (type->kind == _tok_keywd && name->kind == _tok_ident && l_paren->kind == _tok_keywd);
	ffly_ulex(__compiler, l_paren);
_r0:
	ffly_ulex(__compiler, name);
_r1:
	ffly_ulex(__compiler, type);
_r2:
	return res;
}

ff_bool_t
is_func_call(ff_compilerp __compiler) {
	struct token *name, *l_paren;
	ff_u8_t res = 0;

	if (!(name = next_token(__compiler)))
		goto _r1;	
	if (!(l_paren = next_token(__compiler)))
		goto _r0;
	res = (name->kind == _tok_ident && l_paren->kind == _tok_keywd);
	ffly_ulex(__compiler, l_paren);
_r0:
	ffly_ulex(__compiler, name);
_r1:
	return res;
}

ff_err_t
ffly_ff_parse(ff_compilerp __compiler) {
	ff_err_t err = FFLY_SUCCESS;
	while(!at_eof(__compiler)) {
		struct token *tok;

		struct node *nod = NULL;
		if (!(tok = peek_token(__compiler))) {
			errmsg("got dead token.\n");
			break;
		}

		if (is_func(__compiler)) {
			parser_func(__compiler, &nod);
		} else if (tok->kind == _tok_keywd) {
			parser_stmt(__compiler, &nod);
		} else if (tok->kind == _tok_ident) {
			parser_expr(__compiler, &nod);
		}

		if (nod != NULL) {
			struct node **p;
			ffly_vec_push_back(&__compiler->nodes, (void**)&p);
			*p = nod;
		}
	}

	if (_err(err))
		return err;
	return FFLY_SUCCESS;
}
