# define __ffly_script_internal
# include "script.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "system/nanosleep.h"
# include "data/mem_cpy.h"
# include "system/string.h"
# include "data/str_len.h"
void build_node(struct ffly_script *__script, struct node **__node, struct node *__tmpl) {
	*__node = (struct node*)__ffly_mem_alloc(sizeof(struct node));
	**__node = *__tmpl;
    (*__node)->p = NULL;
    cleanup(__script, (void*)*__node);
}

void build_type(struct ffly_script *__script, struct type **__type, struct type *__tmpl) {
	*__type = (struct type*)__ffly_mem_alloc(sizeof(struct type));
	**__type = *__tmpl;
    cleanup(__script, (void*)*__type);
}

struct type *float_t = &(struct type){.kind=_float, .size=sizeof(double)};

struct type *uint_t = &(struct type){.kind=_uint_t, .size=sizeof(mdl_uint_t)};
struct type *int_t = &(struct type){.kind=_int_t, .size=sizeof(mdl_int_t)};

struct type *u64_t = &(struct type){.kind=_u64_t, .size=8};
struct type *i64_t = &(struct type){.kind=_i64_t, .size=8};

struct type *u32_t = &(struct type){.kind=_u32_t, .size=4};
struct type *i32_t = &(struct type){.kind=_i32_t, .size=4};

struct type *u16_t = &(struct type){.kind=_u16_t, .size=2};
struct type *i16_t = &(struct type){.kind=_i16_t, .size=2};

struct type *u8_t = &(struct type){.kind=_u8_t, .size=1};
struct type *i8_t = &(struct type){.kind=_i8_t, .size=1};

struct type *ptr = &(struct type){.kind=_u64_t, .size=8};
void make_notype(struct ffly_script *__script, struct type **__type, mdl_u8_t __id) {
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
	build_type(__script, __type, tmpl);
}

void ast_decl(struct ffly_script *__script, struct node **__node, struct node *__var, struct node *__init) {
	build_node(__script, __node, &(struct node){.kind=_ast_decl, .var=__var, .init=__init, ._type=NULL});
}

void ast_int_type(struct ffly_script *__script, struct node **__node, struct type *__type, ffly_byte_t *__p) {
	build_node(__script, __node, &(struct node){.kind=_ast_literal, ._type=__type});
	ffly_mem_cpy((*__node)->val, __p, __type->size);
}

void ast_print_call(struct ffly_script *__script, struct node **__node, struct node *__arg) {
	build_node(__script, __node, &(struct node){.kind=_ast_print_call, .arg=__arg, ._type=NULL});
}

void ast_var(struct ffly_script *__script, struct node **__node, struct type *__type) {
    build_node(__script, __node, &(struct node){.kind=_ast_var, ._type=__type});
}

void ast_binop(struct ffly_script *__script, struct node **__node, mdl_u8_t __kind, struct type *__type, struct node *__l, struct node *__r) {
    build_node(__script, __node, &(struct node){.kind=__kind, ._type=__type, .l=__l, .r=__r});
}

void ast_if(struct ffly_script *__script, struct node **__node, struct node *__cond, struct ffly_vec *__do, struct ffly_vec *__else) {
    build_node(__script, __node, &(struct node){.kind=_ast_if, .cond=__cond, ._do=*__do, ._else=*__else, ._type=NULL});
}

void ast_func(struct ffly_script *__script, struct node **__node, struct ffly_vec *__args, struct ffly_vec *__block, struct type *__type) {
    build_node(__script, __node, &(struct node){.kind=_ast_func, .args=*__args, .block=*__block, ._type=__type});
}

void ast_func_call(struct ffly_script *__script, struct node **__node, struct node *__call, struct ffly_vec *__params) {
    build_node(__script, __node, &(struct node){.kind=_ast_func_call, .call=__call, .params=*__params, ._type=__call->_type});
}

void ast_struct_ref(struct ffly_script *__script, struct node **__node, struct node *__struct, struct type *__type){
    build_node(__script, __node, &(struct node){.kind=_ast_struct_ref, ._struct=__struct, ._type=__type});
}

void ast_assign(struct ffly_script *__script, struct node **__node, struct node *__l, struct node *__r, struct type *__type) {
    build_node(__script, __node, &(struct node){.kind=_ast_assign, .l=__l, .r=__r, ._type=__type});
}

void ast_exit(struct ffly_script *__script, struct node **__node) {
    build_node(__script, __node, &(struct node){.kind=_ast_exit});
}

void ast_while(struct ffly_script *__script, struct node **__node, struct node *__cond, struct ffly_vec *__block) {
    build_node(__script, __node, &(struct node){.kind=_ast_while, .cond=__cond, .block=*__block, ._type=NULL});
}

void ast_uop(struct ffly_script *__script, struct node **__node, mdl_u8_t __kind, struct node *__operand, struct type *__type) {
    build_node(__script, __node, &(struct node){.kind=__kind, .operand=__operand, ._type=__type});
}

void ast_match(struct ffly_script *__script, struct node **__node) {
    build_node(__script, __node, &(struct node){.kind=_ast_match});
}

void ast_call(struct ffly_script *__script, struct node **__node, struct node *__no, struct ffly_vec *__params) {
    build_node(__script, __node, &(struct node){.kind=_ast_call, .no=__no, .params=*__params});
}

void ast_ret(struct ffly_script *__script, struct node **__node, struct node *__ret, struct type *__type) {
    build_node(__script, __node, &(struct node){.kind=_ast_ret, .ret=__ret, ._type=__type});
}

void mk_ptr_type(struct ffly_script *__script, struct type **__type, struct type *__ptr) {
    build_type(__script, __type, ptr);
    (*__type)->ptr = __ptr;
}

struct type* get_typedef(struct ffly_script *__script, char *__name, ffly_err_t *__err) {
    return (struct type*)ffly_map_get(&__script->typedefs, __name, ffly_str_len(__name), __err);
}

ffly_bool_t is_type(struct ffly_script *__script, struct token *__tok) {
    ffly_err_t err;
    if (__tok->kind == TOK_IDENT) { 
        struct type *_type = get_typedef(__script, (char*)__tok->p, &err);
        if (_ok(err) && _type != NULL) return 1;
        return 0;
    }

    if (__tok->kind != TOK_KEYWORD) return 0;
    return ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_struct || __tok->id == _k_var || __tok->id == _k_float || __tok->id == _k_typedef);
}

ffly_err_t read_func_call(struct ffly_script*, struct node**, struct node*);
ffly_err_t read_decl_spec(struct ffly_script*, struct token*, struct type**);
ffly_err_t read_struct_decl(struct ffly_script *__script, struct ffly_map *__fields, mdl_uint_t *__size) {
    if (!expect_token(__script, TOK_KEYWORD, _l_brace)) {
        return FFLY_FAILURE;
    }

    ffly_off_t off = 0;
    ffly_err_t err;
    if (!next_token_is(__script, TOK_KEYWORD, _r_brace)) {
        _next:
        {
            struct token *tok = next_token(__script);
            struct token *name = next_token(__script);
            struct type *_type;
            if (_err(err = read_decl_spec(__script, tok, &_type))) {
                return FFLY_FAILURE;
            }

            (*__size)++;
            _type->off = off++;
            ffly_map_put(__fields, name->p, ffly_str_len((char*)name->p), _type);
            if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
                return FFLY_FAILURE;
            }

            if (!next_token_is(__script, TOK_KEYWORD, _r_brace)) {
                goto _next;
            }
        }
    }

    return FFLY_SUCCESS;
}

ffly_err_t read_struct_spec(struct ffly_script *__script, struct type **__type) {
    struct token *name = next_token(__script);
    if (name->kind != TOK_IDENT) {
        return FFLY_FAILURE;
    }

    ffly_err_t err;
    *__type = (struct type*)ffly_map_get(__script->local != NULL?__script->local:&__script->env, name->p, ffly_str_len((char*)name->p), &err);
    if (*__type != NULL) return FFLY_SUCCESS;

    struct ffly_map fields;    
    ffly_map_init(&fields);
    mdl_uint_t size;
    if (_err(err = read_struct_decl(__script, &fields, &size))) {
        return FFLY_FAILURE;
    }

    build_type(__script, __type, &(struct type){.kind=_struct, .size=size, .fields=fields});
    ffly_map_put(__script->local != NULL?__script->local:&__script->env, name->p, ffly_str_len((char*)name->p), *__type);
    return FFLY_SUCCESS;
}

ffly_err_t read_struct_field(struct ffly_script *__script, struct node **__node, struct node *__struct) {
    struct token *name = next_token(__script); 
    ffly_err_t err;
    struct type *_type = (struct type*)ffly_map_get(&__struct->_type->fields, name->p, ffly_str_len((char*)name->p), &err);
    if (!_type) {
        ffly_fprintf(ffly_out, "structure field doesen't exist.\n");
        return FFLY_FAILURE;
    }
    
    ast_struct_ref(__script, __node, __struct, _type);
    return FFLY_SUCCESS;
}

ffly_err_t read_decl_spec(struct ffly_script *__script, struct token *__tok, struct type **__type) {
    if (__tok->kind == TOK_IDENT) {
        ffly_err_t err;
        struct type *_type = get_typedef(__script, (char*)__tok->p, &err);
        if (_ok(err) && _type != NULL) {
            build_type(__script, __type, _type);
            return FFLY_SUCCESS;
        }
        return FFLY_FAILURE;
    }

	if ((__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) || __tok->id == _k_float) {
		make_notype(__script, __type, __tok->id);
	} else if (__tok->id == _k_struct) {
        read_struct_spec(__script, __type);
    } else if (__tok->id == _k_void) {
        *__type = NULL;
    } else if (__tok->id == _k_var) {
        build_type(__script, __type, &(struct type){.kind=_unknown});
    } else
        return FFLY_FAILURE;
    return FFLY_SUCCESS;
}

ffly_err_t read_stmt(struct ffly_script*, struct node**);
ffly_err_t read_decl(struct ffly_script*, struct node**);
ffly_err_t read_print_call(struct ffly_script*, struct node**);
void read_no(struct ffly_script *__script, struct node **__node, char *__s, mdl_u8_t __is_hex, mdl_u8_t __is_float) {
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

	ast_int_type(__script, __node, _type, val);
}

ffly_err_t read_primary_expr(struct ffly_script *__script, struct node **__node) {
	struct token *tok = next_token(__script);
    ffly_err_t err;
	if (!tok) return FFLY_SUCCESS;
	switch(tok->kind) {
        case TOK_IDENT: {
            if (__script->local != NULL) {
                *__node = (struct node*)ffly_map_get(__script->local, tok->p, ffly_str_len((char*)tok->p), &err);
                if (*__node != NULL) break;   
            }

            *__node = (struct node*)ffly_map_get(&__script->env, tok->p, ffly_str_len((char*)tok->p), &err);
            if (!*__node) {
                ffly_fprintf(ffly_out, "%u;%u: '%s' could not find in environment.\n", tokl(tok), tokcol(tok), (char*)tok->p);
                return FFLY_FAILURE;
            }
            break;
        }
		case TOK_NO:
			read_no(__script, __node, (char*)tok->p, tok->is_hex, tok->is_float);
		break;
        case TOK_CHR:
            ast_int_type(__script, __node, u8_t, tok->p);
        break;
        default:
            *__node = NULL;
            ffly_script_ulex(__script, tok);
	}
    return FFLY_SUCCESS;
}

ffly_err_t read_conditional_expr(struct ffly_script *__script, struct node **__node) {
    struct token *tok = next_token(__script);
    if (is_keyword(tok, _eqeq) || is_keyword(tok, _neq) || is_keyword(tok, _gt) || is_keyword(tok, _lt)) {
        struct node *l = *__node;
        struct node *r;
        ffly_err_t err;
        if (_err(err = read_primary_expr(__script, &r)))
            return err;
        struct type *_type;
        build_type(__script, &_type, u8_t);

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
            return FFLY_FAILURE;
        ast_binop(__script, __node, op, _type, l, r);
        return FFLY_SUCCESS;
    }

    ffly_script_ulex(__script, tok);
    return FFLY_SUCCESS;
}

ffly_err_t read_expr(struct ffly_script*, struct node**);
ffly_err_t read_unary_addrof(struct ffly_script *__script, struct node **__node) {
    struct node *_node;
    ffly_err_t err;
    if (_err(err = read_expr(__script, &_node)))
        return err;

    
    struct type *_type;
    build_type(__script, &_type, u64_t);
    ast_uop(__script, __node, _ast_addrof, _node, _type);
    return FFLY_SUCCESS;
}

ffly_err_t read_unary_expr(struct ffly_script *__script, struct node **__node) {
    if (next_token_is(__script, TOK_KEYWORD, _incr))
        ast_uop(__script, __node, _ast_incr, *__node, (*__node)->_type);
    else if (next_token_is(__script, TOK_KEYWORD, _decr))
        ast_uop(__script, __node, _ast_decr, *__node, (*__node)->_type);
    else {
        struct token *tok = next_token(__script);
        switch(tok->id) {
            case _ampersand: read_unary_addrof(__script, __node); goto _end;
        }

        ffly_script_ulex(__script, tok);
        return FFLY_SUCCESS;
    }

    _end:
    return FFLY_SUCCESS;
}

ffly_err_t read_postfix_expr(struct ffly_script *__script, struct node **__node) {
    ffly_err_t err;
    if (next_token_is(__script, TOK_KEYWORD, _period)) {
        if (_err(err = read_struct_field(__script, __node, *__node))) {
            return FFLY_FAILURE;
        }
    }

    struct token *tok = peek_token(__script);
    if (tok->kind == TOK_KEYWORD && tok->id == _l_paren) {
        if (_err(err = read_func_call(__script, __node, *__node))) {
            return FFLY_FAILURE;
        }    
    }    

    return FFLY_SUCCESS;
}

ffly_err_t read_assign_expr(struct ffly_script *__script, struct node **__node) {
    if (next_token_is(__script, TOK_KEYWORD, _eq)) {
        struct node *l = *__node;
        struct node *r = NULL;
        read_expr(__script, &r);
        ast_assign(__script, __node, l, r, l->_type);
        if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
            return FFLY_FAILURE;
        }   
    }
    return FFLY_SUCCESS;
}

ffly_err_t read_expr(struct ffly_script *__script, struct node **__node) {
    *__node = NULL;
    ffly_err_t err;
    if (_err(err = read_primary_expr(__script, __node)))
        return err;
    if (_err(err = read_postfix_expr(__script, __node)))
        return err;
    if (_err(err = read_unary_expr(__script, __node)))
        return err;
    if (_err(err = read_conditional_expr(__script, __node)))
        return err;
	if (_err(err = read_assign_expr(__script, __node)))
        return err;
    return FFLY_SUCCESS;
}

ffly_err_t read_decl_init(struct ffly_script *__script, struct node **__node) {
    ffly_err_t err;
	if (_err(err = read_expr(__script, __node))) {
        ffly_fprintf(ffly_out, "failed to read expression.\n");
        return err;
    }
    if (!*__node) {
        ffly_fprintf(ffly_out, "tryed to read expression but diden't find anything.\n");
        return FFLY_FAILURE;
    }

    return FFLY_SUCCESS;
}

ffly_err_t read_call(struct ffly_script*, struct node**);
ffly_bool_t is_func_call(struct ffly_script*);
ffly_err_t read_compound_stmt(struct ffly_script *__script, struct ffly_vec *__vec) {
    if (!expect_token(__script, TOK_KEYWORD, _l_brace)) {
        return FFLY_FAILURE;
    }

    ffly_err_t err = FFLY_SUCCESS;
    struct token *tok = peek_token(__script);
    while(!is_keyword(tok, _r_brace)) {
        struct node *_node = NULL;
        if (is_type(__script, tok)) {

        } else if (tok->kind == TOK_KEYWORD) {
            if (tok->id == _k_print) {
                if (_err(err = read_print_call(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read print call.\n");
                    break;
                }
            } else if (tok->id == _colon) {
                if (_err(err = read_call(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read call.\n");
                    break;
                }
            } else if (tok->id >= _k_uint_t && tok->id <= _k_i8_t) {
                if (_err(err = read_decl(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to decl.\n");
                    break;
                }
            } else {
                if (tok->id == _k_if || tok->id == _k_while || tok->id == _k_ret) {
                    if (_err(err = read_stmt(__script, &_node))) {
                        ffly_fprintf(ffly_out, "failed to read statment.\n");
                        break;
                    }
                }
            }
        } else if (tok->kind == TOK_IDENT) {
            if (_err(err = read_stmt(__script, &_node))) {
                ffly_fprintf(ffly_out, "failed to read statment.\n");
                break;
            }

            if (!_node) {
                if (_err(err = read_expr(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read expression.\n");
                    break;
                }

                expect_token(__script, TOK_KEYWORD, _semicolon);
            }
        }

        struct node **p;
        ffly_vec_push_back(__vec, (void**)&p);
        *p = _node;

        tok = peek_token(__script);
    }
    if (_err(err))
        return err;
    skip_token(__script);
    return FFLY_SUCCESS;
}

ffly_err_t read_declarator(struct ffly_script *__script, struct type **__type, struct type *__base_type) {
    if (next_token_is(__script, TOK_KEYWORD, _astrisk)) {
        mk_ptr_type(__script, __type, __base_type);
        read_declarator(__script, __type, *__type);
    } else
        *__type = __base_type;
    return FFLY_SUCCESS;
}

ffly_err_t read_decl(struct ffly_script *__script, struct node **__node) {
    ffly_err_t err;
	struct type *_type;
    struct node *init = NULL;
    mdl_u8_t is_typedef = next_token_is(__script, TOK_KEYWORD, _k_typedef);
	struct token *tok = next_token(__script);
    if (tok->kind != TOK_KEYWORD && tok->kind != TOK_IDENT) {
        ffly_fprintf(ffly_out, "%u;%u: wanted keyword got %s.\n", tokl(tok), tokcol(tok), tokk_str(tok->kind));
    }
	if (_err(err = read_decl_spec(__script, tok, &_type)))
        return err;
    if (_err(err = read_declarator(__script, &_type, _type)))
        return err;
    if (next_token_is(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_SUCCESS;
    }

	struct token *name = next_token(__script);
	if (name->kind != TOK_IDENT) {
		ffly_fprintf(ffly_out, "%u;%u: wanted ident got %s\n", tokl(name), tokcol(name), tokk_str(name->kind));
        return FFLY_FAILURE;
	}

    if (is_typedef)
        ffly_map_put(&__script->typedefs, name->p, ffly_str_len((char*)name->p), _type);
    else {
        struct node *var;
        ast_var(__script, &var, _type);

        ffly_map_put(__script->local != NULL?__script->local:&__script->env, name->p, ffly_str_len((char*)name->p), var);

	    if (next_token_is(__script, TOK_KEYWORD, _eq)) {
		    if (_err(err = read_decl_init(__script, &init)))
                return err;
            if (_type->kind == _unknown) {
                _type->kind = init->_type->kind;
                _type->size = init->_type->size;
            }
        }
	    ast_decl(__script, __node, var, init);
    }
    if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_err_t read_print_call(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);
	if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        return FFLY_FAILURE;
    }
	struct node *arg;
	read_expr(__script, &arg);
	ast_print_call(__script, __node, arg);
	if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
        return FFLY_FAILURE;
    }
	if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_err_t read_if_stmt(struct ffly_script *__script, struct node **__node) {
    ffly_err_t err;
    skip_token(__script);
    if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        return FFLY_FAILURE;
    }

    struct node *cond;
    if (_err(err = read_expr(__script, &cond)))
        return err;

    if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
        return FFLY_FAILURE;
    }

    struct ffly_vec _do;
    ffly_vec_set_flags(&_do, VEC_AUTO_RESIZE);
    ffly_vec_init(&_do, sizeof(struct node*));
    if (_err(err = read_compound_stmt(__script, &_do)))
        return err;
    vec_cleanup(__script, &_do);

    struct ffly_vec _else;
    ffly_vec_set_flags(&_else, VEC_AUTO_RESIZE);
    ffly_vec_init(&_else, sizeof(struct node*));
    if (next_token_is(__script, TOK_KEYWORD, _k_else)) {
        if (_err(err = read_compound_stmt(__script, &_else)))
            return err;
    }

    vec_cleanup(__script, &_else);
    ast_if(__script, __node, cond, &_do, &_else);
    return FFLY_SUCCESS;
}

ffly_err_t read_exit_stmt(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);
    if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_FAILURE;
    }

    ast_exit(__script, __node);
    return FFLY_SUCCESS;
}

ffly_err_t read_ret_stmt(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);

    struct node *ret;
    read_expr(__script, &ret);

    if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_FAILURE;
    }
    ast_ret(__script, __node, ret, __script->ret_type);
    return FFLY_SUCCESS;
}

ffly_err_t read_while_stmt(struct ffly_script*, struct node**);
ffly_err_t read_stmt(struct ffly_script *__script, struct node **__node) {
    ffly_err_t err;
    struct token *tok = peek_token(__script);
    if (is_keyword(tok, _k_if)) {
        if (_err(err = read_if_stmt(__script, __node))) {
            ffly_fprintf(ffly_out, "failed to read if statment.\n");
            return err;
        }
    } else if (is_keyword(tok, _k_exit)) {
        if (_err(err = read_exit_stmt(__script, __node))) {
            ffly_fprintf(ffly_out, "failed to read exit statment.\n");
            return err;
        }
    } else if (is_keyword(tok, _k_while)) {
        if (_err(err = read_while_stmt(__script, __node))) {
            ffly_fprintf(ffly_out, "failed to read while statment.\n");
            return err;
        }
    } else if (is_keyword(tok, _k_ret)) {
        if (_err(err = read_ret_stmt(__script, __node))) {
            ffly_fprintf(ffly_out, "failed to read return statment.\n");
            return err;
        }
    }
    return FFLY_SUCCESS;
}

ffly_err_t read_func_call(struct ffly_script *__script, struct node **__node, struct node *__call) {
    if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        return FFLY_FAILURE;
    }

    struct ffly_vec params;
    ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
    ffly_vec_init(&params, sizeof(struct node*));

    if (!next_token_is(__script, TOK_KEYWORD, _r_paren)) {
        _next:
        {
            struct node *param;
            read_expr(__script, &param);    
    
            struct node **p;
            ffly_vec_push_back(&params, (void**)&p);
            *p = param;

            if (next_token_is(__script, TOK_KEYWORD, _comma)) {
                goto _next;
            }
        }

        if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
            return FFLY_FAILURE;
        }
    }
    
    vec_cleanup(__script, &params);
    ast_func_call(__script, __node, __call, &params);
    return FFLY_SUCCESS;
}

ffly_err_t read_func_def(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);
    ffly_err_t err;
    struct token *name = next_token(__script);
    if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        return FFLY_FAILURE;
    }

    struct ffly_map local, *save = NULL;
    ffly_map_init(&local);
    save = __script->local;
    __script->local = &local;

    struct ffly_vec args;
    ffly_vec_set_flags(&args, VEC_AUTO_RESIZE);
    ffly_vec_init(&args, sizeof(struct node*)); 
    if (!next_token_is(__script, TOK_KEYWORD, _r_paren)) {
        _next:
        {
            struct node *decl, *var = NULL;
            struct token *tok = next_token(__script);
            struct type *_type;
            if (_err(err = read_decl_spec(__script, tok, &_type)))
                return err;
            if (_err(err = read_declarator(__script, &_type, _type)))
                return err;

            struct token *name = next_token(__script);
            ast_var(__script, &var, _type);

            ast_decl(__script, &decl, var, NULL);
         
            struct node **p;
            ffly_vec_push_back(&args, (void**)&p);
            *p = decl;

            ffly_map_put(&local, name->p, ffly_str_len((char*)name->p), var);
            if (next_token_is(__script, TOK_KEYWORD, _comma)) {  
                goto _next;
            }
        }
        if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
            return FFLY_FAILURE;
        }
    }

    vec_cleanup(__script, &args);
    if (!expect_token(__script, TOK_KEYWORD, _r_arrow)) {
        return FFLY_FAILURE;
    }

    struct token *tok = next_token(__script);
    struct type *ret_type;
    if (_err(err = read_decl_spec(__script, tok, &ret_type)))
        return err;

    struct ffly_vec block;
    ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
    ffly_vec_init(&block, sizeof(struct node*));

    __script->ret_type = ret_type;
    read_compound_stmt(__script, &block); 
    __script->local = save;
    ffly_map_de_init(&local);
   
    vec_cleanup(__script, &block);
    ast_func(__script, __node, &args, &block, ret_type);
    ffly_map_put(__script->local != NULL?__script->local:&__script->env, name->p, ffly_str_len((char*)name->p), *__node);
    return FFLY_SUCCESS;
}


ffly_bool_t is_func_call(struct ffly_script *__script) {
    struct token *name = next_token(__script);
    struct token *l_paren = next_token(__script);
    if (!name || !l_paren) return 0;
    mdl_u8_t res = !(name->kind != TOK_IDENT || !is_keyword(l_paren, _l_paren));
    ffly_script_ulex(__script, l_paren);
    ffly_script_ulex(__script, name);
    return res;
}

ffly_err_t read_while_stmt(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);
    ffly_err_t err;    
    if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        return FFLY_FAILURE;
    }

    struct node *cond;
    if (_err(err = read_expr(__script, &cond)))
        return err;

    if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
        return FFLY_FAILURE;
    }

    struct ffly_vec block;
    ffly_vec_set_flags(&block, VEC_AUTO_RESIZE);
    ffly_vec_init(&block, sizeof(struct node*));
    if (_err(err = read_compound_stmt(__script, &block)))
        return err;
    vec_cleanup(__script, &block);

    ast_while(__script, __node, cond, &block);
    return FFLY_SUCCESS;
}

ffly_err_t read_call(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);

    struct node *no;
    read_expr(__script, &no);    

    if (!expect_token(__script, TOK_KEYWORD, _l_arrow)) {
        return FFLY_FAILURE;
    }

    if (!expect_token(__script, TOK_KEYWORD, _l_brace)) {
        return FFLY_FAILURE;
    }

    struct ffly_vec params;
    ffly_vec_set_flags(&params, VEC_AUTO_RESIZE);
    ffly_vec_init(&params, sizeof(struct node*));

    if (!next_token_is(__script, TOK_KEYWORD, _r_brace)) {
        _next:
        {
            struct node *param;
            read_expr(__script, &param);

            struct node **p;
            ffly_vec_push_back(&params, (void**)&p);
            *p = param;

            if (next_token_is(__script, TOK_KEYWORD, _comma)) {
                goto _next;
            }
        }

        if (!expect_token(__script, TOK_KEYWORD, _r_brace)) {
            return FFLY_FAILURE;
        }
    }

    vec_cleanup(__script, &params);
    if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        return FFLY_FAILURE;
    }

    ast_call(__script, __node, no, &params);
    return FFLY_SUCCESS;
}

ffly_err_t ffly_script_parse(struct ffly_script *__script) {
    ffly_err_t err = FFLY_SUCCESS;
	while((__script->p+__script->off) < __script->end) {
		struct token *tok = peek_token(__script);
		struct node *_node = NULL;
		if (!tok) break; 
        if (is_type(__script, tok)) {
            if (_err(err = read_decl(__script, &_node))) {
                ffly_fprintf(ffly_out, "failed to read decl.\n");
                break;
            }
        } else if (tok->kind == TOK_KEYWORD) {
	        if (tok->id == _k_print) {
				if (_err(err = read_print_call(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read print call.\n");
                    break;
                }
			} else if (tok->id == _k_fn) {
                if (_err(err = read_func_def(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read function.\n");
                    break;
                }
            } else if (tok->id == _colon) {
                if (_err(err = read_call(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read call.\n");
                    break;
                }
            } else {
                if (tok->id == _k_if || tok->id == _k_exit || tok->id == _k_while) {
                    if (_err(err = read_stmt(__script, &_node))) {
                        ffly_fprintf(ffly_out, "failed to read statement.\n");
                        break;
                    }
                }
            }
		} else if (tok->kind == TOK_IDENT) {
            if (_err(err = read_stmt(__script, &_node))) {
                ffly_fprintf(ffly_out, "failed to read statement.\n");
                break;
            }

            if (!_node) {
                if (_err(err = read_expr(__script, &_node))) {
                    ffly_fprintf(ffly_out, "failed to read expression.\n");
                    break;
                }
            
                expect_token(__script, TOK_KEYWORD, _semicolon);
            }
        }

		//pr_tok(tok);
		if (_node != NULL) {
			struct node **p;
			ffly_vec_push_back(&__script->nodes, (void**)&p);
			*p = _node;
		}
	}
    if (_err(err))
        return err;
    return FFLY_SUCCESS;
}
