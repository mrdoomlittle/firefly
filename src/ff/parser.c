# define __ffly_compiler_internal
# define __ffly_parser
# include "../compiler.h"
# include "../memory/mem_alloc.h"
ffly_mapp ffc_get_env(struct ffly_compiler*);
void ffc_build_node(struct ffly_compiler*, struct node**, struct node*);
void ffc_build_type(struct ffly_compiler*, struct type**, struct type*);

ff_bool_t static is_func_call(ff_compilerp);
ff_err_t parser_expr(ff_compilerp, struct node**);

struct type static *u64_t = &(struct type){.kind=_u64_t, .size=8};
struct type static *i64_t = &(struct type){.kind=_i64_t, .size=8};

struct type static *u32_t = &(struct type){.kind=_u32_t, .size=4};
struct type static *i32_t = &(struct type){.kind=_i32_t, .size=4};

struct type static *u16_t = &(struct type){.kind=_u16_t, .size=2};
struct type static *i16_t = &(struct type){.kind=_i16_t, .size=2};

struct type static *u8_t = &(struct type){.kind=_u8_t, .size=1};
struct type static *i8_t = &(struct type){.kind=_i8_t, .size=1};

void static
ast_decl(struct ffly_compiler *__compiler, struct node **__node, struct node *__var) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_decl, .var=__var, ._type=NULL});
}

void static
ast_out(struct ffly_compiler *__compiler, struct node **__node, struct node *__var) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_out, .var=__var, ._type=NULL});
}

void static
ast_var(struct ffly_compiler *__compiler, struct node **__node, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_var, ._type=__type});
}

void static
ast_func(ff_compilerp __compiler, struct node **__node, char const *__name, ffly_vecp __block, ff_u8_t __flags) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func, ._block=__block, .flags=__flags});
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
ast_assign(ff_compilerp __compiler, struct node **__node, struct node *__l, struct node *__r, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_assign, .l=__l, .r=__r, ._type=__type});
}

void static
ast_ret(ff_compilerp __compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_ret});
}

void static
ast_int_type(ff_compilerp __compiler, struct node **__node, struct type *__type, char const *__s) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_literal, ._type=__type});
	(*__node)->p = __s;
}

ff_bool_t static
is_type(struct ffly_compiler *__compiler, struct token *__tok) {
	if (__tok->kind != _tok_keywd) return 0;
	return (__tok->id == _k_u8_t);
}

void
read_no(ff_compilerp __compiler, struct node **__node, char *__s) {
	ff_u64_t no;
	struct type *_type;
	no = ffly_stno(__s);
	if (no >= 0 && no <= (ff_u8_t)~0)
		_type = u8_t;
	ast_int_type(__compiler, __node, _type, __s);
}

ff_err_t
parser_primary_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);

	ff_err_t err;
	switch(tok->kind) {
		case _tok_ident:
			*__node = (struct node*)ffly_map_get(&__compiler->env, tok->p, ffly_str_len((char*)tok->p), &err);
		break;
		case _tok_no:
			read_no(__compiler, __node, tok->p);
		break;
		case _tok_str:
			
		break;
		default:
			ffly_ulex(&__compiler->lexer, tok);
	}
	retok;
}

ff_err_t
parser_assign_expr(ff_compilerp __compiler, struct node **__node) {
	if (next_token_is(__compiler, _tok_keywd, _eq)) {
		struct node *l = *__node;
		struct node *r = NULL;
		parser_expr(__compiler, &r);
		ast_assign(__compiler, __node, l, r, l->_type);
	}
	retok;
}

ff_err_t
parser_expr(ff_compilerp __compiler, struct node **__node) {
	parser_primary_expr(__compiler, __node);
	parser_assign_expr(__compiler, __node);
	retok;
}

void static
mk_notype(struct ffly_compiler *__compiler, struct type **__type, ff_u8_t __id) {
	struct type *tmpl;
	switch(__id) {
		case _k_u8_t:
			tmpl = u8_t;
		break;
	}
	ffc_build_type(__compiler, __type, tmpl);
}

ff_err_t
parser_decl_spec(ff_compilerp __compiler, struct token *__tok, struct type **__type) {
	if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
		mk_notype(__compiler, __type, __tok->id);	
	} else if (__tok->id == _k_void) {
		*__type = NULL;
	}
	retok;
}

ff_err_t
parser_decl(ff_compilerp __compiler, struct node **__node) {
	struct type *_type;
	parser_decl_spec(__compiler, next_token(__compiler), &_type);

	struct token *name = next_token(__compiler);	

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	struct node *var = NULL;
	ast_var(__compiler, &var, _type);
	ffly_map_put(ffc_get_env(__compiler), name->p, ffly_str_len((char*)name->p), var);

	ast_decl(__compiler, __node, var);
}

ff_err_t 
parser_out_stmt(ff_compilerp __compiler, struct node **__node) {
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {}
	struct token *name;
	name = next_token(__compiler);

	struct node *var;

	ff_err_t err;
	var = (struct node*)ffly_map_get(ffc_get_env(__compiler), name->p, ffly_str_len((char*)name->p), &err);

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	ast_out(__compiler, __node, var);
}

ff_err_t
parser_as_stmt(ff_compilerp __compiler, struct node **__node) {
	sktok(__compiler);

	ff_err_t err;
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	char *p;
	ff_uint_t l = 0;
	struct token *buf[24];
	struct token **cur = buf;
	struct token *tok;
_again:
	*(cur++) = (tok = next_token(__compiler));
	l+=tok->l;
	if (!next_token_is(__compiler, _tok_keywd, _r_paren))
		goto _again;
	*cur = NULL;

	p = (char*)__ffly_mem_alloc(l+1);
	cur = buf;
	char *end = p;
	ff_lexerp lexer = &__compiler->lexer;
	while((tok = *cur) != NULL) {
		end+=ffly_str_cpy(end, tok->p);
		ff_lexer_free(lexer, tok->p);
		ff_lexer_free(lexer, tok);
		cur++;
	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	ast_as(__compiler, __node, (char const*)p);

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

	if (tok->id == _k_out) 
		parser_out_stmt(__compiler, &nod);
	else if (tok->id == _k_as)
		parser_as_stmt(__compiler, &nod);
	else if (tok->id == _k_ret)
		parser_ret_stmt(__compiler, &nod);
	else if (is_func_call(__compiler))
		parser_func_call(__compiler, &nod);
	else {
		parser_expr(__compiler, &nod);
		if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

		}
	}
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

	ff_u8_t flags = _func_gbl|_func_def;
	ffly_vecp block = NULL;
	if (!next_token_is(__compiler, _tok_keywd, _semicolon)) {
		block = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));	
		ffly_vec_set_flags(block, VEC_AUTO_RESIZE);
		ffly_vec_init(block, sizeof(struct node*));
		parser_compound_stmt(__compiler, block);

		struct node *nod;
		ff_err_t err;
		if ((nod = (struct node*)ffly_map_get(&__compiler->env, name->p, ffly_str_len((char*)name->p), &err)) != NULL) {
			nod->_block = block;
			nod->flags = flags;
			goto _end;
		}
	} else
		flags = _func_exr;

	ast_func(__compiler, __node, name->p, block, flags);
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
	res = (type->kind == _tok_keywd && name->kind == _tok_ident && (l_paren->kind == _tok_keywd && l_paren->id == _l_paren));
	ffly_ulex(&__compiler->lexer, l_paren);
_r0:
	ffly_ulex(&__compiler->lexer, name);
_r1:
	ffly_ulex(&__compiler->lexer, type);
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
	res = (name->kind == _tok_ident && (l_paren->kind == _tok_keywd && l_paren->id == _l_paren));
	ffly_ulex(&__compiler->lexer, l_paren);
_r0:
	ffly_ulex(&__compiler->lexer, name);
_r1:
	return res;
}

ff_err_t
ffly_ff_parse(ff_compilerp __compiler) {
	ff_err_t err = FFLY_SUCCESS;
	while(!at_eof(&__compiler->lexer)) {
		struct token *tok;

		struct node *nod = NULL;
		if (!(tok = peek_token(__compiler))) {
			errmsg("got dead token.\n");
			break;
		}

		if (is_func(__compiler)) {
			parser_func(__compiler, &nod);
		} else if (is_type(__compiler, tok)) {
			parser_decl(__compiler, &nod);
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
