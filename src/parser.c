# define __ffly_compiler_internal
# define __ffly_parser
# include "compiler.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "system/nanosleep.h"
# include "data/mem_cpy.h"
# include "system/string.h"
# include "data/str_len.h"
void static
build_node(struct ffly_compiler *__compiler, struct node **__node, struct node *__tmpl) {
	*__node = (struct node*)__ffly_mem_alloc(sizeof(struct node));
	**__node = *__tmpl;
	(*__node)->p = NULL;
	cleanup(__compiler, (void*)*__node);
}

void static
build_type(struct ffly_compiler *__compiler, struct type **__type, struct type *__tmpl) {
	*__type = (struct type*)__ffly_mem_alloc(sizeof(struct type));
	**__type = *__tmpl;
	cleanup(__compiler, (void*)*__type);
}

struct type static *float_t = &(struct type){.kind=_float, .size=sizeof(double)};

struct type static *uint_t = &(struct type){.kind=_uint_t, .size=sizeof(mdl_uint_t)};
struct type static *int_t = &(struct type){.kind=_int_t, .size=sizeof(mdl_int_t)};

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
make_notype(struct ffly_compiler *__compiler, struct type **__type, mdl_u8_t __id) {
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
			return;
	}
	build_type(__compiler, __type, tmpl);
}

void static
ast_decl(struct ffly_compiler *__compiler, struct node **__node, struct node *__var, struct node *__init) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_decl, .var=__var, .init=__init, ._type=NULL});
}

void static
ast_int_type(struct ffly_compiler *__compiler, struct node **__node, struct type *__type, ffly_byte_t *__p) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_literal, ._type=__type});
	ffly_mem_cpy((*__node)->val, __p, __type->size);
}

void static
ast_print_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__arg) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_print_call, .arg=__arg, ._type=NULL});
}

void static
ast_var(struct ffly_compiler *__compiler, struct node **__node, struct type *__type) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_var, ._type=__type});
}

void static
ast_if(struct ffly_compiler *__compiler, struct node **__node, struct node *__cond, struct ffly_vec *__do, struct ffly_vec *__else) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_if, .cond=__cond, ._do=*__do, ._else=*__else, ._type=NULL});
}

void static
ast_func(struct ffly_compiler *__compiler, struct node **__node, struct ffly_vec *__args, struct ffly_vec *__block, struct type *__type, ffly_bool_t __va) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_func, .args=*__args, .block=*__block, ._type=__type, .va=__va});
}

void static
ast_func_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__call, struct ffly_vec *__params) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_func_call, .call=__call, .params=*__params, ._type=__call->_type});
}

void static
ast_struct_ref(struct ffly_compiler *__compiler, struct node **__node, struct node *__struct, struct type *__type){
	build_node(__compiler, __node, &(struct node){.kind=_ast_struct_ref, ._struct=__struct, ._type=__type});
}

void static
ast_assign(struct ffly_compiler *__compiler, struct node **__node, struct node *__l, struct node *__r, struct type *__type) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_assign, .l=__l, .r=__r, ._type=__type});
}

void static
ast_exit(struct ffly_compiler *__compiler, struct node **__node) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_exit});
}

void static
ast_while(struct ffly_compiler *__compiler, struct node **__node, struct node *__cond, struct ffly_vec *__block) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_while, .cond=__cond, .block=*__block, ._type=NULL});
}

void static
ast_uop(struct ffly_compiler *__compiler, struct node **__node, mdl_u8_t __kind, struct node *__operand, struct type *__type) {
	build_node(__compiler, __node, &(struct node){.kind=__kind, .operand=__operand, ._type=__type});
}

void static
ast_match(struct ffly_compiler *__compiler, struct node **__node) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_match});
}

void static
ast_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__no, struct ffly_vec *__params) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_call, .no=__no, .params=*__params});
}

void static
ast_ret(struct ffly_compiler *__compiler, struct node **__node, struct node *__ret, struct type *__type) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_ret, .ret=__ret, ._type=__type});
}

void static
ast_conv(struct ffly_compiler *__compiler, struct node **__node, struct type *__to, struct node *__operand) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_conv, ._type=__to, .operand=__operand});
}

void static
ast_brk(struct ffly_compiler *__compiler, struct node **__node) {
	build_node(__compiler, __node, &(struct node){.kind=_ast_brk});
}

void static
ast_binop(struct ffly_compiler *__compiler, struct node **__node, mdl_u8_t __op, struct type *__type, struct node *__l, struct node *__r) {
	build_node(__compiler, __node, &(struct node){.kind=__op, ._type=__type, .l=__l, .r=__r});
}

void static
mk_ptr_type(struct ffly_compiler *__compiler, struct type **__type, struct type *__ptr) {
	build_type(__compiler, __type, ptr);
	(*__type)->ptr = __ptr;
}

struct type static*
get_typedef(struct ffly_compiler *__compiler, char *__name, ffly_err_t *__err) {
	return (struct type*)ffly_map_get(&__compiler->typedefs, __name, ffly_str_len(__name), __err);
}

ffly_bool_t static
is_type(struct ffly_compiler *__compiler, struct token *__tok) {
	ffly_err_t err;
	if (__tok->kind == TOK_IDENT) { 
		struct type *_type = get_typedef(__compiler, (char*)__tok->p, &err);
		if (_ok(err) && _type != NULL) return 1;
		return 0;
	}

	if (__tok->kind != TOK_KEYWORD) return 0;
	return ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_struct || __tok->id == _k_var || __tok->id == _k_float || __tok->id == _k_typedef);
}
	
ffly_bool_t static
is_stmt(struct token *__tok) {
	return (__tok->id == _k_if || __tok->id == _k_exit || __tok->id == _k_while || __tok->id == _k_match);
}

ffly_err_t static
conv(struct ffly_compiler *__compiler, struct node **__node, struct node *__operand, struct type *__to) {
	ast_conv(__compiler, __node, __to, __operand);
}

ffly_err_t static parser_func_call(struct ffly_compiler*, struct node**, struct node*);
ffly_err_t static parser_decl_spec(struct ffly_compiler*, struct token*, struct type**);
ffly_err_t static parser_struct_decl(struct ffly_compiler*, struct ffly_map*, mdl_uint_t*);
ffly_err_t static parser_struct_spec(struct ffly_compiler*, struct type**);
ffly_err_t static parser_struct_field(struct ffly_compiler*, struct node**, struct node*);
ffly_err_t static parser_declarator(struct ffly_compiler*, struct type**, struct type*);
ffly_err_t static parser_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_decl(struct ffly_compiler*, struct node**);
ffly_err_t static parser_print_call(struct ffly_compiler*, struct node**);
ffly_err_t static parser_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_primary_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_conditional_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_unary_addrof(struct ffly_compiler*, struct node**);
ffly_err_t static parser_unary_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_postfix_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_assign_expr(struct ffly_compiler*, struct node**);
struct type static* parser_cast_type(struct ffly_compiler*, ffly_err_t*);
ffly_err_t static parser_cast_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_binary_expr(struct ffly_compiler*, struct node**);
ffly_err_t static parser_decl_init(struct ffly_compiler*, struct node**);
ffly_err_t static parser_brk_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_call(struct ffly_compiler*, struct node**);
ffly_bool_t static is_func_call(struct ffly_compiler*);
ffly_err_t static parser_compound_stmt(struct ffly_compiler*, struct ffly_vec*);
ffly_err_t static parser_if_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_match_stmt(struct ffly_compiler*, struct node**); 
ffly_err_t static parser_exit_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_ret_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_while_stmt(struct ffly_compiler*, struct node**);
ffly_err_t static parser_func_def(struct ffly_compiler*, struct node**);
ffly_err_t static parser_call(struct ffly_compiler*, struct node**);
/*
mdl_u64_t static
__parser_func_call() {
	struct ffly_compiler *p0;
	struct node **p1;
	struct node *p2;
	__asm__("mov 32(%%rbp), %%rax\n\t"
		"mov %%rax, %0\n\t"
		"mov 24(%%rbp), %%rax\n\t"
		"mov %%rax, %1\n\t"
		"mov 16(%%rbp), %%rax\n\t"
		"mov %%rax, %2" : "=m"(p0), "=m"(p1), "=m"(p2) :);
	return (mdl_u64_t)parser_func_call(p0, p1, p2);
}

mdl_u64_t static
__parser_decl_spec() {
	struct ffly_compiler *p0;
	struct token *p1;
	struct type **p2;
    __asm__("mov 32(%%rbp), %%rax\n\t"
			"mov %%rax, %0\n\t"
			"mov 24(%%rbp), %%rax\n\t"
			"mov %%rax, %1\n\t"
			"mov 16(%%rbp), %%rax\n\t"
			"mov %%rax, %2" : "=m"(p0), "=m"(p1), "=m"(p2) :);
	return (mdl_u64_t)parser_decl_spec(p0, p1, p2);
}

mdl_u64_t static
__parser_struct_decl() {
	struct ffly_compiler *p0;
	struct ffly_map *p1;
	mdl_uint_t *p2;
	__asm__("mov 32(%%rbp), %%rax\n\t"
		"mov %%rax, %0\n\t"
		"mov 24(%%rbp), %%rax\n\t"
		"mov %%rax, %1\n\t"
		"mov 16(%%rbp), %%rax\n\t"
		"mov %%rax, %2" : "=m"(p0), "=m"(p1), "=m"(p2) :);
	return (mdl_u64_t)parser_struct_decl(p0, p1, p2);

}

ffly_err_t
parser_init(struct ffly_compiler *__compiler) {
	set_call(__compiler, __parser_func_call, _parser_func_call);
	set_call(__compiler, __parser_decl_spec, _parser_decl_spec);
}
*/
ffly_err_t parser_struct_decl(struct ffly_compiler *__compiler, struct ffly_map *__fields, mdl_uint_t *__size) {
	if (!expect_token(__compiler, TOK_KEYWORD, _l_brace)) {
		errmsg("expected error.\n");
		reterr;
	}

	ffly_off_t off = 0;
	ffly_err_t err;
	if (!next_token_is(__compiler, TOK_KEYWORD, _r_brace)) {
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
			if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
				reterr;
			}

			if (!next_token_is(__compiler, TOK_KEYWORD, _r_brace)) {
				goto _next;
			}
		}
	}
	retok;
}

ffly_err_t parser_struct_spec(struct ffly_compiler *__compiler, struct type **__type) {
	struct token *name = next_token(__compiler);
	if (name->kind != TOK_IDENT) {
		errmsg("diden't get identifier.\n");
		reterr;
	}

	ffly_err_t err;
	*__type = (struct type*)ffly_map_get(__compiler->local != NULL?__compiler->local:&__compiler->env, name->p, ffly_str_len((char*)name->p), &err);
	if (*__type != NULL) {
		// err
		reterr;
	}

	struct ffly_map fields;    
	ffly_map_init(&fields, _ffly_map_127);
	mdl_uint_t size;
	if (_err(err = parser_struct_decl(__compiler, &fields, &size))) {
		// err
		reterr;
	}

	build_type(__compiler, __type, &(struct type){.kind=_struct, .size=size, .fields=fields});
	ffly_map_put(__compiler->local != NULL?__compiler->local:&__compiler->env, name->p, ffly_str_len((char*)name->p), *__type);
	retok;
}

ffly_err_t parser_struct_field(struct ffly_compiler *__compiler, struct node **__node, struct node *__struct) {
	struct token *name = next_token(__compiler); 
	ffly_err_t err;
	struct type *_type = (struct type*)ffly_map_get(&__struct->_type->fields, name->p, ffly_str_len((char*)name->p), &err);
	if (!_type) {
		errmsg("structure field doesen't exist.\n");
		reterr;
	}
	
	ast_struct_ref(__compiler, __node, __struct, _type);
	retok;
}

ffly_err_t parser_decl_spec(struct ffly_compiler *__compiler, struct token *__tok, struct type **__type) {
	if (__tok->kind == TOK_IDENT) {
		ffly_err_t err;
		struct type *_type = get_typedef(__compiler, (char*)__tok->p, &err);
		if (_ok(err) && _type != NULL) {
			build_type(__compiler, __type, _type);
			retok;
		}
		reterr;
	}

	if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
		make_notype(__compiler, __type, __tok->id);
	} else if (__tok->id == _k_struct) {
		parser_struct_spec(__compiler, __type);
	} else if (__tok->id == _k_void) {
		*__type = NULL;
	} else if (__tok->id == _k_var) {
		build_type(__compiler, __type, &(struct type){.kind=_unknown});
	} else {
		if (!is_type(__compiler, __tok))
			errmsg("token mismatch.\n");
		else
			errmsg("unknown type.\n");
		reterr;
	}
	retok;
}

void read_no(struct ffly_compiler *__compiler, struct node **__node, char *__s, mdl_u8_t __is_hex, mdl_u8_t __is_float) {
	mdl_u8_t sign = *__s == '-';
	struct type *_type;
	ffly_byte_t val[sizeof(mdl_u64_t)];
	if (__is_float) {
		*(double*)val = ffly_stfloat(__s);
		_type = float_t;
	} else {
		mdl_u64_t no;
		if (__is_hex) {
			if (ffly_islen(__s, 2) == -1)
				if (*__s == '0' && ffly_tolow(*(__s+1)) == 'x') __s+=2;
			no = ffly_htint(__s);
		} else
			no = ffly_stno(__s);

		if (sign) no = -no;
		if (no >= 0 && no <= (mdl_u8_t)~0)
			_type = sign?i8_t:u8_t;
		else if (no > (mdl_u8_t)~0 && no <= (mdl_u16_t)~0)
			_type = sign?i16_t:u16_t;
		else if (no > (mdl_u16_t)~0 && no <= (mdl_u32_t)~0)
			_type = sign?i32_t:u32_t;
		else if (no > (mdl_u32_t)~0 && no <= (mdl_u64_t)~0)
			_type = sign?i64_t:u64_t;
		*(mdl_u64_t*)val = sign?-no:no;
	}

	ast_int_type(__compiler, __node, _type, val);
}

ffly_err_t parser_primary_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	ffly_err_t err;
	if (!tok) {
		retok;
	}

	switch(tok->kind) {
		case TOK_IDENT: {
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
		case TOK_NO:
			read_no(__compiler, __node, (char*)tok->p, tok->is_hex, tok->is_float);
		break;
		case TOK_CHR:
			ast_int_type(__compiler, __node, u8_t, tok->p);
		break;
		default:
			*__node = NULL;
			ffly_ulex(__compiler, tok);
	}
	retok;
}

ffly_err_t parser_conditional_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	if (is_keyword(tok, _eqeq) || is_keyword(tok, _neq) || is_keyword(tok, _gt) || is_keyword(tok, _lt)) {
		struct node *l = *__node;
		struct node *r;
		ffly_err_t err;
		if (_err(err = parser_primary_expr(__compiler, &r))) {
			_ret;
		}
		struct type *_type;
		build_type(__compiler, &_type, u8_t);

		mdl_u8_t op;
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
		retok;
	}

	ffly_ulex(__compiler, tok);
	retok;
}

ffly_err_t parser_unary_addrof(struct ffly_compiler *__compiler, struct node **__node) {
	struct node *_node;
	ffly_err_t err;
	if (_err(err = parser_expr(__compiler, &_node)))
		_ret;
	
	struct type *_type;
	build_type(__compiler, &_type, u64_t);
	ast_uop(__compiler, __node, _ast_addrof, _node, _type);
	return FFLY_SUCCESS;
}

ffly_err_t parser_unary_expr(struct ffly_compiler *__compiler, struct node **__node) {
	if (next_token_is(__compiler, TOK_KEYWORD, _incr))
		ast_uop(__compiler, __node, _ast_incr, *__node, (*__node)->_type);
	else if (next_token_is(__compiler, TOK_KEYWORD, _decr))
		ast_uop(__compiler, __node, _ast_decr, *__node, (*__node)->_type);
	else {
		struct token *tok = next_token(__compiler);
		switch(tok->id) {
			case _ampersand: parser_unary_addrof(__compiler, __node); goto _end;
		}

		ffly_ulex(__compiler, tok);
	}

	_end:
	retok;
}

ffly_err_t parser_postfix_expr(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
	if (next_token_is(__compiler, TOK_KEYWORD, _period)) {
		if (_err(err = parser_struct_field(__compiler, __node, *__node))) {
			errmsg("failed to read struct field.\n");
			_ret;
		}
		retok;
	}

	struct token *tok = peek_token(__compiler);
	if (tok->kind == TOK_KEYWORD && tok->id == _l_paren) {
		if (_err(err = parser_func_call(__compiler, __node, *__node))) {
			errmsg("failed to read function call.\n");
			_ret;
		}	 
	}
	retok;
}

ffly_err_t parser_assign_expr(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
	if (next_token_is(__compiler, TOK_KEYWORD, _eq)) {
		struct node *l = *__node;
		struct node *r = NULL;
		if (_err(err = parser_expr(__compiler, &r))) {
			errmsg("failed to read expression.\n");
			_ret;
		}
		ast_assign(__compiler, __node, l, r, l->_type);  
	}
	retok;
}

struct type* parser_cast_type(struct ffly_compiler *__compiler, ffly_err_t *__err) {
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

	if (_err(*__err = parser_declarator(__compiler, &_type, _type))) {
		errmsg("failed to read declarator.\n");
		return NULL;
	} 
	return _type;
}

ffly_err_t parser_cast_expr(struct ffly_compiler *__compiler, struct node **__node) {
	struct token *tok = next_token(__compiler);
	ffly_err_t err;
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

		if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
			reterr;
		}

		struct node *n;
		if (_err(err = parser_expr(__compiler, &n))) {
			errmsg("failed to read expression.\n");
			_ret;
		}
		ast_uop(__compiler, __node, _op_cast, n, _type);			 
		retok; 
	}

	ffly_ulex(__compiler, tok);
	retok;
}

ffly_err_t parser_binary_expr(struct ffly_compiler *__compiler, struct node **__node) {
/*
	struct token *tok = next_token(__compiler);
	mdl_u8_t op;
	if (is_keyword(tok, _astrisk)) {
		op = _opr_mul;
	} else if (is_keyword(tok, _plus)) {
		op = _opr_add;
	} else if (is_keyword(tok, _minus)) {
		op = _opr_sub;
	} else {
		ffly_ulex(__compiler, tok);
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

ffly_err_t parser_expr(struct ffly_compiler *__compiler, struct node **__node) {
	*__node = NULL;
	ffly_err_t err;
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

ffly_err_t parser_decl_init(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
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

ffly_err_t parser_brk_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
		reterr;
	}

	ast_brk(__compiler, __node);
	retok;
}

ffly_err_t parser_compound_stmt(struct ffly_compiler *__compiler, struct ffly_vec *__vec) {
	if (!expect_token(__compiler, TOK_KEYWORD, _l_brace)) {
		reterr;
	}

	ffly_err_t err = FFLY_SUCCESS;
	struct token *tok = peek_token(__compiler);
	while(!is_keyword(tok, _r_brace)) {
		struct node *_node = NULL;
		if (is_type(__compiler, tok)) {
			if (_err(err = parser_decl(__compiler, &_node))) {
				errmsg("failed to read decl.\n");
				break;
			}
		} else if (tok->kind == TOK_KEYWORD) {
			if (tok->id == _k_print) {
				if (_err(err = parser_print_call(__compiler, &_node))) {
					errmsg("failed to read print call.\n");
					break;
				}
			} else if (tok->id == _colon) {
				if (_err(err = parser_call(__compiler, &_node))) {
					errmsg("failed to read call.\n");
					break;
				}
			} else if (tok->id >= _k_uint_t && tok->id <= _k_i8_t) {
				if (_err(err = parser_decl(__compiler, &_node))) {
					errmsg("failed to read decl.\n");
					break;
				}
			} else {
				if (is_stmt(tok) || tok->id == _k_ret || tok->id == _k_brk) {
					if (_err(err = parser_stmt(__compiler, &_node))) {
						errmsg("failed to read statment.\n");
						break;
					}
				}
			}
		} else if (tok->kind == TOK_IDENT) {
			if (_err(err = parser_expr(__compiler, &_node))) {
				errmsg("failed to read expression.\n");
				break;
			}

			if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
				errmsg("expect error.\n");
				break;
			}
		}

		struct node **p;
		ffly_vec_push_back(__vec, (void**)&p);
		*p = _node;

		tok = peek_token(__compiler);
	}
	if (_err(err)) _ret;
	skip_token(__compiler);
	retok;
}

ffly_err_t parser_declarator(struct ffly_compiler *__compiler, struct type **__type, struct type *__base_type) {
	if (next_token_is(__compiler, TOK_KEYWORD, _astrisk)) {
		mk_ptr_type(__compiler, __type, __base_type);
		parser_declarator(__compiler, __type, *__type);
	} else
		*__type = __base_type;
	retok;
}

ffly_mapp static
get_env(struct ffly_compiler *__compiler) {
	return __compiler->local != NULL?__compiler->local:&__compiler->env;
}

ffly_err_t parser_decl(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
	struct type *_type;
	struct node *init = NULL;
	mdl_u8_t is_typedef = next_token_is(__compiler, TOK_KEYWORD, _k_typedef);
	struct token *tok = next_token(__compiler);
	if (tok->kind != TOK_KEYWORD && tok->kind != TOK_IDENT) {
		errmsg("%u;%u: wanted keyword got %s.\n", tokl(tok), tokcol(tok), tokk_str(tok->kind));
		reterr;
	}
	if (_err(err = parser_decl_spec(__compiler, tok, &_type))) {
		errmsg("failed to read declaration specifier.\n");
		_ret;
	} 
	if (_err(err = parser_declarator(__compiler, &_type, _type))) {
		errmsg("failed to read declarator.\n");
		_ret;
	}
	if (next_token_is(__compiler, TOK_KEYWORD, _semicolon)) {
		retok;
	}

	struct token *name = next_token(__compiler);
	if (name->kind != TOK_IDENT) {
		errmsg("%u;%u: wanted identifier got %s\n", tokl(name), tokcol(name), tokk_str(name->kind));
		reterr;
	}

	if (is_typedef)
		ffly_map_put(&__compiler->typedefs, name->p, ffly_str_len((char*)name->p), _type);
	else {
		struct node *var;
		ast_var(__compiler, &var, _type);

		ffly_map_put(get_env(__compiler), name->p, ffly_str_len((char*)name->p), var);
		if (next_token_is(__compiler, TOK_KEYWORD, _eq)) {
			if (_err(err = parser_decl_init(__compiler, &init))) {
				errmsg("failed to read decl init.\n");
				_ret;
			}

			if (_type->kind == _unknown) {
				_type->kind = init->_type->kind;
				_type->size = init->_type->size;
			}
		}

		if (init != NULL) {
			if (init->_type->kind != var->_type->kind)
			   conv(__compiler, &init, init, var->_type);
		}

		ast_decl(__compiler, __node, var, init);
	}
	if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}
	retok;
}

ffly_err_t parser_print_call(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ffly_err_t err;
	struct node *arg;
	if (_err(err = parser_expr(__compiler, &arg))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	ast_print_call(__compiler, __node, arg);
	if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
		errmsg("expect error.\n");
		reterr;
	}
	if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}
	retok;
}

ffly_err_t parser_if_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
	skip_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct node *cond;
	if (_err(err = parser_expr(__compiler, &cond))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
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
	if (next_token_is(__compiler, TOK_KEYWORD, _k_else)) {
		if (_err(err = parser_compound_stmt(__compiler, &_else))) {
			errmsg("failed to read compound statment.\n");
			_ret;
		}
	}

	vec_cleanup(__compiler, &_else);
	ast_if(__compiler, __node, cond, &_do, &_else);
	return FFLY_SUCCESS;
}

ffly_err_t parser_match_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct node *expr;
	ffly_err_t err;
	if (_err(err = parser_expr(__compiler, &expr))) {
		errmsg("failed to read expression.\n");
		_ret;
	}

	if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	if (!expect_token(__compiler, TOK_KEYWORD, _l_brace)) {
		errmsg("expect error.\n");
		reterr;
	}

	if (!next_token_is(__compiler, TOK_KEYWORD, _r_brace)) {
		_next:
		{
			struct node *val;
			if (_err(err = parser_expr(__compiler, &val))) {
				errmsg("failed to read expression.\n");
				_ret;
			}

			if (!expect_token(__compiler, TOK_KEYWORD, _colon)) {
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
			if (!next_token_is(__compiler, TOK_KEYWORD, _r_brace)) {
				goto _next;
			}
		}
	}
	retok;
}

ffly_err_t parser_exit_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
		errmsg("expect error.\n");
		reterr;
	}

	ast_exit(__compiler, __node);
	retok;
}

ffly_err_t parser_ret_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);

	ffly_err_t err;
	struct node *ret = NULL;
	if (!next_token_is(__compiler, TOK_KEYWORD, _semicolon)) {
		if (_err(err = parser_expr(__compiler, &ret))) {
			errmsg("failed to read expression.\n");
			_ret;
		}

		if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
			errmsg("expect error.\n");
			reterr;
		}
	}
	ast_ret(__compiler, __node, ret, __compiler->ret_type);
	retok;
}

ffly_err_t parser_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	ffly_err_t err;
	struct token *tok = peek_token(__compiler);
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

ffly_err_t parser_func_call(struct ffly_compiler *__compiler, struct node **__node, struct node *__call) {
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ffly_err_t err;
	struct ffly_vec params;
	ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
	ffly_vec_init(&params, sizeof(struct node*));

	struct node **arg = (struct node**)ffly_vec_begin(&__call->args);
	if (!next_token_is(__compiler, TOK_KEYWORD, _r_paren)) {
		_next:
		{
			struct node *param;
			if (_err(err = parser_expr(__compiler, &param))) {
				errmsg("failed to read expression.\n");
				return err;
			}	 
	
			if (param->_type->kind != (*arg)->var->_type->kind)
				conv(__compiler, &param, param, (*arg)->var->_type);
 
			struct node **p;
			ffly_vec_push_back(&params, (void**)&p);
			*p = param;

			if (next_token_is(__compiler, TOK_KEYWORD, _comma)) {
				arg++;
				goto _next;
			}
		}

		if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
			return FFLY_FAILURE;
		}

		if (arg != (struct node**)ffly_vec_end(&__call->args)) {
			errmsg("to few params.\n");
			reterr;
		}	 
	}
 
	vec_cleanup(__compiler, &params);
	ast_func_call(__compiler, __node, __call, &params);
	return FFLY_SUCCESS;
}

ffly_err_t parser_func_def(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	ffly_err_t err;
	struct token *name = next_token(__compiler);
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		errmsg("expect error.\n");
		reterr;
	}

	ffly_bool_t va = 0;
	struct ffly_map local, *save = NULL;
	ffly_map_init(&local, _ffly_map_127);
	save = __compiler->local;
	__compiler->local = &local;

	struct ffly_vec args;
	ffly_vec_set_flags(&args, VEC_AUTO_RESIZE);
	ffly_vec_init(&args, sizeof(struct node*)); 
	if (next_token_is(__compiler, TOK_KEYWORD, _ellipsis)) {
		va = 1; 
	}

	if (!next_token_is(__compiler, TOK_KEYWORD, _r_paren)) {
		_next:
		if (next_token_is(__compiler, TOK_KEYWORD, _ellipsis)) {
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

			if (_err(err = parser_declarator(__compiler, &_type, _type))) {
				errmsg("failed to read declarator.\n");
				return err;
			}

			struct token *name = next_token(__compiler);
			ast_var(__compiler, &var, _type);

			ast_decl(__compiler, &decl, var, NULL);
		 
			struct node **p;
			ffly_vec_push_back(&args, (void**)&p);
			*p = decl;

			ffly_map_put(&local, name->p, ffly_str_len((char*)name->p), var);
			if (next_token_is(__compiler, TOK_KEYWORD, _comma)) {  
				goto _next;
			}
		}

		_sk:
		if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
			errmsg("expect error.\n");
			reterr;
		}
	}

	vec_cleanup(__compiler, &args);
	if (!expect_token(__compiler, TOK_KEYWORD, _r_arrow)) {
		errmsg("expect error.\n");
		reterr;
	}

	struct token *tok = next_token(__compiler);
	struct type *ret_type;
	if (_err(err = parser_decl_spec(__compiler, tok, &ret_type))) {
		errmsg("failed to read declaration specifier.\n");
		return err;
	}

	struct ffly_vec block;
	ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
	ffly_vec_init(&block, sizeof(struct node*));

	__compiler->ret_type = ret_type;
	parser_compound_stmt(__compiler, &block); 
	__compiler->local = save;
	ffly_map_de_init(&local);
   
	vec_cleanup(__compiler, &block);
	ast_func(__compiler, __node, &args, &block, ret_type, va);
	ffly_map_put(__compiler->local != NULL?__compiler->local:&__compiler->env, name->p, ffly_str_len((char*)name->p), *__node);
	return FFLY_SUCCESS;
}


ffly_bool_t is_func_call(struct ffly_compiler *__compiler) {
	struct token *name = next_token(__compiler);
	struct token *l_paren = next_token(__compiler);
	if (!name || !l_paren) return 0;
	mdl_u8_t res = !(name->kind != TOK_IDENT || !is_keyword(l_paren, _l_paren));
	ffly_ulex(__compiler, l_paren);
	ffly_ulex(__compiler, name);
	return res;
}

ffly_err_t parser_while_stmt(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);
	ffly_err_t err;    
	if (!expect_token(__compiler, TOK_KEYWORD, _l_paren)) {
		reterr;
	}

	struct node *cond;
	if (_err(err = parser_expr(__compiler, &cond))) {
		errmsg("failed to read expression.\n");
		return err;
	}

	if (!expect_token(__compiler, TOK_KEYWORD, _r_paren)) {
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

ffly_err_t parser_call(struct ffly_compiler *__compiler, struct node **__node) {
	skip_token(__compiler);

	ffly_err_t err;
	struct node *no;
	if (_err(err = parser_expr(__compiler, &no))) {
		errmsg("failed to read expression.\n");
		return err;
	}	 

	if (!expect_token(__compiler, TOK_KEYWORD, _l_arrow)) {
		reterr;
	}

	if (!expect_token(__compiler, TOK_KEYWORD, _l_brace)) {
		reterr;
	}

	struct ffly_vec params;
	ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
	ffly_vec_init(&params, sizeof(struct node*));
	if (!next_token_is(__compiler, TOK_KEYWORD, _r_brace)) {
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

			if (next_token_is(__compiler, TOK_KEYWORD, _comma)) {
				goto _next;
			}
		}

		if (!expect_token(__compiler, TOK_KEYWORD, _r_brace)) {
			reterr;
		}
	}

	vec_cleanup(__compiler, &params);
	if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
		reterr;
	}

	ast_call(__compiler, __node, no, &params);
	retok;
}

ffly_err_t
ffly_parse(struct ffly_compiler *__compiler) {
	ffly_err_t err = FFLY_SUCCESS;
	while((__compiler->file->p+__compiler->file->off) < __compiler->file->end) {
		struct token *tok = peek_token(__compiler);
		struct node *_node = NULL;
		if (!tok) break; 
		if (is_type(__compiler, tok)) {
			if (_err(err = parser_decl(__compiler, &_node))) {
				errmsg("failed to read decl.\n");
				break;
			}
		} else if (tok->kind == TOK_KEYWORD) {
			if (tok->id == _k_print) {
				if (_err(err = parser_print_call(__compiler, &_node))) {
					errmsg("failed to read print call.\n");
					break;
				}
			} else if (tok->id == _k_fn) {
				if (_err(err = parser_func_def(__compiler, &_node))) {
					errmsg("failed to read function.\n");
					break;
				}
			} else if (tok->id == _colon) {
				if (_err(err = parser_call(__compiler, &_node))) {
					errmsg("failed to read call.\n");
					break;
				}
			} else {
				if (is_stmt(tok)) {
					if (_err(err = parser_stmt(__compiler, &_node))) {
						errmsg("failed to read statment.\n");
						break;
					}
				}
			}
		} else if (tok->kind == TOK_IDENT) {
			if (_err(err = parser_expr(__compiler, &_node))) {
				errmsg("failed to read expression.\n");
				break;
			}
			if (!expect_token(__compiler, TOK_KEYWORD, _semicolon)) {
				errmsg("expect failure.\n");
			}
		}

		if (_node != NULL) {
			struct node **p;
			ffly_vec_push_back(&__compiler->nodes, (void**)&p);
			*p = _node;
		}
	}
	if (_err(err))
		return err;
	return FFLY_SUCCESS;
}
