# define __ffly_script_internal
# include "script.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "system/nanosleep.h"
# include "data/mem_cpy.h"
void build_node(struct node **__node, struct node *__tmpl) {
	*__node = (struct node*)__ffly_mem_alloc(sizeof(struct node));
	**__node = *__tmpl;
}

void build_type(struct type **__type, struct type *__tmpl) {
	*__type = (struct type*)__ffly_mem_alloc(sizeof(struct type));
	**__type = *__tmpl;
}

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

void make_notype(struct type **__type, mdl_u8_t __id) {
	struct type *tmpl;
	switch(__id) {
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
	build_type(__type, tmpl);
}

void ast_decl(struct node **__node, struct node *__var, struct node *__init) {
	build_node(__node, &(struct node){.kind=_ast_decl, .var = __var, .init = __init});
}

void ast_int_type(struct node **__node, struct type *__type, ffly_byte_t *__p) {
	build_node(__node, &(struct node){.kind=_ast_literal, ._type=__type});
	ffly_mem_cpy((*__node)->val, __p, __type->size);
}

void ast_print_call(struct node **__node, struct node *__arg) {
	build_node(__node, &(struct node){.kind=_ast_print_call, .arg=__arg});
}

void ast_var(struct node **__node, struct type *__type) {
    build_node(__node, &(struct node){.kind=_ast_var, ._type=__type});
}

void read_decl_spec(struct token *__tok, struct type **__type) {
	if (__tok->id >= _k_uint_t && __tok->id <= _k_i8_t) {
		make_notype(__type, __tok->id);
	}
}

# include "system/string.h"
void read_no(struct ffly_script *__script, struct node **__node, char *__s) {
	mdl_u64_t val = ffly_stno(__s);
	struct type *_type;
	if (val >= 0 && val <= (mdl_u8_t)~0)
		_type = u8_t;
	else if (val > (mdl_u8_t)~0 && val <= (mdl_u16_t)~0)
		_type = u16_t;
	else if (val > (mdl_u16_t)~0 && val <= (mdl_u32_t)~0)
		_type = u32_t;
	else if (val > (mdl_u32_t)~0 && val <= (mdl_u64_t)~0)
		_type = u64_t;
	ast_int_type(__node, _type, (ffly_byte_t*)&val);
}

void read_primary_expr(struct ffly_script *__script, struct node **__node) {
	struct token *tok = next_token(__script);
	if (!tok) return;
	switch(tok->kind) {
        case TOK_IDENT: {
            *__node = (struct node*)ffly_map_get(&__script->env, tok->p, ffly_str_len((char*)tok->p));
            if (!*__node) {
                ffly_fprintf(ffly_err, "%s could not find in environment.\n", (char*)tok->p);
            }
            break;
        }
		case TOK_NO:
			read_no(__script, __node, (char*)tok->p);
		break;
	}
}

void read_assign_expr(struct ffly_script *__script, struct node **__node) {
	read_primary_expr(__script, __node);
}

void read_expr(struct ffly_script *__script, struct node **__node) {
	read_assign_expr(__script, __node);
}

void read_decl_init(struct ffly_script *__script, struct node **__node) {
	read_expr(__script, __node);
}

void read_decl(struct ffly_script *__script, struct node **__node) {
	struct type *_type, *init = NULL;
	struct token *tok = next_token(__script);
	read_decl_spec(tok, &_type);
	struct token *name = next_token(__script);
	if (name->kind != TOK_IDENT) {
		ffly_fprintf(ffly_err, "%u: wanted ident got %s\n", curl(__script), tokk_str(name->kind));
	}

	if (next_token_is(__script, TOK_KEYWORD, _eq))
		read_decl_init(__script, &init);

	if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        ffly_fprintf(ffly_err, "%u: expected semicolon.\n", curl(__script));
    }

    struct node *var;
    ast_var(&var, _type);

	ast_decl(__node, var, init);
    ffly_map_put(&__script->env, name->p, ffly_str_len((char*)name->p), var);
}

void read_print_call(struct ffly_script *__script, struct node **__node) {
    skip_token(__script);
	if (!expect_token(__script, TOK_KEYWORD, _l_paren)) {
        ffly_fprintf(ffly_err, "%u: expected left paren.\n", curl(__script));
    }
	struct node *arg;
	read_expr(__script, &arg);
	ast_print_call(__node, arg);
	if (!expect_token(__script, TOK_KEYWORD, _r_paren)) {
        ffly_fprintf(ffly_err, "%u: expected right paren.\n", curl(__script));
    }

	if (!expect_token(__script, TOK_KEYWORD, _semicolon)) {
        ffly_fprintf(ffly_err, "%u: expected semicolon.\n", curl(__script));
    }
}

ffly_err_t ffly_script_parse(struct ffly_script *__script) {
	while((__script->p+__script->off) < __script->end) {
		ffly_err_t err;
		struct token *tok = peek_token(__script);
		struct node *_node = NULL;
		if (!tok) break;
		if (tok->kind == TOK_KEYWORD) {
			if (tok->id >= _k_uint_t && tok->id <= _k_i8_t) {
				read_decl(__script, &_node);
			}
			if (tok->id == _k_print) {
				read_print_call(__script, &_node);
			}
		}
		pr_tok(tok);
		if (_node != NULL) {
			struct node **p;
			ffly_vec_push_back(&__script->nodes, (void**)&p);
			*p = _node;
		}
	}
}
