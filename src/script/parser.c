# define __ffly_compiler_internal
# define __ffly_parser
# include "../compiler.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../system/nanosleep.h"
# include "../dep/mem_cpy.h"
# include "../system/string.h"
# include "../dep/str_len.h"

ffly_mapp ffc_get_env(struct ffly_compiler*);
void ffc_build_node(struct ffly_compiler*, struct node**, struct node*);
void ffc_build_type(struct ffly_compiler*, struct type**, struct type*);
struct type* ffc_get_typedef(struct ffly_compiler*, char*, ff_err_t*);

struct type static *float_t = &(struct type){.kind=_float, .size=sizeof(double)};

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

struct type static *ptr = &(struct type){.kind=_u64_t, .size=8};
void static
mk_notype(ffly_compilerp __compiler, struct type **__type, ff_u8_t __id) {
	struct type *tmpl;
	switch(__id) {
		case _k_float:
			tmpl = float_t;
		break;
		case _k_uint_t:
			tmpl = uint_t;
		break;
		case _k_int_t:
			tmpl = int_t;
		break;
		case _k_u64_t:
			tmpl = u64_t;
		break;
		case _k_i64_t:
			tmpl = i64_t;
		break;
		case _k_u32_t:
			tmpl = u32_t;
		break;
		case _k_i32_t:
			tmpl = i32_t;
		break;
		case _k_u16_t:
			tmpl = u16_t;
		break;
		case _k_i16_t:
			tmpl = i16_t;
		break;
		case _k_u8_t:
			tmpl = u8_t;
		break;
		case _k_i8_t:
			tmpl = i8_t;
		break;
		default:
			ffly_printf("unknown...\n");
			return;
	}
	ffc_build_type(__compiler, __type, tmpl);
}

void static
mk_arrtype(ffly_compilerp __compiler, struct type **__type, struct type *__base, ff_uint_t __len) {
	ffc_build_type(__compiler, __type, &(struct type){.kind=_array, .ptr=__base, .size=(__len*__base->size), .len=__len});
}

void static
ast_decl(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__var, struct node *__init)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_decl, .var=__var, .init=__init, ._type=NULL});
}

void static
ast_int_type(struct ffly_compiler *__compiler, struct node **__node,
	struct type *__type, ff_byte_t *__p)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_literal, ._type=__type});
	ffly_mem_cpy((*__node)->val, __p, __type->size);
}

void static
ast_print_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__arg) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_print_call, .arg=__arg, ._type=NULL});
}

void static
ast_var(struct ffly_compiler *__compiler, struct node **__node, struct type *__type) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_var, ._type=__type});
}

void static
ast_if(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__cond, struct ffly_vec *__do, struct ffly_vec *__else)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_if, .cond=__cond, ._do=*__do, ._else=*__else, ._type=NULL});
}

void static
ast_func(struct ffly_compiler *__compiler, struct node **__node, struct ffly_vec *__params,
	struct ffly_vec *__block, struct type *__type, ff_bool_t __va)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func, .params=*__params, .block=*__block, ._type=__type, .va=__va});
}

void static
ast_func_call(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__call, struct ffly_vec *__args, struct node **__va_arg)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_func_call, .call=__call, .args=*__args, ._type=__call->_type, .va_arg=__va_arg});
}

void static
ast_struct_ref(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__struct, struct type *__type)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_struct_ref, ._struct=__struct, ._type=__type});
}

void static
ast_assign(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__l, struct node *__r, struct type *__type)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_assign, .l=__l, .r=__r, ._type=__type});
}

void static
ast_exit(struct ffly_compiler *__compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_exit});
}

void static
ast_while(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__cond, struct ffly_vec *__block)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_while, .cond=__cond, .block=*__block, ._type=NULL});
}

void static
ast_str(struct ffly_compiler *__compiler, struct node **__node, char *__s, ff_uint_t __len) {
	struct type *_type;
	mk_arrtype(__compiler, &_type, u8_t, __len+1);

	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_literal, ._type=_type});
	(*__node)->p = __s;
}

void static
ast_va_args(struct ffly_compiler *__compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_va_args});
}

void static
ast_uop(struct ffly_compiler *__compiler, struct node **__node,
	ff_u8_t __kind, struct node *__operand, struct type *__type) {

	ffc_build_node(__compiler, __node, &(struct node){.kind=__kind, .operand=__operand, ._type=__type});
}

void static
ast_match(struct ffly_compiler *__compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_match});
}

void static
ast_call(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__no, struct ffly_vec *__params)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_call, .no=__no, .params=*__params});
}

void static
ast_ret(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__ret, struct type *__type)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_ret, .ret=__ret, ._type=__type});
}

void static
ast_conv(struct ffly_compiler *__compiler, struct node **__node,
	struct type *__to, struct node *__operand)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_conv, ._type=__to, .operand=__operand});
}

void static
ast_brk(struct ffly_compiler *__compiler, struct node **__node) {
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_brk});
}

void static
ast_binop(struct ffly_compiler *__compiler, struct node **__node,
	ff_u8_t __op, struct type *__type, struct node *__l, struct node *__r)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=__op, ._type=__type, .l=__l, .r=__r});
}

void static
ast_syput(struct ffly_compiler *__compiler, struct node **__node,
	struct node *__p, char const *__name)
{
	ffc_build_node(__compiler, __node, &(struct node){.kind=_ast_syput, .name=__name});
	(*__node)->p = __p;
}

void static
mk_ptr_type(struct ffly_compiler *__compiler, struct type **__type, struct type *__ptr) {
	ffc_build_type(__compiler, __type, &(struct type){.kind=_u64_t, .ptr=__ptr, .size=8, .len=0});
}

ff_bool_t static
is_type(struct ffly_compiler *__compiler, struct token *__tok) {
	ff_err_t err;
	if (__tok->kind == _tok_ident) { 
		struct type *_type = ffc_get_typedef(__compiler, (char*)__tok->p, &err);
		if (_ok(err) && _type != NULL) return 1;
		return 0;
	}

	if (__tok->kind != _tok_keywd) return 0;
	return ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_struct
		|| __tok->id == _k_var || __tok->id == _k_float);
}
	
ff_bool_t static
is_stmt(struct token *__tok) {
	return (__tok->id == _k_if || __tok->id == _k_exit
		|| __tok->id == _k_while || __tok->id == _k_match);
}

ff_err_t static
conv(struct ffly_compiler *__compiler, struct node **__node, struct node *__operand, struct type *__to) {
	ast_conv(__compiler, __node, __to, __operand);
	retok;
}

ff_err_t static parser_func_call(struct ffly_compiler*, struct node**, struct node*);
ff_err_t static parser_decl_spec(struct ffly_compiler*, struct token*, struct type**);
ff_err_t static parser_struct_decl(struct ffly_compiler*, struct ffly_map*, ff_uint_t*);
ff_err_t static parser_struct_spec(struct ffly_compiler*, struct type**);
ff_err_t static parser_struct_field(struct ffly_compiler*, struct node**, struct node*);
ff_err_t static parser_declarator(struct ffly_compiler*, struct type**, struct type*, char**);
ff_err_t static parser_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_decl(struct ffly_compiler*, struct node**);
ff_err_t static parser_print_call(struct ffly_compiler*, struct node**);
ff_err_t static parser_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_primary_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_conditional_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_unary_addrof(struct ffly_compiler*, struct node**);
ff_err_t static parser_unary_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_postfix_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_assign_expr(struct ffly_compiler*, struct node**);
struct type static* parser_cast_type(struct ffly_compiler*, ff_err_t*);
ff_err_t static parser_cast_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_binary_expr(struct ffly_compiler*, struct node**);
ff_err_t static parser_decl_init(struct ffly_compiler*, struct node**);
ff_err_t static parser_brk_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_call(struct ffly_compiler*, struct node**);
ff_bool_t static is_func_call(struct ffly_compiler*);
ff_err_t static parser_compound_stmt(struct ffly_compiler*, struct ffly_vec*);
ff_err_t static parser_if_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_match_stmt(struct ffly_compiler*, struct node**); 
ff_err_t static parser_exit_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_ret_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_while_stmt(struct ffly_compiler*, struct node**);
ff_err_t static parser_func_def(struct ffly_compiler*, struct node**);
ff_err_t static parser_call(struct ffly_compiler*, struct node**);

ff_err_t
parser_struct_decl(struct ffly_compiler *__compiler, struct ffly_map *__fields, ff_uint_t *__size) {
	if (!expect_token(__compiler, _tok_keywd, _l_brace)) {
		errmsg("expected error.\n");
		reterr;
	}

	ff_off_t off = 0;
	ff_err_t err;
	if (!next_token_is(__compiler, _tok_keywd, _r_brace)) {
	_next:
		{
			struct token *tok = next_token(__compiler);
			struct token *name = next_token(__compiler);
			struct type *_type;
			if (_err(err = parser_decl_spec(__compiler, tok, &_type))) {
				errmsg("declarator specifier, failure.\n");
				reterr;
			}

			(*__size)++;
			_type->off = off++;
			ffly_map_put(__fields, name->p, ffly_str_len((char*)name->p), _type);
			if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
				reterr;
			}

			if (!next_token_is(__compiler, _tok_keywd, _r_brace)) {
				goto _next;
			}
		}
	}
	retok;
}

ff_err_t
parser_struct_spec(struct ffly_compiler *__compiler, struct type **__type) {
	struct token *name = next_token(__compiler);
	if (name->kind != _tok_ident) {
		errmsg("diden't get identifier.\n");
		reterr;
	}

	ff_err_t err;
	*__type = (struct type*)ffly_map_get(ffc_get_env(__compiler), name->p, ffly_str_len((char*)name->p), &err);
	if (*__type != NULL) {
		// err
		reterr;
	}

	struct ffly_map fields;    
	ffly_map_init(&fields, _ffly_map_127);
	ff_uint_t size;
	if (_err(err = parser_struct_decl(__compiler, &fields, &size))) {
		// err
		reterr;
	}

	ffc_build_type(__compiler, __type, &(struct type){.kind=_struct, .size=size, .fields=fields});
	ffly_map_put(ffc_get_env(__compiler), name->p, ffly_str_len((char*)name->p), *__type);
	retok;
}

ff_err_t
parser_struct_field(struct ffly_compiler *__compiler, struct node **__node, struct node *__struct) {
	struct token *name = next_token(__compiler); 
	ff_err_t err;
	struct type *_type = (struct type*)ffly_map_get(&__struct->_type->fields, name->p, ffly_str_len((char*)name->p), &err);
	if (!_type) {
		errmsg("structure field doesen't exist.\n");
		reterr;
	}
	
	ast_struct_ref(__compiler, __node, __struct, _type);
	retok;
}

ff_err_t
parser_decl_spec(struct ffly_compiler *__compiler, struct token *__tok, struct type **__type) {
	if (__tok->kind == _tok_ident) {
		ff_err_t err;
		struct type *_type = ffc_get_typedef(__compiler, (char*)__tok->p, &err);
		if (_ok(err) && _type != NULL) {
			ffc_build_type(__compiler, __type, _type);
			retok;
		}
		reterr;
	}

	if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
		mk_notype(__compiler, __type, __tok->id);
	} else if (__tok->id == _k_struct) {
		parser_struct_spec(__compiler, __type);
	} else if (__tok->id == _k_void) {
		*__type = NULL;
	} else if (__tok->id == _k_var) {
		ffc_build_type(__compiler, __type, &(struct type){.kind=_unknown});
	} else {
		if (!is_type(__compiler, __tok))
			errmsg("token mismatch.\n");
		else
			errmsg("unknown type.\n");
		reterr;
	}
	retok;
}

void
read_no(struct ffly_compiler *__compiler, struct node **__node, char *__s, ff_u8_t __is_hex, ff_u8_t __is_float) {
	ff_u8_t sign = *__s == '-';
	struct type *_type;
	ff_byte_t val[sizeof(ff_u64_t)];
	if (__is_float) {
		*(double*)val = ffly_stfloat(__s);
		_type = float_t;
	} else {
		ff_u64_t no;
		if (__is_hex) {
			if (ffly_islen(__s, 2) == -1)
				if (*__s == '0' && ffly_tolow(*(__s+1)) == 'x') __s+=2;
			no = ffly_htint(__s);
		} else
			no = ffly_stno(__s);

		if (sign) no = -no;
		if (no >= 0 && no <= (ff_u8_t)~0)
			_type = sign?i8_t:u8_t;
		else if (no > (ff_u8_t)~0 && no <= (ff_u16_t)~0)
			_type = sign?i16_t:u16_t;
		else if (no > (ff_u16_t)~0 && no <= (ff_u32_t)~0)
			_type = sign?i32_t:u32_t;
		else if (no > (ff_u32_t)~0 && no <= (ff_u64_t)~0)
			_type = sign?i64_t:u64_t;
		*(ff_u64_t*)val = sign?-no:no;
	}

	ffc_build_type(__compiler, &_type, _type);
	ast_int_type(__compiler, __node, _type, val);
}

ff_err_t
parser_primary_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	ff_err_t err;
	if (!tok) {
		retok;
	}

	switch(tok->kind) {
		case _tok_ident: {
			if (__compiler->local != NULL) {
				*__node = (struct node*)ffly_map_get(__compiler->local, tok->p, ffly_str_len((char*)tok->p), &err);
				if (*__node != NULL) break;   
			}

			*__node = (struct node*)ffly_map_get(&__compiler->env, tok->p, ffly_str_len((char*)tok->p), &err);
			if (!*__node) {
				errmsg("%u;%u: '%s' could not find in environment.\n", tokl(tok), tokcol(tok), (char*)tok->p);
				return FFLY_FAILURE;
			}
			break;
		}
		case _tok_str:
			ast_str(__compiler, __node, tok->p, tok->l);
		break;
		case _tok_no:
			read_no(__compiler, __node, (char*)tok->p, tok->is_hex, tok->is_float);
		break;
		case _tok_chr: {
			struct type *_type;
			ffc_build_type(__compiler, &_type, u8_t);
			ast_int_type(__compiler, __node, _type, tok->p);
			break;
		}
		default:
			*__node = NULL;
			ffly_ulex(&__compiler->lexer, tok);
			goto _sk;
	}
	ff_token_free(tok);
_sk:
	retok;
}

ff_err_t
parser_conditional_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	if (is_keyword(tok, _eqeq) || is_keyword(tok, _neq) || is_keyword(tok, _gt) || is_keyword(tok, _lt)) {
		struct node *l = *__node;
		struct node *r;
		ff_err_t err;
		if (_err(err = parser_primary_expr(__compiler, &r))) {
			_ret;
		}
		struct type *_type;
		ffc_build_type(__compiler, &_type, u8_t);

		ff_u8_t op;
		if (is_keyword(tok, _eqeq))
			op = _op_eq;
		else if (is_keyword(tok, _neq))
			op = _op_neq;
		else if (is_keyword(tok, _gt))
			op = _op_gt;
		else if (is_keyword(tok, _lt))
			op = _op_lt;
		else
			reterr;
		ast_binop(__compiler, __node, op, _type, l, r);
		ff_token_free(tok);
		retok;
	}

	ffly_ulex(&__compiler->lexer, tok);
	retok;
}

ff_err_t
parser_unary_addrof(struct ffly_compiler *__compiler, struct node **__node) {
	struct node *_node;
	ff_err_t err;
	if (_err(err = parser_primary_expr(__compiler, &_node)))
		_ret;
	
	struct type *_type;
	ffc_build_type(__compiler, &_type, u64_t);
	ast_uop(__compiler, __node, _ast_addrof, _node, _type);
	return FFLY_SUCCESS;
}

ff_err_t
parser_sizeof_operand(struct ffly_compiler *__compiler, struct node **__node) {
	ff_u8_t size = 0;

	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		reterr;
	}

	struct token *tok = next_token(__compiler);
	if (tok->kind != _tok_keywd) {
		// error
	}

	switch(tok->id) {
		case _k_uint_t:
			size = sizeof(ff_uint_t);
		break;
		case _k_u64_t:
			size = 8;
		break;
		case _k_u32_t:
			size = 4;
		break;
		case _k_u16_t:
			size = 2;
		break;
		case _k_u8_t:
			size = 1;
		break;
		default:
			ffly_printf("error, unknown type, got: %u\n", tok->id);
	}

	ff_token_free(tok);
	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		reterr;
	}

	struct type *_type;
	ffc_build_type(__compiler, &_type, u8_t);
	ast_int_type(__compiler, __node, _type, &size);
}

ff_err_t
parser_unary_expr(struct ffly_compiler *__compiler, struct node **__node) {
	if (next_token_is(__compiler, _tok_keywd, _incr))
		ast_uop(__compiler, __node, _ast_incr, *__node, (*__node)->_type);
	else if (next_token_is(__compiler, _tok_keywd, _decr))
		ast_uop(__compiler, __node, _ast_decr, *__node, (*__node)->_type);
	else {
		struct token *tok = next_token(__compiler);
		switch(tok->id) {
			case _k_sizeof: parser_sizeof_operand(__compiler, __node); goto _sk;
			case _ampersand: parser_unary_addrof(__compiler, __node); goto _sk;
		}

		ffly_ulex(&__compiler->lexer, tok);
		goto _end;
	_sk:
		ff_token_free(tok);
	}
_end:
	retok;
}

ff_err_t
parser_postfix_expr(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	if (next_token_is(__compiler, _tok_keywd, _period)) {
		if (_err(err = parser_struct_field(__compiler, __node, *__node))) {
			errmsg("failed to read struct field.\n");
			_ret;
		}
		retok;
	}

	struct token *tok = peek_token(__compiler);
	if (tok->kind == _tok_keywd && tok->id == _l_paren) {
		if (_err(err = parser_func_call(__compiler, __node, *__node))) {
			errmsg("failed to read function call.\n");
			_ret;
		}	 
	}
	retok;
}

ff_err_t
parser_assign_expr(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	if (next_token_is(__compiler, _tok_keywd, _eq)) {
		struct node *l = *__node;
		struct node *r = NULL;
		if (_err(err = parser_expr(__compiler, &r))) {
			errmsg("failed to read expression.\n");
			_ret;
		}

		ffly_printf("---> %u, %u\n", l->_type->size, r->_type->size);
		if (l->_type->kind != r->_type->kind)
			conv(__compiler, &r, r, l->_type);
		ast_assign(__compiler, __node, l, r, l->_type);  
	}
	retok;
}

struct type*
parser_cast_type(struct ffly_compiler *__compiler, ff_err_t *__err) {
	struct type *_type;
	struct token *tok = next_token(__compiler);
	if (!is_type(__compiler, tok)) {
		errmsg("got non type.\n");
		*__err = FFLY_FAILURE;
		return NULL;
	}

	*__err = FFLY_SUCCESS;
	if (_err(*__err = parser_decl_spec(__compiler, tok, &_type))) {
		errmsg("failed to read declaration specifier.\n");
		return NULL;
	}

	if (_err(*__err = parser_declarator(__compiler, &_type, _type, NULL))) {
		errmsg("failed to read declarator.\n");
		return NULL;
	} 
	ff_token_free(tok);
	return _type;
}

ff_err_t
parser_cast_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	ff_err_t err;
	if (is_keyword(tok, _l_paren) && is_type(__compiler, peek_token(__compiler))) {
		struct type *_type = parser_cast_type(__compiler, &err);
		if (_err(err)) {
			errmsg("failed to read cast type.\n");
			reterr;
		}
		if (!_type) {
			errmsg("somthing went wong.\n");
			reterr;
		} 

		if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
			reterr;
		}

		struct node *n;
		if (_err(err = parser_expr(__compiler, &n))) {
			errmsg("failed to read expression.\n");
			_ret;
		}
		ast_uop(__compiler, __node, _op_cast, n, _type);
		ff_token_free(tok);
		retok; 
	}

	ffly_ulex(&__compiler->lexer, tok);
	retok;
}

ff_err_t
parser_binary_expr(struct ffly_compiler *__compiler, struct node **__node) {
/*
	struct token *tok = next_token(__compiler);
	ff_u8_t op;
	if (is_keyword(tok, _astrisk)) {
		op = _opr_mul;
	} else if (is_keyword(tok, _plus)) {
		op = _opr_add;
	} else if (is_keyword(tok, _minus)) {
		op = _opr_sub;
	} else {
		ffly_ulex(&__compiler->lexer, tok);
		retok;
	}
	
	struct node *l = *__node, *r = NULL;
	parser_expr(__compiler, &r);

	if (l->_type->kind != r->_type->kind)
		conv(__compiler, &r, r, l->_type);

	ast_binop(__compiler, __node, op, l->_type, l, r);
*/
	retok;
}

ff_err_t
parser_expr(struct ffly_compiler *__compiler, struct node **__node) {
	*__node = NULL;
	ff_err_t err;
	if (next_token_is(__compiler, _tok_keywd, _k_va_args)) {
		ast_va_args(__compiler, __node);
		retok;
	}
	if (_err(err = parser_primary_expr(__compiler, __node))) {
		errmsg("failed to read primary expression.\n");
		_ret;
	}
	if (_err(err = parser_cast_expr(__compiler, __node))) {
		errmsg("failed to read cast expression.\n");
		_ret;
	}
	if (_err(err = parser_postfix_expr(__compiler, __node))) {
		errmsg("failed to read postfix expression.\n");
		_ret;
	}
	if (_err(err = parser_unary_expr(__compiler, __node))) {
		errmsg("failed to read unary expression.\n");
		_ret;
	}
	if (_err(err = parser_conditional_expr(__compiler, __node))) {
		errmsg("failed to read conditional expression.\n");
		_ret;
	}
	if (_err(err = parser_assign_expr(__compiler, __node))) {
		errmsg("failed to read assign expression.\n");
		_ret;
	}
	retok;
}

ff_err_t
parser_decl_init(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	*__node = NULL;
	if (_err(err = parser_expr(__compiler, __node))) {
		errmsg("failed to read expression.\n");
		_ret;
	}
	if (!*__node) {
		errmsg("somthing went wrong.\n");
		reterr;
	}
	retok;
}

ff_err_t
parser_brk_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		reterr;
	}

	ast_brk(__compiler, __node);
	retok;
}

ff_err_t
parser_compound_stmt(struct ffly_compiler *__compiler, struct ffly_vec *__vec) {
	if (!expect_token(__compiler, _tok_keywd, _l_brace)) {
		reterr;
	}

	ff_err_t err = FFLY_SUCCESS;
	struct token *tok;
	struct node *_node;
	struct node **p;
_again:
	tok = peek_token(__compiler);
	if (is_keyword(tok, _r_brace)) {
		sktok(__compiler);
		goto _end;
	}

	if (is_type(__compiler, tok) || tok->id == _k_typedef) {
		if (_err(err = parser_decl(__compiler, &_node))) {
			errmsg("failed to read decl.\n");
			goto _end;
		}
	} else if (tok->kind == _tok_keywd) {
		if (tok->id == _k_print) {
			if (_err(err = parser_print_call(__compiler, &_node))) {
				errmsg("failed to read print call.\n");
				goto _end;
			}
		} else if (tok->id == _colon) {
			if (_err(err = parser_call(__compiler, &_node))) {
				errmsg("failed to read call.\n");
				goto _end;
			}
		} else if (tok->id >= _k_uint_t && tok->id <= _k_i8_t) {
			if (_err(err = parser_decl(__compiler, &_node))) {
				errmsg("failed to read decl.\n");
				goto _end;
			}
		} else {
			if (is_stmt(tok) || tok->id == _k_ret || tok->id == _k_brk) {
				if (_err(err = parser_stmt(__compiler, &_node))) {
					errmsg("failed to read statment.\n");
					goto _end;
				}
			}
		}
	} else if (tok->kind == _tok_ident) {
		if (_err(err = parser_expr(__compiler, &_node))) {
			errmsg("failed to read expression.\n");
			goto _end;
		}
		if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
			errmsg("expect error.\n");
			goto _end;
		}
	}

	ffly_vec_push_back(__vec, (void**)&p);
	*p = _node;
	goto _again;
_end:
	if (_err(err)) _ret;
	retok;
}

ff_err_t
parser_declarator(struct ffly_compiler *__compiler, struct type **__type, struct type *__base_type, char **__name) {
	if (next_token_is(__compiler, _tok_keywd, _astrisk)) {
		struct type *_type;
		mk_ptr_type(__compiler, &_type, __base_type);
		parser_declarator(__compiler, __type, _type, __name);
		retok;
	} else
		*__type = __base_type;

	if (__name != NULL) {
		struct token *tok = next_token(__compiler);
		if (tok->kind == _tok_ident) {
			*__name = tok->p;
			ff_token_free(tok);
		} else
			ffly_ulex(&__compiler->lexer, tok);
	}
	retok; 
}

ff_err_t
parser_decl(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	struct type *_type;
	struct node *init = NULL;
	ff_u8_t is_typedef = next_token_is(__compiler, _tok_keywd, _k_typedef);
	struct token *tok = next_token(__compiler);
	if (tok->kind != _tok_keywd && tok->kind != _tok_ident) {
		errmsg("%u;%u: wanted keyword got %s.\n", tokl(tok), tokcol(tok), tokk_str(tok->kind));
		reterr;
	}
	if (_err(err = parser_decl_spec(__compiler, tok, &_type))) {
		errmsg("failed to read declaration specifier.\n");
		_ret;
	} 
	ff_token_free(tok);

	if(next_token_is(__compiler, _tok_keywd, _semicolon))
		retok;

	char *name;
	if (_err(err = parser_declarator(__compiler, &_type, _type, &name))) {
		errmsg("failed to read declarator.\n");
		_ret;
	}

	if (is_typedef)
		ffly_map_put(&__compiler->typedefs, name, ffly_str_len(name), _type);
	else {
		struct node *var;
		ast_var(__compiler, &var, _type);

		ffly_map_put(ffc_get_env(__compiler), name, ffly_str_len(name), var);
		if (next_token_is(__compiler, _tok_keywd, _eq)) {
			if (_err(err = parser_decl_init(__compiler, &init))) {
				errmsg("failed to read decl init.\n");
				_ret;
			}

			if (_type->kind == _unknown) {
				_type->kind = init->_type->kind;
				_type->size = init->_type->size;
			}
		}

		ffly_printf("----> var: %s, size: %u, %p\n", name, _type->size, var);
		if (init != NULL) {
			if (init->_type->kind != var->_type->kind)
			   conv(__compiler, &init, init, var->_type);
		}

		ast_decl(__compiler, __node, var, init);
	}
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}
	retok;
}

ff_err_t
parser_print_call(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ff_err_t err;
	struct node *arg;
	if (_err(err = parser_expr(__compiler, &arg))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	ast_print_call(__compiler, __node, arg);
	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		errmsg("expect error.\n");
		reterr;
	}
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}
	retok;
}

ff_err_t
parser_if_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct node *cond;
	if (_err(err = parser_expr(__compiler, &cond))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct ffly_vec _do;
	ffly_vec_set_flags(&_do, VEC_AUTO_RESIZE);
	ffly_vec_init(&_do, sizeof(struct node*));
	if (_err(err = parser_compound_stmt(__compiler, &_do))) {
		errmsg("failed to read compound statment.\n");
		_ret;
	}

	vec_cleanup(__compiler, &_do);

	struct ffly_vec _else;
	ffly_vec_set_flags(&_else, VEC_AUTO_RESIZE);
	ffly_vec_init(&_else, sizeof(struct node*));
	if (next_token_is(__compiler, _tok_keywd, _k_else)) {
		if (_err(err = parser_compound_stmt(__compiler, &_else))) {
			errmsg("failed to read compound statment.\n");
			_ret;
		}
	}

	vec_cleanup(__compiler, &_else);
	ast_if(__compiler, __node, cond, &_do, &_else);
	return FFLY_SUCCESS;
}

ff_err_t
parser_match_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct node *expr;
	ff_err_t err;
	if (_err(err = parser_expr(__compiler, &expr))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	if (!expect_token(__compiler, _tok_keywd, _l_brace)) {
		errmsg("expect error.\n");
		reterr;
	}

	if (!next_token_is(__compiler, _tok_keywd, _r_brace)) {
		_next:
		{
			struct node *val;
			if (_err(err = parser_expr(__compiler, &val))) {
				errmsg("failed to read expression.\n");
				_ret;
			}

			if (!expect_token(__compiler, _tok_keywd, _colon)) {
				errmsg("expect error.\n");
				reterr;
			}

			struct ffly_vec block;
			ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
			ffly_vec_init(&block, sizeof(struct node*));
			if (_err(err = parser_compound_stmt(__compiler, &block))) {
				errmsg("failed to read compound statment.\n");
				_ret;
			}			 

			vec_cleanup(__compiler, &block);
			if (!next_token_is(__compiler, _tok_keywd, _r_brace)) {
				goto _next;
			}
		}
	}
	retok;
}

ff_err_t
parser_exit_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}

	ast_exit(__compiler, __node);
	retok;
}

ff_err_t
parser_ret_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);

	ff_err_t err;
	struct node *ret = NULL;
	if (!next_token_is(__compiler, _tok_keywd, _semicolon)) {
		if (_err(err = parser_expr(__compiler, &ret))) {
			errmsg("failed to read expression.\n");
			_ret;
		}

		if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
			errmsg("expect error.\n");
			reterr;
		}
	}
	ast_ret(__compiler, __node, ret, __compiler->ret_type);
	retok;
}

ff_err_t
parser_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	ff_err_t err;
	struct token *tok;
	if (!(tok = peek_token(__compiler))) {
		// err
	}

	if (is_keyword(tok, _k_if)) {
		if (_err(err = parser_if_stmt(__compiler, __node))) {
			errmsg("failed to read if statment.\n");
			_ret;
		}
	} else if (is_keyword(tok, _k_exit)) {
		if (_err(err = parser_exit_stmt(__compiler, __node))) {
			errmsg("failed to read exit statment.\n");
			_ret;
		}
	} else if (is_keyword(tok, _k_while)) {
		if (_err(err = parser_while_stmt(__compiler, __node))) {
			errmsg("failed to read while statment.\n");
			_ret;
		}
	} else if (is_keyword(tok, _k_ret)) {
		if (_err(err = parser_ret_stmt(__compiler, __node))) {
			errmsg("failed to read return statment.\n");
			_ret;
		}
	} else if (is_keyword(tok, _k_match)) {
		if (_err(err = parser_match_stmt(__compiler, __node))) {
			errmsg("failed to read match statment.\n");
			_ret;
		}
	} else if (is_keyword(tok, _k_brk)) {
		if (_err(err = parser_brk_stmt(__compiler, __node))) {
			errmsg("failed to read break statment.\n");
			_ret;
		}
	}
	retok;
}

ff_err_t
parser_func_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__call) {
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ff_err_t err;
	struct ffly_vec args;
	ffly_vec_set_flags(&args, VEC_AUTO_RESIZE);
	ffly_vec_init(&args, sizeof(struct node*));

	struct node **va_arg = NULL;
	struct node **param = (struct node**)ffly_vec_begin(&__call->params);
	if (!next_token_is(__compiler, _tok_keywd, _r_paren)) {
	_next:
		{
			struct node *arg;
			if (_err(err = parser_expr(__compiler, &arg))) {
				errmsg("failed to read expression.\n");
				return err;
			}	 
	
			if (arg->_type->kind == _array) {
				struct type *_type;
				ffc_build_type(__compiler, &_type, ptr);
				conv(__compiler, &arg, arg, _type);
			}

			if (param <= (struct node**)ffly_vec_end(&__call->params)) {
				if (arg->_type->kind != (*param)->var->_type->kind)
					conv(__compiler, &arg, arg, (*param)->var->_type);
			}	
 
			struct node **p;
			ffly_vec_push_back(&args, (void**)&p);
			*p = arg;

			if (param > (struct node**)ffly_vec_end(&__call->params)) {
				if (!va_arg)
					va_arg = p;
			} else
				param++;
			if (next_token_is(__compiler, _tok_keywd, _comma))
				goto _next;
		}

		if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
			return FFLY_FAILURE;
		}

		if (param-1 != (struct node**)ffly_vec_end(&__call->params)) {
			errmsg("to few args.\n");
			reterr;
		}
	}
 
	vec_cleanup(__compiler, &args);
	ast_func_call(__compiler, __node, __call, &args, va_arg);
	return FFLY_SUCCESS;
}

ff_err_t
parser_func_def(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	ff_err_t err;
	struct token *name = next_token(__compiler);
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ff_bool_t va = 0;
	struct ffly_map local, *save = NULL;
	ffly_map_init(&local, _ffly_map_127);
	save = __compiler->local;
	__compiler->local = &local;

	struct ffly_vec params;
	ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
	ffly_vec_init(&params, sizeof(struct node*)); 
	if (next_token_is(__compiler, _tok_keywd, _ellipsis)) {
		va = 1; 
	}

	if (!next_token_is(__compiler, _tok_keywd, _r_paren)) {
	_next:
		if (next_token_is(__compiler, _tok_keywd, _ellipsis)) {
			va = 1;
			goto _sk;
		}
		{
			struct node *decl, *var = NULL;
			struct token *tok = next_token(__compiler);
			struct type *_type;
			if (_err(err = parser_decl_spec(__compiler, tok, &_type))) {
				errmsg("failed to read declaration specifier.\n");
				return err;
			}

			char *name;
			if (_err(err = parser_declarator(__compiler, &_type, _type, &name))) {
				errmsg("failed to read declarator.\n");
				return err;
			}

			ast_var(__compiler, &var, _type);

			ast_decl(__compiler, &decl, var, NULL);
		 
			struct node **p;
			ffly_vec_push_back(&params, (void**)&p);
			*p = decl;

			ffly_map_put(&local, name, ffly_str_len(name), var);
			ff_token_free(tok);
			if (next_token_is(__compiler, _tok_keywd, _comma)) {  
				goto _next;
			}
		}
	_sk:
		if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
			errmsg("expect error.\n");
			reterr;
		}
	}

	vec_cleanup(__compiler, &params);
	if (!expect_token(__compiler, _tok_keywd, _r_arrow)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct token *tok = next_token(__compiler);
	struct type *ret_type;
	if (_err(err = parser_decl_spec(__compiler, tok, &ret_type))) {
		errmsg("failed to read declaration specifier.\n");
		return err;
	}
	ff_token_free(tok);

	struct ffly_vec block;
	ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
	ffly_vec_init(&block, sizeof(struct node*));

	__compiler->ret_type = ret_type;
	parser_compound_stmt(__compiler, &block); 
	__compiler->local = save;
	ffly_map_de_init(&local);
   
	vec_cleanup(__compiler, &block);
	ast_func(__compiler, __node, &params, &block, ret_type, va);
	ffly_printf("func: %s, %p\n", name->p, *__node);
	ffly_map_put(__compiler->local != NULL?__compiler->local:&__compiler->env, name->p, ffly_str_len((char*)name->p), *__node);
	ff_token_free(name);
	return FFLY_SUCCESS;
}


ff_bool_t
is_func_call(struct ffly_compiler *__compiler) {
	struct token *name, *l_paren;
	ff_u8_t res = 0;	
	if (!(name = next_token(__compiler)))
		goto _r0;
	if (!(l_paren = next_token(__compiler)))
		goto _r1;
	res = !(name->kind != _tok_ident || !is_keyword(l_paren, _l_paren));
	ffly_ulex(&__compiler->lexer, l_paren);
_r1:
	ffly_ulex(&__compiler->lexer, name);
_r0:
	return res;
}

ff_err_t
parser_while_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	ff_err_t err;    
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		reterr;
	}

	struct node *cond;
	if (_err(err = parser_expr(__compiler, &cond))) {
		errmsg("failed to read expression.\n");
		return err;
	}

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		reterr;
	}

	struct ffly_vec block;
	ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
	ffly_vec_init(&block, sizeof(struct node*));
	if (_err(err = parser_compound_stmt(__compiler, &block))) {
		errmsg("failed to read compound statment.\n");
		return err;
	}

	vec_cleanup(__compiler, &block);
	ast_while(__compiler, __node, cond, &block);
	retok;
}

ff_err_t
parser_call(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);

	ff_err_t err;
	struct node *no;
	if (_err(err = parser_expr(__compiler, &no))) {
		errmsg("failed to read expression.\n");
		return err;
	}	 

	if (!expect_token(__compiler, _tok_keywd, _l_arrow)) {
		reterr;
	}

	if (!expect_token(__compiler, _tok_keywd, _l_brace)) {
		reterr;
	}

	struct ffly_vec params;
	ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
	ffly_vec_init(&params, sizeof(struct node*));
	if (!next_token_is(__compiler, _tok_keywd, _r_brace)) {
	_next:
		{
			struct node *param;
			if (_err(err = parser_expr(__compiler, &param))) {
				errmsg("failed to read expression.\n");
				return err;
			}

			struct node **p;
			ffly_vec_push_back(&params, (void**)&p);
			*p = param;

			if (next_token_is(__compiler, _tok_keywd, _comma))
				goto _next;
		}

		if (!expect_token(__compiler, _tok_keywd, _r_brace)) {
			reterr;
		}
	}

	vec_cleanup(__compiler, &params);
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		reterr;
	}

	ast_call(__compiler, __node, no, &params);
	retok;
}

ff_err_t
parser_syput(struct ffly_compiler *__compiler, struct node **__node) {
	sktok(__compiler);
	struct node *nod;
	struct token *name;
	ff_err_t err;
	if (!expect_token(__compiler, _tok_keywd, _l_paren)) {
		reterr;
	}

	if (_err(err = parser_expr(__compiler, &nod))) {
		errmsg("failed to read expression.\n");
		return err;
	}

	ffly_printf("syput: %p\n", nod);

	if (!expect_token(__compiler, _tok_keywd, _comma)) {

	}

	name = next_token(__compiler);

	if (!expect_token(__compiler, _tok_keywd, _r_paren)) {
		reterr;
	}
	if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
		reterr;
	}
	ast_syput(__compiler, __node, nod, name->p);
	ff_token_free(name);
	retok;
}

ff_err_t
ffly_script_parse(struct ffly_compiler *__compiler) {
	ff_err_t err = FFLY_SUCCESS;
	while(!at_eof(&__compiler->lexer)) {
		struct token *tok;
		if (!(tok = peek_token(__compiler))) {
			errmsg("got dead token.\n");
			break;
		}

		struct node *nod = NULL; 
		if (is_type(__compiler, tok) || tok->id == _k_typedef) {
			if (_err(err = parser_decl(__compiler, &nod))) {
				errmsg("failed to read decl.\n");
				break;
			}
		} else if (tok->kind == _tok_keywd) {
			if (tok->id == _k_print) {
				if (_err(err = parser_print_call(__compiler, &nod))) {
					errmsg("failed to read print call.\n");
					break;
				}
			} else if (tok->id == _k_fn) {
				if (_err(err = parser_func_def(__compiler, &nod))) {
					errmsg("failed to read function.\n");
					break;
				}
			} else if (tok->id == _colon) {
				if (_err(err = parser_call(__compiler, &nod))) {
					errmsg("failed to read call.\n");
					break;
				}
			} else if (tok->id == _k_syput) {
				if (_err(err = parser_syput(__compiler, &nod))) {
					errmsg("failed to syput call.\n");
					break;
				}
			} else {
				if (is_stmt(tok)) {
					if (_err(err = parser_stmt(__compiler, &nod))) {
						errmsg("failed to read statment.\n");
						break;
					}
				}
			}
		} else if (tok->kind == _tok_ident) {
			if (_err(err = parser_expr(__compiler, &nod))) {
				errmsg("failed to read expression.\n");
				break;
			}
			if (!expect_token(__compiler, _tok_keywd, _semicolon)) {
				errmsg("expect failure.\n");
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
