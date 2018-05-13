# define __ffly_compiler_internal
# define __ffly_parser
# include "../compiler.h"
# include "../memory/mem_alloc.h"
# include "../dep/str_cpy.h"
# include "../dep/str_len.h"
ffly_mapp ffc_get_env(struct ffly_compiler*);
void ffc_build_node(struct ffly_compiler*, struct node**, struct node*);
void ffc_build_type(struct ffly_compiler*, struct type**, struct type*);
struct type* ffc_get_typedef(struct ffly_compiler*, char*, ff_err_t*);

ff_bool_t static is_func_call(ff_compilerp);
ff_err_t parser_expr(ff_compilerp, struct node**);
ff_err_t parser_compound_stmt(ff_compilerp, struct ffly_vec*);

struct type static *u64_t = &(struct type){.kind=_u64_t, .size=8};
struct type static *i64_t = &(struct type){.kind=_i64_t, .size=8};

struct type static *u32_t = &(struct type){.kind=_u32_t, .size=4};
struct type static *i32_t = &(struct type){.kind=_i32_t, .size=4};

struct type static *u16_t = &(struct type){.kind=_u16_t, .size=2};
struct type static *i16_t = &(struct type){.kind=_i16_t, .size=2};

struct type static *u8_t = &(struct type){.kind=_u8_t, .size=1};
struct type static *i8_t = &(struct type){.kind=_i8_t, .size=1};

struct type static *ptr = &(struct type){.kind=_u64_t, .size=8};

void static
ast_decl(struct ffly_compiler *__compiler, struct node **__node, struct node *__var, struct node *__init) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_decl, .var=__var, .init=__init, ._type=NULL});
}

void static
ast_out(struct ffly_compiler *__compiler, struct node **__node, struct node *__var) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_out, .var=__var, ._type=NULL});
}

void static
ast_uop(struct ffly_compiler *__compiler, struct node **__node, ff_u8_t __kind, struct node *__operand, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=__kind, .operand=__operand, ._type=__type});
}

void static
ast_var(struct ffly_compiler *__compiler, struct node **__node, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_var, ._type=__type});
}

void static
ast_func(ff_compilerp __compiler, struct node **__node, char const *__name, ffly_vecp __block, ff_u8_t __flags, ffly_vecp __var_pond, ffly_vecp __args, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func, .block=*__block, .flags=__flags, .var_pond=*__var_pond, .args=*__args, ._type=__type});
	(*__node)->p = (void*)__name;
}

void static
ast_func_call(ff_compilerp __compiler, struct node **__node, char const *__to, ffly_vecp __params, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func_call, .params=*__params, ._type=__type});
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
ast_ret(ff_compilerp __compiler, struct node **__node, struct node *__retval, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_ret, .rtv=__retval, ._type=__type});
}

void static
ast_exit(ff_compilerp __compiler, struct node **__node, struct node *__code) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_exit, .code=__code});
}

void static
ast_int_type(ff_compilerp __compiler, struct node **__node, struct type *__type, char const *__s) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_literal, ._type=__type});
	(*__node)->p = __s;
}

void static 
ast_conv(ffly_compilerp __compiler, struct node **__node, struct type *__to, struct node *__operand) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_conv, ._type=__to, .operand=__operand});
}

void static
ast_label(ffly_compilerp __compiler, struct node **__node, char const *__name) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_label});
	(*__node)->p = __name;
}

void static
ast_jmpto(ffly_compilerp __compiler, struct node **__node, char const *__label) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_jmpto});
	(*__node)->p = __label;
}

void static
ast_if(ffly_compilerp __compiler, struct node **__node, struct node *__cond, ffly_vecp __block) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_if, .cond=__cond, .block=*__block});
}

void static
ast_binop(ffly_compilerp __compiler, struct node **__node, ff_u8_t __op, struct type *__type, struct node *__l, struct node *__r) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=__op, ._type=__type, .l=__l, .r=__r});
}

void static
mk_ptr_type(struct ffly_compiler *__compiler, struct type **__type, struct type *__ptr) {
	ffc_build_type(__compiler, __type, ptr);
	(*__type)->ptr = __ptr;
}

ff_err_t static
conv(ffly_compilerp __compiler, struct node **__node, struct node *__operand, struct type *__to) {
	ast_conv(__compiler, __node, __to, __operand);
	retok;
}

ff_bool_t static
is_type(struct ffly_compiler *__compiler, struct token *__tok) {
	if (__tok->kind == _tok_ident) {
		ff_err_t err;
		ffc_get_typedef(__compiler, (char*)__tok->p, &err);
		if (_ok(err))
			return 1;
	}
	if (__tok->kind != _tok_keywd) return 0;
	return (__tok->id == _k_u8_t || __tok->id == _k_u16_t || __tok->id == _k_u32_t || __tok->id == _k_u64_t);
}

void
read_no(ff_compilerp __compiler, struct node **__node, char *__s) {
	ff_u64_t no;
	struct type *_type;
	no = ffly_stno(__s);
	if (no >= 0 && no <= (ff_u8_t)~0)
		_type = u8_t;
	else if (no > (ff_u8_t)~0 && no <= (ff_u16_t)~0)
		_type = u16_t;
	else if (no > (ff_u16_t)~0 && no <= (ff_u32_t)~0)
		_type = u32_t;
	else if (no > (ff_u32_t)~0 && no <= (ff_u64_t)~0)
		_type = u64_t;
	ast_int_type(__compiler, __node, _type, __s);
}

ff_err_t
parser_primary_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);

	ff_err_t err;
	switch(tok->kind) {
		case _tok_ident:
			*__node = (struct node*)ffly_map_get(&__compiler->env, tok->p, ffly_str_len((char*)tok->p), &err);
			if (_err(err) && __compiler->local != NULL)
				*__node = (struct node*)ffly_map_get(__compiler->local, tok->p, ffly_str_len((char*)tok->p), &err);
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
parser_additive_expr(ff_compilerp __compiler, struct node **__node) {
	ff_u8_t op;
	if (next_token_is(__compiler, _tok_keywd, _plus))
		op = _op_add;
	else if (next_token_is(__compiler, _tok_keywd, _minus))
		op = _op_sub;
	else
		goto _end;
	struct node *l = *__node, *r = NULL;
	parser_expr(__compiler, &r);
	conv(__compiler, &r, r, l->_type);
	ast_binop(__compiler, __node, op, l->_type, l, r);
_end:
	retok;
}

ff_err_t
parser_assign_expr(ff_compilerp __compiler, struct node **__node) {
	if (next_token_is(__compiler, _tok_keywd, _eq)) {
		struct node *l = *__node;
		struct node *r = NULL;
		parser_expr(__compiler, &r);
		conv(__compiler, &r, r, l->_type);
		
		ast_assign(__compiler, __node, l, r, l->_type);
	}
	retok;
}

ff_err_t parser_func_call(ff_compilerp, struct node**, struct node*);

ff_err_t
parser_postfix_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = peek_token(__compiler);
	if (tok->kind == _tok_keywd && tok->id == _l_paren) {
		parser_func_call(__compiler, __node, *__node);
	}
}

ff_err_t
parser_conditional_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	if (tok->kind == _tok_keywd && (tok->id == _eqeq || tok->id == _neq)) {
		struct node *l = *__node, *r;
		parser_expr(__compiler, &r);

		struct type *_type;
		ffc_build_type(__compiler, &_type, u8_t);
		ff_u8_t op;
		if (tok->id == _eqeq)
			op = _op_eq;
		else if (tok->id == _neq)
			op = _op_neq;
		ast_binop(__compiler, __node, op, _type, l, r);
		retok;
	}

	ffly_ulex(&__compiler->lexer, tok);
}

ff_err_t
parser_unary_addrof(ffly_compilerp __compiler, struct node **__node) {
	struct node *_node;
	parser_expr(__compiler, &_node);

	struct type *_type;
	mk_ptr_type(__compiler, &_type, (*__node)->_type);
	ast_uop(__compiler, __node, _ast_addrof, _node, _type);
}

ff_err_t
parser_unary_expr(ff_compilerp __compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	switch(tok->id) {
		case _ampersand: parser_unary_addrof(__compiler, __node); goto _sk;
	}
	ffly_ulex(&__compiler->lexer, tok);
_sk:
	retok;
}

ff_err_t
parser_expr(ff_compilerp __compiler, struct node **__node) {
	parser_primary_expr(__compiler, __node);
	parser_postfix_expr(__compiler, __node);
	parser_additive_expr(__compiler, __node);
	parser_conditional_expr(__compiler, __node);
	parser_unary_expr(__compiler, __node);
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
		case _k_u16_t:
			tmpl = u16_t;
		break;
		case _k_u32_t:
			tmpl = u32_t;
		break;
		case _k_u64_t:
			tmpl = u64_t;
		break;
	}
	ffc_build_type(__compiler, __type, tmpl);
}

ff_err_t
parser_decl_spec(ff_compilerp __compiler, struct token *__tok, struct type **__type) {
	if (__tok->kind == _tok_ident) {
		ff_err_t err;
		*__type = ffc_get_typedef(__compiler, (char*)__tok->p, &err);
	} else if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
		mk_notype(__compiler, __type, __tok->id);	
	} else if (__tok->id == _k_void) {
		*__type = NULL;
	}
	retok;
}

ff_err_t
parser_declarator(ffly_compilerp __compiler, struct type **__type, struct type *__base_type) {
	if (next_token_is(__compiler, _tok_keywd, _astrisk)) {
		mk_ptr_type(__compiler, __type, __base_type);
		parser_declarator(__compiler, __type, *__type);
	} else
		*__type = __base_type;
}

ff_err_t
parser_decl_init(ff_compilerp __compiler, struct node **__node) {
	parser_expr(__compiler, __node);
}

ff_err_t
parser_decl(ff_compilerp __compiler, struct node **__node) {
	struct type *_type;
	struct node *init = NULL;
	ff_u8_t is_typedef = next_token_is(__compiler, _tok_keywd, _k_typedef);

	parser_decl_spec(__compiler, next_token(__compiler), &_type);
	parser_declarator(__compiler, &_type, _type);

	struct token *name = next_token(__compiler);	

	if (is_typedef)
		ffly_map_put(&__compiler->typedefs, name->p, ffly_str_len((char*)name->p), _type);
	else {
		if (next_token_is(__compiler, _tok_keywd, _eq))
			parser_decl_init(__compiler, &init);

		struct node *var = NULL;
		ast_var(__compiler, &var, _type);
		ffly_map_put(ffc_get_env(__compiler), name->p, ffly_str_len((char*)name->p), var);

		if (__compiler->var_pond != NULL) {
			struct node **p;
			ffly_vec_push_back(__compiler->var_pond, (void**)&p);
			*p = var;
		}

		ast_decl(__compiler, __node, var, init);
	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
	}
}

ff_err_t 
parser_out_stmt(ff_compilerp __compiler, struct node **__node) {
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {}
	struct node *var;
	parser_expr(__compiler, &var);
	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	ast_out(__compiler, __node, var);
}

ff_err_t
parser_as_stmt(ff_compilerp __compiler, struct node **__node) {
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
		ff_token_free(tok);
		cur++;
	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	ast_as(__compiler, __node, (char const*)p);

	retok;
}

ff_err_t
parser_label(ff_compilerp __compiler, struct node **__node, struct token *__name) {
	ast_label(__compiler, __node, __name->p);
}

ff_err_t 
parser_goto_stmt(ff_compilerp __compiler, struct node **__node) {
	struct token *label = next_token(__compiler);	

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}
	ast_jmpto(__compiler, __node, label->p);
}

ff_err_t
parser_ret_stmt(ff_compilerp __compiler, struct node **__node) {
	struct node *retval;
	parser_expr(__compiler, &retval);

	ast_ret(__compiler, __node, retval, __compiler->ret_type);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}
	retok;
}

ff_err_t
parser_exit_stmt(ff_compilerp __compiler, struct node **__node) {
	struct node *code;
	parser_expr(__compiler, &code);
	ast_exit(__compiler, __node, code);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
	}
	retok;
}

ff_bool_t is_stmt(ff_compilerp __compiler) {
	ff_bool_t res;
	ff_u8_t id;
	struct token *tok = next_token(__compiler);
	if (tok->kind != _tok_keywd) {
		struct token *tok;	
		if (!(tok = next_token(__compiler)))
			goto _r;
		ffly_ulex(&__compiler->lexer, tok);
		if (tok->kind == _tok_keywd && tok->id == _colon)
			goto _end;
	}
_r:
	id = tok->id;
	res = (id == _k_exit || id == _k_out || id == _k_as || id == _k_ret || id == _k_goto || id == _k_if);
_end:
	ffly_ulex(&__compiler->lexer, tok);
	return res;
}

ff_err_t
parser_if_stmt(ff_compilerp __compiler, struct node **__node) {
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	struct node *cond;
	parser_expr(__compiler, &cond);

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

	}

	struct ffly_vec block;
	ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
	ffly_vec_init(&block, sizeof(struct token*));
	parser_compound_stmt(__compiler, &block);	
	ast_if(__compiler, __node, cond, &block);
}

ff_err_t
parser_stmt(ff_compilerp __compiler, struct node **__node) {
	struct token *tok;
	tok = next_token(__compiler);
	if (tok->kind == _tok_keywd) {
		switch(tok->id) {
			case _k_exit:	return parser_exit_stmt(__compiler, __node);
			case _k_out:	return parser_out_stmt(__compiler, __node);
			case _k_as:		return parser_as_stmt(__compiler, __node);
			case _k_ret:	return parser_ret_stmt(__compiler, __node);
			case _k_goto:	return parser_goto_stmt(__compiler, __node);
			case _k_if:   return parser_if_stmt(__compiler, __node);
		}
	}

	if (tok->kind == _tok_ident
		&& next_token_is(__compiler, _tok_keywd, _colon))
	{
		return parser_label(__compiler, __node, tok);
	}
	
	ffly_ulex(&__compiler->lexer, tok);
	retok;
}

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

	if (is_type(__compiler, tok))
		parser_decl(__compiler, &nod);
	else if (is_stmt(__compiler))
		parser_stmt(__compiler, &nod);
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

ff_bool_t static
is_func_decl(ff_compilerp __compiler) {
	struct ffly_vec buf;
	ffly_vec_set_flags(&buf, VEC_AUTO_RESIZE);
	ffly_vec_init(&buf, sizeof(struct token*));
	struct token *tok;
	struct token **p;
	ff_bool_t res = 0;
	struct token **itr;
	ff_err_t err;
_again:
	if (!(tok = ffly_lex(&__compiler->lexer, &err))) {
			goto _r;
	}

	ffly_vec_push_back(&buf, (void**)&p);
	*p = tok;

	if (!(tok->kind == _tok_keywd && tok->id == _r_paren))
		goto _again;

	if (!(tok = ffly_lex(&__compiler->lexer, &err))) {
		goto _r;
	}
	res = (tok->kind == _tok_keywd && tok->id == _semicolon);
	ffly_ulex(&__compiler->lexer, tok);
_r: // retreat
	___ffly_vec_nonempty(&buf) {
		itr = (struct token**)ffly_vec_end(&buf);
		while(itr >= (struct token**)ffly_vec_beg(&buf)) {
			ffly_ulex(&__compiler->lexer, *(itr--));
		}
	}

	ffly_vec_de_init(&buf);
	return res;
}

ff_err_t parser_func_decl(ff_compilerp __compiler, struct node **__node) {
	struct type *ret_type;
	parser_decl_spec(__compiler, next_token(__compiler), &ret_type);

	struct token *name;
	name = next_token(__compiler);

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

	}

	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {

	}

	struct ffly_vec block, var_pond, args; // dummy
	ast_func(__compiler, __node, name->p, &block, _func_exr, &var_pond, &args, ret_type);
	ffly_map_put(&__compiler->env, name->p, ffly_str_len((char*)name->p), *__node);
}

ff_err_t
parser_func_def(ff_compilerp __compiler, struct node **__node) {
	struct type *ret_type;
	parser_decl_spec(__compiler, next_token(__compiler), &ret_type);

	struct token *name;
	name = next_token(__compiler);

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {

	}

	ff_u8_t flags = _func_gbl|_func_def;
	struct ffly_vec args, var_pond;
	ffly_vec_set_flags(&args, VEC_AUTO_RESIZE);
	ffly_vec_init(&args, sizeof(struct node*));

	struct ffly_map local, *pre_local;
	ffly_map_init(&local, _ffly_map_127);
	pre_local = __compiler->local;
	__compiler->local = &local;

	if (!next_token_is(__compiler, _tok_keywd, _r_paren)) {
	_next:
		{
			struct type *_type;
			struct node *decl, *var;

			parser_decl_spec(__compiler, next_token(__compiler), &_type);
			parser_declarator(__compiler, &_type, _type);
			struct token *name = next_token(__compiler);

			ast_var(__compiler, &var, _type);

			ffly_map_put(&local, name->p, ffly_str_len((char*)name->p), var);

			ast_decl(__compiler, &decl, var, NULL);

			struct node **p;
			ffly_vec_push_back(&args, (void**)&p);
			*p = decl;

			if (next_token_is(__compiler, _tok_keywd, _comma)) {
				goto _next;
			}
			if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

			}
		}
	}

	ffly_vec_set_flags(&var_pond, VEC_AUTO_RESIZE);
	ffly_vec_init(&var_pond, sizeof(struct node*));
	ffly_vecp pre_vars = __compiler->var_pond;
	__compiler->var_pond = &var_pond;

	struct ffly_vec block;
	ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
	ffly_vec_init(&block, sizeof(struct node*));
	__compiler->ret_type = ret_type;
	parser_compound_stmt(__compiler, &block);
	__compiler->local = pre_local;
	__compiler->var_pond = pre_vars;

	ffly_map_de_init(&local);
	struct node *nod;
	ff_err_t err;
	if ((nod = (struct node*)ffly_map_get(&__compiler->env, name->p, ffly_str_len((char*)name->p), &err)) != NULL) {
		nod->block = block;
		nod->flags = flags;
		nod->var_pond = var_pond;
		nod->args = args;
		goto _end;
	}

	ast_func(__compiler, __node, name->p, &block, flags, &var_pond, &args, ret_type);
	ffly_map_put(&__compiler->env, name->p, ffly_str_len((char*)name->p), *__node);
_end:
	retok;
}

ff_err_t
parser_func_call(ff_compilerp __compiler, struct node **__node, struct node *__func) {
	char *name;
	name = __func->p;

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
	}

	 struct ffly_vec params;
	ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
	ffly_vec_init(&params, sizeof(struct node*));

	if (!next_token_is(__compiler, _tok_keywd, _r_paren)) {
	_next:
		{
			struct node *nod;
			parser_expr(__compiler, &nod);

			struct node **p;
			ffly_vec_push_back(&params, (void**)&p);
			*p = nod;

			if (next_token_is(__compiler, _tok_keywd, _comma))
				goto _next;
			if (!expect_token(__compiler, _tok_keywd, _r_paren)) {

			}
		}
	}

	struct node **arg = (struct node**)ffly_vec_beg(&__func->args);
	struct node **itr, *param;
	___ffly_vec_nonempty(&params) {
		itr = (struct node**)ffly_vec_beg(&params);
		while(itr <= (struct node**)ffly_vec_end(&params)) {
			param = *(itr++);
			conv(__compiler, itr-1, param, (*arg)->var->_type);
			arg++;
		}
	}

	ast_func_call(__compiler, __node, name, &params, __func->_type);
	retok;
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
			if (is_func_decl(__compiler))
				parser_func_decl(__compiler, &nod);
			else
				parser_func_def(__compiler, &nod);
		} else if (is_type(__compiler, tok) || tok->id == _k_typedef) {
			parser_decl(__compiler, &nod);
		} else if (is_stmt(__compiler)) {
			parser_stmt(__compiler, &nod);
		} else  {
			parser_expr(__compiler, &nod);
			if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
			}
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
